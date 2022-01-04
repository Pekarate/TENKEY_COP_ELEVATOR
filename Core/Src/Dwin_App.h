/*
 * Dwin_App.h
 *
 *  Created on: Dec 31, 2021
 *      Author: User
 */

#ifndef SRC_DWIN_APP_H_
#define SRC_DWIN_APP_H_

#include "stdint.h"
#define DATE_VP 0x1006
#define DATE_VP_SIZE 8

#define FLOORNAME_VP 0x1000
#define FLOORNAME_SP 0x5100
#define FLOORNAME_VP_SIZE 8


#define WEEKDAY_VP 0x1010
#define WEEKDAY_VP_SIZE 10

#define TIME_VP 0x1025
#define TIME_VP_SIZE 6

#define ARROW_ICON_SP   0x5000
#define ARROW_UP_STOP 	0
#define ARROW_UP_START 	0
#define ARROW_UP_END		54

#define ARROW_DOWN_STOP 		55
#define ARROW_DOWN_START 	55
#define ARROW_DOWN_END			109

#define ARROW_STOP 		110

#define TOTALFLOORNAME_VP 0x1300
#define TOTALOORNAME_FLASH_ADDR 0x000002
#define TOTALFLOORNAME_LENGH 60

#define FLOOR_NAME_X 120
#define FLOOR_NAME_Y 208

#define MAX_PAGE 5

typedef struct
{
	char Data[50];
	uint8_t  DataSize;
	uint32_t Timeshow;
}_Message;

void Dwin_Change_Current_FloorName(char * flName);
void Dwin_switch_to_next_page(void);
void Dwin_update_time(void);
void DWin_Calltable_Process(void);
void DWIN_Arrow_Process(void);
void DWIN_Message_Process(void);
void DWIN_add_key(uint8_t key);
#endif /* SRC_DWIN_APP_H_ */
