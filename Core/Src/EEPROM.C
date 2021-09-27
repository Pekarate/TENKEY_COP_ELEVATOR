
#define	_EEPROM_C_
#include "AllHeader.h"

/************************************************************************************************/
/* read a single byte from EEPROM																*/
/************************************************************************************************/
BYTE read_eeprom (BYTE address){
	EEADR 				= address;						/* write address to EEPROM register		*/
	EECON1bits.EEPGD 	= 0;							/* point to EEPROM						*/
	EECON1bits.CFGS 	= 0;							/* access to EEPROM						*/
	EECON1bits.RD 		= 1;							/* initiates EEPROM read				*/
	return (EEDATA);									/* read EEPROM data						*/
}

/************************************************************************************************/
/* write a single byte to EEPROM																*/
/************************************************************************************************/
BYTE write_eeprom (BYTE address, BYTE value){
	EEADR 				= address;						/* write address to EEPROM register		*/
	EEDATA				= value;						/* write value to EEPROM register		*/
	EECON1bits.EEPGD 	= 0;							/* point to EEPROM						*/
	EECON1bits.CFGS 	= 0;							/* access to EEPROM						*/
	EECON1bits.WREN		= 1;							/* enables EEPROM write					*/
	INTCONbits.GIEH		= 0;							/* high priority interrupts disable		*/
	INTCONbits.GIEL		= 0;							/* low priority interrupts disable		*/
	EECON2				= 0x55;							/* special write enable sequence		*/
	EECON2				= 0xAA;
	EECON1bits.WR 		= 1;							/* initiates EEPROM write				*/
	INTCONbits.GIEH		= 1;							/* high priority interrupts enable		*/
	INTCONbits.GIEL		= 1;							/* low priority interrupts enable		*/
	ClrWdt ();										/* reset watchdog timer					*/
	while (!PIR2bits.EEIF);								/* wait until write is ready			*/
	PIR2bits.EEIF 		= 0;							/* reset EEPROM write ready bit			*/
	EECON1bits.WREN		= 1;							/* disables EEPROM write				*/
	if (read_eeprom (address) == value)					/* read EEPROM back to verify			*/
		return (1);										/* return ok.							*/
	return (0);											/* return error							*/
}

/************************************************************************************************/
/* read a number of byte from EEPROM															*/
/************************************************************************************************/
BYTE read_eeprom_data (BYTE startaddress, BYTE size, BYTE *value){
	BYTE i;
	for (i = 0; i < size; i++)
		*value++ = read_eeprom (startaddress++);
	return (1);											/* return ok.							*/
}

/************************************************************************************************/
/* write a number of byte to EEPROM																*/
/************************************************************************************************/
BYTE write_eeprom_data (BYTE startaddress, BYTE size, BYTE *value){
	BYTE i;
	for (i = 0; i < size; i++)
		{
			if (!write_eeprom (startaddress++, *value++))	/* write single byte to EEPROM			*/
				return (0);									/* return error							*/
		}
	return (1);											/* return ok.							*/
}


