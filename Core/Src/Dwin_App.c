/*
 * Dwin_App.c
 *
 *  Created on: Dec 31, 2021
 *      Author: User
 */


#include "Dwin.h"
#include "Dwin_App.h"
#include "AllHeader.h"
#include "stdio.h"
extern uint8_t aBCAN_ReceiveBuf_Clock[8];
extern uint8_t aBCAN_ReceiveBuf_Clock_old[8];
extern uint8_t Callstatus[8] ;
extern uint8_t Callstatus_old[8] ;

extern uint8_t Arrow_state;

uint8_t		FloorName_Registed[300];
uint16_t	FloorName_Registed_length;
uint8_t 	Tol_Floor_Registed;
uint16_t FloorName_Registed_Page;
uint32_t FloorName_Registed_Time;

_Message User_Message = {0};

void Dwin_Change_Current_FloorName(char * flName)
{
	uint8_t Buf[20] = {0};
	Buf[0]=0x10;
	Buf[1]=0x01;
	uint16_t tmp;
	if((display [BUF_TEN] == 0x20) || (display [BUF_TEN] == 0x00) )
	{
		if(( display [BUF_UNIT]<0x40) &&( display [BUF_UNIT]>0x29))
			tmp=FLOOR_NAME_X- 50;
		else
			tmp=FLOOR_NAME_X - 60;
		Buf[2]=tmp>>8;
		Buf[3]=tmp;

	}
	else
	{
		Buf[2]=FLOOR_NAME_X>>8;
		Buf[3]=FLOOR_NAME_X;
	}
	Buf[4]=FLOOR_NAME_Y>>8;
	Buf[5]=FLOOR_NAME_Y;
	Dwin_Write_VP(FLOORNAME_SP,Buf,3);        //change location
	Buf[0] = 0;
	Buf[2] = 0;
	Buf[1] = flName[0];
	Buf[3] = flName[1];
	Dwin_Write_VP(FLOORNAME_VP, Buf, 2);
}
void Dwin_switch_to_next_page(void)
{
	static int PageCnt = 0;
	uint8_t Buf[20] = {0};
	Buf[0]= 0x5A;
	Buf[1] = 0x01;
	Buf[2] = 0x00;
	Buf[3] = PageCnt;
	Dwin_Write_VP(0X0084,Buf,2) ;
	PageCnt++;
	if(PageCnt == MAX_PAGE)
	{
		PageCnt = 0;
	}
}
void Dwin_update_time(void)
{
	int WeekDay;
	uint8_t Buf[20] = {0};
	Buf[0]=aBCAN_ReceiveBuf_Clock[2]/10 +0x30;
	Buf[1]=aBCAN_ReceiveBuf_Clock[2]%10 +0x30;
	Buf[2]=':';
	Buf[3]=aBCAN_ReceiveBuf_Clock[1]/10 +0x30;
	Buf[4]=aBCAN_ReceiveBuf_Clock[1]%10 +0x30;
	Buf[5]=0;
	Dwin_Write_VP(TIME_VP,Buf,3);        //set time
	Buf[0]=aBCAN_ReceiveBuf_Clock[3]/10 +0x30;
	Buf[1]=aBCAN_ReceiveBuf_Clock[3]%10 +0x30;
	Buf[2]='.';
	Buf[3]=(aBCAN_ReceiveBuf_Clock[4] >> 3) / 10 +0x30;
	Buf[4]=(aBCAN_ReceiveBuf_Clock[4] >> 3) % 10 +0x30;
	Buf[5]='.';
	Buf[6]=aBCAN_ReceiveBuf_Clock[5]/10 +0x30;
	Buf[7]=aBCAN_ReceiveBuf_Clock[5]%10 +0x30;
	Dwin_Write_VP(DATE_VP,Buf,4);        //set time

	WeekDay= aBCAN_ReceiveBuf_Clock[4]&0x07;
	switch(WeekDay)
	{
		case 1:
			sprintf((char *)Buf,"MONDAY   ");
			break;
		case 2:
			sprintf((char *)Buf,"TUESDAY  ");
			break;
		case 3:
			sprintf((char *)Buf,"WEDNESDAY");
			break;
		case 4:
			sprintf((char *)Buf,"THURSDAY ");
			break;
		case 5:
			sprintf((char *)Buf,"FRIDAY    ");
			break;
		case 6:
			sprintf((char *)Buf,"SATURDAY  ");
			break;
		case 7:
			sprintf((char *)Buf,"SUNDAY    ");
			break;
		default:
			break;
	}
	Dwin_Write_VP(WEEKDAY_VP,Buf,5);        //set time
}
void DWin_Calltable_Process(void)
{
	uint16_t i;
	uint16_t j;
	uint8_t change;
	change=0;
	uint8_t Buf[100] = {0};
	for(i =0;i<50;i++)
	{
		Buf[i]=0;
	}

	for(i=0;i<8;i++)
	{
		if(Callstatus[i] != Callstatus_old[i])
		{
			change=1;
			Callstatus_old[i] = Callstatus[i];
		}
	}
	if(change)
	{
			for(i =0;i<150;i++)
			{
				FloorName_Registed[i]=0;
			}
			Tol_Floor_Registed=0;
			FloorName_Registed_length=0;

			for(i=0;i<8;i++)
			{
				for(j=0;j<8;j++)
				{

					if(bit_select(Callstatus_old[i],j))
					{
						FloorName_Registed[FloorName_Registed_length]= 0x20;
						FloorName_Registed_length++;
						if((FloorName[i*8+j][0] !=0x20) && (FloorName[i*8+j][0]!=0x00))
						{
							FloorName_Registed[FloorName_Registed_length]= FloorName[i*8+j][0];
							FloorName_Registed_length++;
						}
						FloorName_Registed[FloorName_Registed_length]= FloorName[i*8+j][1];
						FloorName_Registed_length++;
						FloorName_Registed[FloorName_Registed_length]= 0x20;
						FloorName_Registed_length++;
						FloorName_Registed[FloorName_Registed_length]= '|';
						FloorName_Registed_length++;
						Tol_Floor_Registed++;
					}

				}
			}

			if(Tol_Floor_Registed	==1)
			{
				FloorName_Registed_Time=0;
				FloorName_Registed_Page =0;
			}
	}
	if((HAL_GetTick() > FloorName_Registed_Time ) || (change))
	{
		uint16_t index_start =0;
		uint16_t forcnt =0;
		uint16_t Count =0; //cont '|'

		index_start = FloorName_Registed_Page *6;
		if(index_start)
		{
			for (forcnt =0;forcnt<FloorName_Registed_length;forcnt++)
			{
				if(FloorName_Registed[forcnt] == '|')
				{
					Count++;
					if(Count == index_start)
					{
						index_start = forcnt+1;
						break;
					}
				}
			}
		}


		Count =0;
		for(forcnt = index_start;forcnt<FloorName_Registed_length;forcnt++)
		{
			Buf[forcnt-index_start] = FloorName_Registed[forcnt];
			if(Buf[forcnt-index_start] == '|')
			{
				Count++;
				if(Count ==6)
				{
					Buf[forcnt-index_start]=0x20; //remove '|'
					break;
				}

			}

		}
		Dwin_Write_VP(0x1400,(uint8_t *)Buf,(forcnt-index_start)/2+2);
		FloorName_Registed_Page++;
		if(FloorName_Registed_Page > ((Tol_Floor_Registed-1) /6))
		{
			FloorName_Registed_Page=0;
		}
		FloorName_Registed_Time = HAL_GetTick()+1500;
	}
	return;

}
void DWIN_Arrow_Process(void)
{
	static uint8_t Arrow_state_old;
	uint8_t arrow;
	uint8_t Buf[100] = {0};
	if(Arrow_state_old == Arrow_state)
		return ;
	Arrow_state_old = Arrow_state;
	Buf[0]=00;
	Buf[2]=00;
	Buf[4]=00;
	switch(Arrow_state_old)
	{
		case 0:   //hide

			Buf[1]=ARROW_STOP;
			Buf[3]=ARROW_STOP;
			Buf[5]=ARROW_STOP;
			break;
		case 0x01:
			Buf[1]=ARROW_UP_START;
			Buf[3]=ARROW_UP_START;
			Buf[5]=ARROW_UP_START;
			break;
		case 0x02:
			Buf[1]=ARROW_DOWN_START;
			Buf[3]=ARROW_DOWN_START;
			Buf[5]=ARROW_DOWN_START;
			break;
		case 0x11:
			Buf[1]=ARROW_UP_STOP;
			Buf[3]=ARROW_UP_START;
			Buf[5]=ARROW_UP_END;

			break;
		case 0x22:
			Buf[1]=ARROW_DOWN_STOP;
			Buf[3]=ARROW_DOWN_START;
			Buf[5]=ARROW_DOWN_END;
			break;
		default:
			break;
	}
	Dwin_Write_VP(ARROW_ICON_SP+6,(uint8_t *)&Buf,3);
}
static uint8_t virt_key[2] = {0x20,0x20};
static uint8_t virt_key_cnt =0;
uint32_t Keytimout=0;
void DWIN_add_key(uint8_t key)
{
	if (key!= 'C')
	{
		virt_key[virt_key_cnt] =key;
		virt_key_cnt++;
		if(virt_key_cnt ==2)
		{
			virt_key_cnt =0;
		}
		else if(virt_key_cnt ==1)
		{
			virt_key[virt_key_cnt] = 0x20;
		}
	}
	else
	{
		Keytimout = 0;
	}
}

