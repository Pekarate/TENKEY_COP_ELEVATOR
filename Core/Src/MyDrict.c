/*
 * MyDrict.c
 *
 *  Created on: Sep 15, 2021
 *      Author: User
 */



#include "AllHeader.h"

/************************************************************************************************/
/* Structure for object dictionary																*/
/************************************************************************************************/
_dict dict []= {
/*	uint16_t object,		uint8_t sub,		uint8_t access,	uint8_t size,				void *pointer,				uint8_t ee_address	*/

	{ERROR_REGISTER,	0,				0,				1,						(void*)&errorregister,		0				},
	{DEVICE_NAME,		0,				0,				6,						(void*)device_name,			0				},
	{HW_VERSION,		0,				0,				5,						(void*)hardware_uea,		0				},
	{SW_VERSION,		0,				0,				5,						(void*)version_uea,			0				},
	{CONS_HB_TIME,		8,				0,				4,						(void*)0,					0				},
	{IDENTITY,			4,				1,				4,						(void*)0,					E_VENDOR		},

	{LIFT_NUMBER,		0,				1,				1,						(void*)&disp_lift,			E_LIFT			},
	{INPUT_PARA1,		8,				1,				4,						(void*)0,					0				},
	{INPUT_PARA2,		8,				1,				4,						(void*)0,					0				},
	{INPUT_PARA3,		MAX_IN,			1,				4,						(void*)in_polarity,			0				},
	{INPUT_PARA4,		MAX_IN,			1,				4,						(void*)&inpar[0][IO_ENABLE],0				},
	{OUTPUT_PARA1,		8,				1,				4,						(void*)0,					0				},
	{OUTPUT_PARA2,		8,				1,				4,						(void*)0,					0				},
	{OUTPUT_PARA3,		MAX_IN,			1,				4,						(void*)out_polarity,		0				},
	{OUTPUT_PARA4,		MAX_OUT,		1,				4,						(void*)&outpar[0][IO_ENABLE],0				},
	{VIRTUAL_INPUT,		0,				0,				MAX_IO_TYPE,			(void*)0,					0				},
	{VIRTUAL_OUTPUT,	0,				1,				MAX_IO_TYPE,			(void*)0,					0				},
	{INPUT_GROUP,		MAX_IN,			1,				MAX_IO_TYPE,			(void*)0,					0				},
	{OUTPUT_GROUP,		MAX_OUT,		1,				MAX_IO_TYPE,			(void*)0,					0				},

	{PORT_IN,			9,				0,				1,						(void*)in,					0				},
	{PORT_OUT,			9,				1,				1,						(void*)out,					0				},

	{IOU_EXT_NUMS,		0,				1,				1,						(void*)&mExtern_Number,		0,				},
	{ARROW_MODE,		0,				1,				1,						(void*)&mArrow_State,		0,				},

	{ENDSTRING,			0,				0,				0,						(void*)0,					0				},
};

/************************************************************************************************/
/* Read object dictionary																		*/
/************************************************************************************************/
uint32_t read_dict (uint8_t pos, uint8_t subindex){
	uint16_t address;
	uint8_t ee_addr = 0;
	uint8_t i;
	uint8_t value [4];
	uint8_t size;
	size = dict [pos].size;

	if (subindex)										/* more than 1 subindex					*/
		{
			address = (subindex - 1) * size;
			ee_addr = subindex - 1;
		}
	else
		{
			ee_addr = 0;
			address = 0;
		}
	switch (dict [pos].object)
		{
			case (INPUT_PARA3):
			case (OUTPUT_PARA3):
				return (((uint32_t)bit_select (*((uint8_t *)dict [pos].pointer + ee_addr/8), ee_addr%8)) << 15);

			case (INPUT_PARA4):
				return (*((uint8_t *)dict[pos].pointer + ((uint16_t)(subindex - 1) * MAX_IO_TYPE)));

			case (OUTPUT_PARA4):
				return (*((uint8_t *)dict[pos].pointer + ((uint16_t)(subindex - 1) * (MAX_IO_TYPE + 1))));

			case (CONS_HB_TIME):
				return ((((uint32_t)subindex) << 16) + (HSETIME * 500));

			default:
				*(uint32_t *)value = 0;						/* clear variable						*/
				if (dict [pos].pointer)				/* object in RAM						*/
					{
						for (i = 0; i < size; i++)
		        	value [i] = *((uint8_t *)dict [pos].pointer + address + i);
						return (*(uint32_t *)value);
					}
				else
					return (0);
		}
}

