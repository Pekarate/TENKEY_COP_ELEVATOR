/*
 * Dwin.c
 *
 *  Created on: Dec 31, 2021
 *      Author: User
 */

#include "Dwin.h"
#include "main.h"
#include  "AllHeader.h"
extern UART_HandleTypeDef huart1;
#define D_PORT huart1
static void Dwin_write(uint8_t *data,uint16_t len)
{
	HAL_UART_Transmit(&D_PORT, data, len, 100);
}
int Dwin_Write_VP(uint16_t Addr,uint8_t *data,uint16_t len)
{
	uint8_t DwinBuf[100];
	DwinBuf[0]= 0x5A;
	DwinBuf[1]= 0xA5;
	DwinBuf[2]= (len+1)*2 +1;  //
	DwinBuf[3]= 0x82;
	DwinBuf[4]= Addr>>8;
	DwinBuf[5]= Addr;
	for(int i=0;i<len;i++)
	{
		DwinBuf[6+i*2] = data[i*2];
		DwinBuf[6+i*2+1] = data[i*2+1];
	}
    len = (len+1)*2 +4;
    Dwin_write(DwinBuf, len);
	return 1;
}