void DWIN_Message_Process(void)
{
	static uint8_t virt_key_old[2];
	uint8_t Buf[100] = {0};
	if((virt_key[0]!=virt_key_old[0])||(virt_key[1]!=virt_key_old[1]))
	{
		virt_key_old[0] = virt_key[0];
		virt_key_old[1] = virt_key[1];
		Keytimout = HAL_GetTick()+1500;
		for(int cnt =0;cnt<30;cnt++)
		{
			Buf[cnt] =0;
		}
		Dwin_Write_VP(0x1500,Buf,15);     //write buf to VP
		Buf[0]=00;
		Buf[1]=virt_key_old [0];
		Buf[2]=00;
		Buf[3]=virt_key_old [1];
		Dwin_Write_VP(0x1100,Buf,2);        //set floor name
	}
	if(HAL_GetTick()>Keytimout)
	{
		sprintf((char *)Buf,"PLEASE ENTER DESTINATION FLOOR");
		Dwin_Write_VP(0x1500,Buf,15);     //write buf to VP
		virt_key_cnt =0;
			Buf[0]=00;
			Buf[1]=0x20;
			Buf[2]=00;
			Buf[3]=0x20;
			virt_key_old[0] = virt_key[0] = 0x20;
			virt_key_old[1] = virt_key[1] = 0x20;
			Dwin_Write_VP(0x1100,Buf,2);        //set floor name
			Keytimout = 0-1;
	}
}

