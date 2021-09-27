
#define	_INIT_C_
#include	"AllHeader.h"

void Init_Ports (void){

//	ADCON1 = 0x0F;				// PortA, PortE����AD����
//
//	TRISA = 0x0F; 			// RA0-RA3 Input, ,RA4,RA5 output
//		//RA0: IN1
//		//RA1: IN2
//		//RA2: IN3
//		//RA3: IN4
//		//RA4: 595-RCLK  595�������ź����
//		//RA5: PE, 166���ź�
//	LATA = 0;
//
//	PORTB = 0x0F; 		// switch off (important for CAN!)
//	TRISB = 0x7B; 		//RB2,RB7: Output, others Inout
//		//RB0: IN16
//		//RB1: IN17
//		//RB2: CANTX  ���
//		//RB3: CANRX  ����
//		//RB4: IN18
//		//RB5: IN19
//		//RB6: PGC(ID-SET ID��ַ  ����, ����)
//		//RB7: PGD(���ر������)
//	LATB = 0;
//
//	TRISC	= 0x97;
//		//RC0: IN20
//		//RC1: IN21
//		//RC2: IN22
//		//RC3: SPI-SCK    SPIʱ�����
//		//RC4: SPI-SDI    ����
//		//RC5: SPI-SDO    SPI�������
//		//RC6: TX         �������
//		//RC7:	RX         ��������
//	LATC = 0;
//
//	TRISD	= 0xFF; 		//IN8-IN15
//	CMCON= 0x07; 			// comparator on Port D pins 0 - 3 off
//	PORTD = 0xFF;
//
//	TRISE	= 0x07;
//		//RE0: IN5
//		//RE1: IN6
//		//RE2: IN7
////	Close_RunLed();

}

void Init_Timer0 (void){//500ms
//	INTCONbits.TMR0IE = 1;						//ʹ��T0�ж�
//	INTCONbits.TMR0IF = 0;
//	INTCON2bits.TMR0IP = 0;
//	TMR0H = T0H_500ms;
//	TMR0L = T0L_500ms;
//	T0CON = T0START_1TO128;
}

void Init_Timer1 (void){//10ms
//	PIR1bits.TMR1IF		= 0;		//clear Timer 1 interrupt flag
//	IPR1bits.TMR1IP		= 0;		//Timer 1 low interrupt priority
//	PIE1bits.TMR1IE		= 1;		//enable Timer 1 interrupt
//	TMR1H	= T1H_10ms;				//set timer value (must be set first!)
//	TMR1L	= T1L_10ms;				//set timer value (1 ms)
//	T1CON = T1START_1TO4;			//Timer 1 on, 16 Bit, no prescaler
}

//��ʼ������
void Init_UART (void){//��������ͬ������ģʽ
//	TRISCbits.TRISC6	= 0;			//RC6 is clock output
//	TRISCbits.TRISC7	= 1;			//RC7 is data line
//
//	RCSTA				= 0x80;					//enable serial port
//	TXSTA				= 0xB0;					//set synchronous master mode	,ͬ�����ط���, 8λ����
//	RCSTA				= 0x80;					//enable serial port, ��ֹ����
//
//	BAUDCONbits.BRG16 = 1;
//	SPBRGH = 0x04;								// baud rate 9600bps
//	SPBRG = 0x10;								// ע�⣺�����ʵ���1200���ܵ��·��ͻ���������
//
//	IPR1bits.TXIP		= 0;				//TX interrupt priority
//	PIE1bits.TXIE		= 0;				//enable TX interrupt

}