/************************************************************************************************/
/* Write object dictionary																		*/
/************************************************************************************************/
uint8_t write_dict (uint8_t pos, uint8_t subindex, uint32_t value){
	uint8_t i;
	uint8_t j;
	uint16_t address;
	uint8_t ee_addr;
	uint8_t size;

	size = dict [pos].size;

	if (subindex)			// more than 1 subindex
		{
			address = ((uint16_t)(subindex - 1)) * size;
			ee_addr = subindex - 1;
		}
	else
		{
			ee_addr = 0;
			address = 0;
		}
	switch (dict [pos].object)
		{
			case (INPUT_PARA3):
			case (OUTPUT_PARA3):
				i = *((uint8_t *)dict [pos].pointer + ee_addr/8);
				j = (value >> 15) & 1;
				if (((i >> (ee_addr % 8)) & 1) != j)
					{
						if (j)
							bit_set (i, ee_addr % 8);
						else
							bit_reset (i, ee_addr % 8);
					}
				*((uint8_t *)dict [pos].pointer + ee_addr/8) = i;
				return (0);

			case (INPUT_PARA4):
				if (dict[pos].pointer)
					{
						*((uint8_t *)dict[pos].pointer + ((uint16_t)(subindex - 1)) * MAX_IO_TYPE) = (uint8_t)value;
					}
				break;

			case (OUTPUT_PARA4):
				if (dict[pos].pointer)
					{
						*((uint8_t *)dict[pos].pointer + ((uint16_t)(subindex - 1)) * (MAX_IO_TYPE + 1)) = (uint8_t)value;
					}
				break;

			default:
				if (dict [pos].pointer)						/* object in RAM						*/
					{
						for (i = 0; i < size; i++)
		     		 	*((uint8_t *)dict [pos].pointer + address + i) = *((uint8_t *)&value+i);
					}
				break;
		}
	return (0);
}


/************************************************************************************************/
/* Search object dictionary for an entry														*/
/************************************************************************************************/
uint32_t search_dict (uint16_t index, uint8_t subindex, uint8_t type, uint8_t *pos){
	uint8_t i;
	uint8_t index_exists;
	uint8_t subindex_exists;
	uint8_t size;
	i = 0;
	index_exists = 0;
	subindex_exists = 0;
	while (dict [i].object != ENDSTRING)		//search whole dictionary
		{
			if (dict [i].object == index)					//found object
				{
					if (subindex <= dict [i].sub)				//subindex of object valid
						{
							if ((type & COMMAND_SPECIFIER) == INIT_WRITE_REQ)
								{/// write access to object dictionary
									size = dict [i].size;
									if ((type & EXPEDITED_BIT) && (size > 4) &&	 (subindex || (!dict [i].sub)))
										return (SDO_UNSUPPORTED);		// exped. transfer to object > 4 uint8_ts
									else if (!((type & EXPEDITED_BIT)) && (size <= 4))
										return (SDO_UNSUPPORTED);		// normal transfer to object <= 4 uint8_ts
									else if (!dict [i].access)
										return (SDO_WRITE_OF_RO);		// write access to read only object
									else if ((!subindex) && dict [i].sub && index != PORT_OUT && index != PORT_IN)
										return (SDO_WRITE_OF_RO);		// write access to number of entries
									*pos = i;							//set number of entry in dictionary
									return (0);						//write access possible
								}
							else										//read access
								{
									*pos = i;							//set number of entry in dictionary
									return (0);						//read access possible
								}
							subindex_exists = 1;		//object exists,but access not valid
						}
					index_exists = 1;					//object exists,but subindex not valid
				}
			i++;												//go to next dictionary entry
		}
	if (subindex_exists)					// object exists,but access not valid
		return (SDO_WRITE_OF_RO);	// set error code
	if (index_exists)							// object exists,but subindex not valid
		return (SDO_SUB_WRONG);		// set error code
	return (SDO_NOT_EXIST);				// no entry in object dictionary
}

