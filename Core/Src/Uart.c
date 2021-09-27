
#define	_DISPLAY_C_
#include	"AllHeader.h"


void UART_SendBuf(uint8_t* pBuf, uint8_t len){
//	uint8_t i;
//
//	for(i=0; i<len; i++)
//	{
//		uart_send_buff[uart_send_to++] = pBuf[i];
//		if (uart_send_to == UART_BUFF_SIZE)
//			uart_send_to = 0;
//	}
//	INTCONbits.GIEL	= 0;
//	uart_send_len += len;
//	if (!bFunc.uart_send_busy)
//	{
//		TXREG = uart_send_buff[uart_send_ro++];
//		if (uart_send_ro == UART_BUFF_SIZE)
//			uart_send_ro = 0;
//		-- uart_send_len;
//		bFunc.uart_send_busy = true;
//	}
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

