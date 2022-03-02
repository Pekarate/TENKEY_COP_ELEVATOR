
#define	_DISPLAY_C_
#include	"AllHeader.h"


uint8_t usart2_buf[100];
uint8_t usart2_len =0;
extern TIM_HandleTypeDef htim3;
static inline void delay_us(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim3,0);
	while(htim3.Instance->CNT <us);
}
void SOFT_USART_SEND_BYTE(uint8_t dt)
{

	for(uint8_t i=0;i<8;i++)
	{
		PIN_A2 = (dt>>i)&0x01;
		USART2_CLK_HIGH;

		delay_us(50);
		USART2_CLK_LOW;
		delay_us(50);
	}
}
void SOFT_USART_SEND_BYTES(uint8_t* pBuf, uint8_t len)
{

	for(uint8_t i=0;i<len;i++)
	{
		SOFT_USART_SEND_BYTE(*(pBuf+i));
	}
}
//void HAL_USART_TxCpltCallback(USART_HandleTypeDef *husart)
//{
//	if (husart->Instance == husart2.Instance)
//	{
////					PIR1bits.TXIF = 0;
//					if (uart_send_len)
//						{
//							memcpy(usart2_buf,uart_send_buff,uart_send_len);
//							usart2_len = uart_send_len;
//							HAL_USART_Transmit_IT(&husart2, usart2_buf, usart2_len);
//							uart_send_len =0;
//							bFunc.uart_send_busy = true;
//						}
//					else
//						{
//							bFunc.uart_send_busy = false;
//						}
//	}
//}

void UART_SendBuf(uint8_t* pBuf, uint8_t len){
	uint8_t i;

	for(i=0; i<len; i++)
	{
		uart_send_buff[uart_send_to++] = pBuf[i];
		if (uart_send_to == UART_BUFF_SIZE)
			uart_send_to = 0;
	}
//	//INTCONbits.GIEL	= 0;
//	CLEAR_BIT(husart2.Instance->CR1, USART_CR1_TXEIE);
//	uart_send_len += len;
//	if (!bFunc.uart_send_busy)
//	{
//		//TXREG = uart_send_buff[uart_send_ro++];
//		if(uart_send_len)
//		{
//			memcpy(usart2_buf,uart_send_buff,uart_send_len);
//			usart2_len = uart_send_len;
//			HAL_USART_Transmit_IT(&husart2, usart2_buf, usart2_len);
//			uart_send_len =0;
//			bFunc.uart_send_busy = true;
//		}
//	}
//	SET_BIT(husart2.Instance->CR1, USART_CR1_TXEIE);
//	PIE1bits.TXIE		= 1;
//	INTCONbits.GIEL	= 1;
}

void Display_device(void){
	uint8_t i, checksum;

	checksum = 0;
	mDisp_buf[0] = 0xB0;	
	for (i=0; i<4; ++i)
		checksum += mDisp_buf[i];
	mDisp_buf[4] = checksum;
	UART_SendBuf(mDisp_buf, 5);
}

void Arrow_Status(void){
	uint8_t buf[5], checksum = 0, i;

	buf[0] = 0xA0;
	buf[1] = display[BUF_MESSAGE];
	buf[2] = 0;
	buf[3] = mArrow_State % 3;
	for(i=0; i<4; i++)
		checksum += buf[i];
	buf[4] = checksum;
	UART_SendBuf(buf, 5);
}

void Display_TestMode(void){
	static uint8_t dat_cnt = 0;
	dat_cnt++;

	dat_cnt %= 128;
	mDisp_buf[0] = 0xB0;

	if(dat_cnt < 128)
		{//ֻ��ʾ����
			if(dat_cnt < 64)
				{///1-64
					if((dat_cnt % 3) == 0)
						mDisp_buf[BUF_ARROW + 1] = NO_ARROW;
					else
						mDisp_buf[BUF_ARROW + 1] = ARROW_UP;
					mDisp_buf[BUF_TEN + 1] = tFloor_Tab[(dat_cnt - 14) / 10 + 1];
					mDisp_buf[BUF_UNIT + 1] = tFloor_Tab[((dat_cnt - 14) % 10) + 1];
				}
			else
				{///64-1
					if((dat_cnt % 3) == 0)
						mDisp_buf[BUF_ARROW + 1] = NO_ARROW;
					else
						mDisp_buf[BUF_ARROW + 1] = ARROW_DOWN;// | (1 << ((dat_cnt%3)+1));
					mDisp_buf[BUF_TEN + 1] = tFloor_Tab[(64 - (dat_cnt - 64)) / 10 + 1];
					mDisp_buf[BUF_UNIT + 1] = tFloor_Tab[((64 - (dat_cnt - 64)) % 10) + 1];
				}
		}
	else if(dat_cnt < 180)
		{
			if(dat_cnt < 154)
				{//A1-Z9
					if((dat_cnt % 3) == 0)
						mDisp_buf[BUF_ARROW + 1] = NO_ARROW;
					else
						mDisp_buf[BUF_ARROW + 1] = ARROW_UP;// | (1 << ((dat_cnt%3)+1));

					mDisp_buf[BUF_TEN + 1] = tFloor_Tab[(dat_cnt - 128) / 26 + 11];
					mDisp_buf[BUF_UNIT + 1] = tFloor_Tab[((dat_cnt - 128) % 10) + 1];
				}
			else
				{//Z9-A1
					if((dat_cnt % 3) == 0)
						mDisp_buf[BUF_ARROW + 1] = NO_ARROW;
					else
						mDisp_buf[BUF_ARROW + 1] = ARROW_DOWN;

					mDisp_buf[BUF_TEN + 1] = tFloor_Tab[(26 - (dat_cnt - 154)) / 26 + 11];
					mDisp_buf[BUF_UNIT + 1] = tFloor_Tab[((26 - (dat_cnt - 154)) % 10) + 1];
				}
		}
	else
		dat_cnt = 0;
}

