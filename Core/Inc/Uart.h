
#ifndef _DISPLAY_H_
#define	_DISPLAY_H_

//���峣��
#define	NO_ARROW		0
#define	MES_OVERLOAD	BIT_2
#define	MES_FIRE		BIT_3

#define	FULLLOAD				BIT_7
#define	OVER_LOAD			BIT_4
#define	FIRE_CASE				BIT_5
#define	OUT_OF_SERVICE	BIT_6
#define	IN_USE					BIT_7

#define	BUF_TEN				0
#define	BUF_UNIT				1
#define	BUF_ARROW			2
#define	BUF_MESSAGE		3

#define	A_BETR			38
#define	UART_BUFF_SIZE	1024

#ifdef _DISPLAY_C_
//���庯���ͱ���

//#pragma udata gpr0
uint8_t	volatile		display [4];
	//[0]: ten
	//[1]: unit
	//[2]: Arrow+Message(	bit0: up
		//								bit1: down
		//								bit2: up_move
		//								bit3: down_move
		//								bit4: overload
		//								bit5: firecase
		//								bit6: outof service
		//								bit7: In used
	//[3]: Message:       bit0-bit6: Not used
	//									bit7: Full load

uint8_t mDisp_buf[5];

//uint8_t uart_send_busy = 0;
uint8_t uart_send_to = 0;
uint8_t uart_send_ro = 0;
uint8_t uart_send_len = 0;

uint8_t uart_send_buff[UART_BUFF_SIZE];
//#pragma udata

uint8_t const tFloor_Tab[] = {
	0x00, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
	 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
   'W', 'X', 'Y', 'Z', '+', '-', 0x01,
};

void Display_device(void);
void Arrow_Status(void);
void Display_TestMode(void);

#else
extern	uint8_t	volatile		display [];
extern	uint8_t mDisp_buf[5];

//extern	uint8_t uart_send_busy;
extern	uint8_t uart_send_to;
extern	uint8_t uart_send_ro;
extern	uint8_t uart_send_len;
extern	uint8_t uart_send_buff[];

extern	void Init_SPI(void);
extern	uint8_t SPI_SendOneuint8_t(uint8_t dat);
extern	void Display_device(void);
extern	void Arrow_Status(void);
extern	void Display_TestMode(void);

#endif
#endif

