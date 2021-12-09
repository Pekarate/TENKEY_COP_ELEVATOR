
#ifndef	_PIC_MAIN_H_
#define	_PIC_MAIN_H_

#include "main.h"

#define TOTAL_FLOOR 55
#define DATA_START_ADDRESS 		 	((uint32_t)0x0801FC00)	//Page 127
#define LENGTH_START_ADDRESS 		((uint32_t)0x0801F810)	//Page 126

#define	BIT_0			0x01
#define	BIT_1			0x02
#define	BIT_2			0x04
#define	BIT_3			0x08
#define	BIT_4			0x10
#define	BIT_5			0x20
#define	BIT_6			0x40
#define	BIT_7			0x80

#define	true		1
#define	false		0

#define	BUZ_WORKING	BIT_0
#define	BUZ_PULSE		BIT_1

#define	MAX_IN					70		//24�����밴ť
#define	MAX_OUT				70		//24����ť���2����վ�����
#define	MAX_IN_BYTE		(MAX_IN/8+1)
#define	MAX_FLOOR			60		

//��������������״̬
#define	WD_MERKER			0xAA		// restart by watchdog timer			
#define BS_MERKER			0x55		// restart by CAN bus off				
#define RC_MERKER			0x5A		// restart by error in rc pointer	

//��������ֽڶ���
#define	IO_BASIC_FUNC		0				// basic function of input / output
#define	IO_SUB_FUNC			1				// sub function of input / output	
#define	IO_LIFT				2					// lift number of input / output	
#define	IO_FLOOR			3					// floor number of input / output	
#define	IO_DOOR				4				// door number of input / output	
#define	IO_STATE			5					// state of input / output				
#define	IO_ENABLE		6
#define	IO_ACK				7					// acknowledgement marker for calls
#define	PIC_ENABLE				1
#define	PIC_DISABLE			0

#define	MAX_IO				70
#define	MAX_LIFT			8
#define	MAX_IO_TYPE	7

//��ť�Ƶ���ʾʱ��
#define	C_BAUDRATE_LISTEN_15S	30	//15s
#define	C_MINIMAM_TIMES			4
#define	PUSHTIME							4						// time for push button indication off	
#define	HSETIME							20				// superv. time HSE	(UEA:10s,PSE:0,2s)
#define	HEARTBEAT_TIME				4		///2s
#define ERROR_TIME					20
#define FLASH_TIME			7

#define	ENDSTRING	0xFFFE

//��ʱ������ֵ
#define	T0START_1TO128	0x86	//prescaler 1 : 128
#define	T0H_500ms			0x67		//0xc5
#define	T0L_500ms			0x69		//0x59

#define	T1START_1TO4	0xA1	//prescaler 1 : 4	
#define	T1H_10ms			0x9E		//0xF0		Timer 1 preset value 10 ms			
#define	T1L_10ms			0x58		//0x60

#define	T3START_1TO1	0xA1	//0x91		Start Timer 3, prescaler 1 : 4		
#define	T3H_20ms			0x3C		//0x63		Timer 3 preset value 20 ms			
#define	T3L_20ms			0xB0		//0xC0

//�����
#define bit_select(B, i)	((B >> i) & 1)				// check if bit is set	
#define bit_set(B, i)		(B |= (1 << i))				// set bit						
#define bit_reset(B, i)		(B &= ~(1 << i))		// reset bit					

#define	Close_RunLed() LATBbits.LATB6 = 1
#define	Open_RunLed() LATBbits.LATB6 = 0
#define	Disable_BUZ()	HAL_GPIO_WritePin(Buzzer_Out_GPIO_Port, Buzzer_Out_Pin, GPIO_PIN_RESET)
#define	Enable_BUZ()	HAL_GPIO_WritePin(Buzzer_Out_GPIO_Port, Buzzer_Out_Pin, GPIO_PIN_SET)

#ifdef	_MAIN_C_

//�������

uint8_t						inpar  [MAX_IN][MAX_IO_TYPE];					/* input parameter						*/
uint8_t						outpar [MAX_OUT][MAX_IO_TYPE + 1];					/* output parameter						*/

uint8_t						buzzer = 0;						/* buzzer state							*/
	//bit0: 1-->���������б�־			0-->������������
	//bit1: 1-->�������Ǽ�Ъʽ��  	0-->��������ʽ��

//CanOpen ͨ�ŵı���
uint8_t 						node_id;
uint8_t						disp_lift;						/* lift number assigned to display		*/
uint8_t						nmtstate;						/* network management state 			*/
uint16_t	volatile	sdo_index = 0;					/* index for SDO segment transfer		*/
uint8_t	volatile	sdo_subindex = 0;				/* index for SDO segment transfer		*/
uint8_t						sdo_timer = 0;					/* timer for SDO segment transfer		*/

uint8_t	volatile	input [3][MAX_IN_BYTE];						/* input variables for debouncing		*/
uint8_t 	volatile	in[MAX_IN_BYTE];								/* variable for input after debouncing	*/
uint8_t						inold[MAX_IN_BYTE];							/* variable to save old input state		*/
uint8_t 	volatile	out[MAX_IN_BYTE];							/* output state							*/
uint8_t						in_polarity[MAX_IN_BYTE];					/* input is inverted / not inverted		*/

uint8_t 	mExtern_Number = 0;			//��չ������
uint8_t 	mInOut_Number = 22;			//�����������
uint8_t	mMax_InByte;
uint8_t	mArrow_State;				//��ͷ��ʾ״̬(��ֹ����˸������)

