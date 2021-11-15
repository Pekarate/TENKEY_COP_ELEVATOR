
#ifndef	_SUBPROG_H_
#define	_SUBPROG_H_

//���峣��
#include "main.h"

#define	HC166_PARA_EN()			HC166_SEl =0	//HAL_GPIO_WritePin(HC166_SEL_GPIO_Port, HC166_SEL_Pin, GPIO_PIN_RESET)	//LATAbits.LATA5 = 0
#define	HC166_SERI_EN()			HC166_SEl =1	//HAL_GPIO_WritePin(HC166_SEL_GPIO_Port, HC166_SEL_Pin, GPIO_PIN_SET)		//LATAbits.LATA5 = 1
#define	HC166_CLK_HIGH()		SCK = 1			//HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_SET)					//LATCbits.LATC3 = 1
#define	HC166_CLK_LOW()			SCK = 0			//HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_RESET)				//LATCbits.LATC3 = 0
#define	HC595_LUCK_EN()			HC959_SEl =1	//HAL_GPIO_WritePin(HC595_SEL_GPIO_Port, HC595_SEL_Pin, GPIO_PIN_SET)	//			//LATAbits.LATA4 = 1
#define	HC595_LUCK_DIS()		HC959_SEl =0	//HAL_GPIO_WritePin(HC595_SEL_GPIO_Port, HC595_SEL_Pin, GPIO_PIN_RESET)	//LATAbits.LATA4 = 0
uint8_t SPI_SendOneByte(uint8_t dat);
#ifdef	_SUBPROG_C_

uint8_t Get_NodeID(void);
uint8_t Check_InChange(uint8_t* buf1, uint8_t* buf2);
void Out_Prog(void);
void ReadInput(void);

void test_mode(void);

#else
extern	uint8_t Get_NodeID(void);
extern	uint8_t Check_InChange(uint8_t* buf1, uint8_t* buf2);
extern	void Out_Prog(void);
extern	void ReadInput(void);

extern	void test_mode(void);
extern	void check_key_buttom(void);
extern	void key_buttom_output(void);
extern	void peak_led_out(void);

#endif

#endif