/************************************************************************************************/
/* variable for HSE message supervising															*/
/************************************************************************************************/
uint8_t	volatile	nmtwait;						/* waiting time for 1. heartbeat		*/
uint8_t baudrate = 0;

uint8_t	hse_heartbeat = 0;
uint8_t	bBusOffTimer = 0;
uint8_t 	volatile		hsetime;				// HSE supervising HSE
uint8_t	heartbeat;						/* timer for transmitting heartbeat		*/
uint8_t	att_alarm_timer = 0;
uint8_t	buz_alarm_timer = 0;
uint8_t	buz_alarm_totaltimer = 0;

struct {
	uint8_t Time_10ms: 1;		//10ms ��־
	uint8_t Time_100ms: 1;		//100ms ��־
	uint8_t Time_500ms: 1;		//500ms ��־
	uint8_t Time_2s: 1;
	uint8_t Time_5s: 1;
	uint8_t Time_alarm_fg: 1;		//˾��������������־
	uint8_t flash_floor_500ms: 1;
}bTime;

struct{
	uint8_t buz_set_id:	1;
	uint8_t buz_state:	1;
	uint8_t group_floor_lock:	1;
	uint8_t group_floor_lock_old:	1;
	uint8_t uart_send_busy: 1;
	uint8_t hsecheck: 1;
	uint8_t disp_flash: 1;
}bFunc;

uint8_t						merker = 0;
uint8_t						errorregister = 0;				/* error register						*/
uint16_t						errorcode = 0;					/* error code							*/
uint8_t						virt_in   [MAX_IO_TYPE];					/* virtual input (last changed)			*/
uint8_t						virt_out  [MAX_IO_TYPE];					/* virtual output (last changed)		*/
//�����������
uint8_t 	volatile	outpush[MAX_IN_BYTE];						/* push button indication				*/
uint8_t	pos = 0;

uint8_t						out_polarity[MAX_IN_BYTE];					/* output is inverted / not inverted	*/

#else
extern	uint8_t	merker;
extern	uint8_t	hse_heartbeat;
extern	uint8_t	bBusOffTimer;

extern	uint8_t	volatile	input[3][MAX_IN_BYTE];						/* input variables for debouncing		*/
extern	uint8_t 	volatile	in[MAX_IN_BYTE];								/* variable for input after debouncing	*/
extern	uint8_t						inold[MAX_IN_BYTE];							/* variable to save old input state		*/
extern	uint8_t						virt_in   [MAX_IO_TYPE];					/* virtual input (last changed)			*/
extern	uint8_t						virt_out  [MAX_IO_TYPE];					/* virtual output (last changed)		*/
extern	uint8_t 	volatile	out[MAX_IN_BYTE];							/* output state							*/
extern	uint8_t 	volatile	outpush[];						/* push button indication				*/
extern	uint8_t						in_polarity[MAX_IN_BYTE];					/* input is inverted / not inverted		*/
extern	uint8_t						out_polarity[MAX_IN_BYTE];					/* output is inverted / not inverted	*/
extern	uint8_t						buzzer;						/* buzzer state							*/
extern	uint8_t						inpar  [MAX_IN][MAX_IO_TYPE];					/* input parameter						*/
extern	uint8_t						outpar [MAX_OUT][MAX_IO_TYPE + 1];					/* output parameter						*/
extern	uint8_t 						node_id;
extern	uint8_t						disp_lift;						/* lift number assigned to display		*/
extern	uint8_t						nmtstate;						/* network management state 			*/
extern	uint8_t	volatile	nmtwait;						/* waiting time for 1. heartbeat		*/
extern	uint16_t	volatile	sdo_index ;					/* index for SDO segment transfer		*/
extern	uint8_t	volatile	sdo_subindex;				/* index for SDO segment transfer		*/
extern	uint8_t						sdo_timer;					/* timer for SDO segment transfer		*/
extern	uint8_t						errorregister ;				/* error register						*/
extern	uint16_t						errorcode ;					/* error code							*/
extern	uint8_t 	volatile		hsetime;				// HSE supervising HSE

extern	uint8_t	heartbeat;						/* timer for transmitting heartbeat		*/
extern	uint8_t 	mInOut_Number;			//�����������
extern	uint8_t	mArrow_State;
extern	uint8_t	mMax_InByte;
extern	uint8_t 	mExtern_Number;			//��չ������

extern	struct {
	uint8_t Time_10ms: 1;		//10ms ��־
	uint8_t Time_100ms: 1;		//100ms ��־
	uint8_t Time_500ms: 1;		//500ms ��־
	uint8_t Time_2s: 1;
	uint8_t Time_5s: 1;
	uint8_t Time_alarm_fg: 1;		//˾��������������־
	uint8_t flash_floor_500ms: 1;
}bTime;

extern	struct{
	uint8_t buz_set_id:	1;
	uint8_t buz_state:	1;
	uint8_t group_floor_lock:	1;
	uint8_t group_floor_lock_old:	1;
	uint8_t uart_send_busy: 1;
	uint8_t hsecheck: 1;
	uint8_t disp_flash: 1;
}bFunc;

extern uint8_t pos;
extern uint8_t	att_alarm_timer;
extern uint8_t	buz_alarm_timer;
extern uint8_t	buz_alarm_totaltimer;
extern char FloorName[TOTAL_FLOOR][2];
extern void ClrWdt (void);
#endif
#endif

