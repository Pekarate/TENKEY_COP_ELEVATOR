
#define	_SUBPROG_C_
#include	"AllHeader.h"

extern SPI_HandleTypeDef hspi1;
uint8_t Get_NodeID(void){
	uint8_t i, dat1 = 0;
	for(i=0; i<10; i++)
		{
			if(HAL_GPIO_ReadPin(NODE_ID_GPIO_Port,NODE_ID_Pin))
				dat1++;
			__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();
		}
	if(dat1 >= 7)
		i =  0x10;
	else
		i =  0x11;

	return i;
}


uint8_t Check_InChange(uint8_t* buf1, uint8_t* buf2){
	uint8_t i;

	for(i=0; i<mMax_InByte; i++)
		{
			if(buf1[i] != buf2[i])
				return 1;
		}
	return 0;
}


void Init_SPI(void){

}

uint8_t SPI_SendOneByte(uint8_t dat){
	HAL_SPI_Transmit(&hspi1, &dat, 1, 100);
//	SSPBUF = dat;
//	while(!(SSPSTAT & 0x01))
//		ClrWdt();
//	dat = SSPBUF;

	return dat;
}


/*===============================         ��ȡ���еİ�ť����             =====================================
 1�������ж����ް�ť��( mExtern_Number=0:  �ް�ť�壬ֻ��ֱ��I/0���������������
 														>0:  ���ڰ�ť�壬������166��ʱ�����(1�鰴ť��16���������)
 2��һ��SPI����: ����9���ֽڣ�һ��Ƭѡ��Ч(Ƭѡÿ��ѭ���ƶ�һ�ֽ�)���ܹ�9��ѭ��
 3��9��ѭ����ϣ�����һ��ѭ���ر���������Ƭѡ
============================================================================================================*/
uint8_t 	volatile	Userin[MAX_IN_BYTE];								/* variable for input after debouncing	*/
void ReadInput(void){
//	memcpy((void *)in,(void *)Userin,MAX_IN_BYTE);
//	return ;
	uint8_t i, j, temp;
	uint32_t kCode[3];

//���ȶ�ȡ��ť�������(һ��22��)
//	kCode[0] = ((PORTA & 0x0F) |						// PA0~PA3
//			(((uint32_t)PORTE & 0x07) << 4) |			// PE0~PE2
//			((uint32_t)PORTD << 7) |					// PD0~PD7
//			(((uint32_t)PORTB & 0x03) << 15) |			// PB0~PB1
//			(((uint32_t)PORTB & 0x30) << 13) |			// PB4~PB5
//			(((uint32_t)PORTC & 0x07) << 19));			// PC0~PC2
	kCode[0] = ((GPIOB->IDR) & 0x3FFF);


	if(mExtern_Number)
		{//������չ��
			kCode[1] = 0x00;
			kCode[2] = 0x00;
			//SSPCON1 = 0x00; __NOP();__NOP();__NOP();__NOP();
			HC166_PARA_EN(); __NOP();__NOP();__NOP();__NOP();
			HC166_CLK_LOW();__NOP();__NOP();__NOP();__NOP();
			HC166_CLK_HIGH();__NOP();__NOP();__NOP();__NOP();
			HC166_CLK_LOW();__NOP();__NOP();__NOP();__NOP();
			HC166_SERI_EN(); __NOP();__NOP();__NOP();__NOP();
			HC595_LUCK_DIS();

			for (i=0; i<mExtern_Number; i++)
				{
					temp = 22 + (i + 1) * 16 - 1;
					for(j=0; j<16; j++)
						{
//#warning   please modify here!
//							if (!(PORTC & BIT_4))
								kCode[temp / 32] |= (0x01ul << (temp % 32));
							HC166_CLK_HIGH();__NOP();__NOP();__NOP();__NOP();
							HC166_CLK_LOW();__NOP();__NOP();
							--temp;
						}
				}
			__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();
//			SSPCON1 = 0x32;
		}

	for (i=0; i<MAX_IN_BYTE; ++i)
		{	
			input[2][i] = input[1][i];
			input[1][i] = input[0][i];
		}
	input[0][0] = (uint8_t)kCode[0];
	input[0][1] = (uint8_t)(kCode[0] >> 8);
	input[0][2] = (uint8_t)(kCode[0] >> 16);
	j = 3;

	switch (mExtern_Number)
		{
			case 1:
				input[0][3] = (uint8_t)(kCode[0] >> 24);
				input[0][4] = (uint8_t)kCode[1];
				j = 5;
				break;
				
			case 2:
				input[0][3] = (uint8_t)(kCode[0] >> 24);
				input[0][4] = (uint8_t)kCode[1];
				input[0][5] = (uint8_t)(kCode[1] >> 8);
				input[0][6] = (uint8_t)(kCode[1] >> 16);
				j = 7;
				break;
				
			case 3:
				input[0][3] = (uint8_t)(kCode[0] >> 24);
				input[0][4] = (uint8_t)kCode[1];
				input[0][5] = (uint8_t)(kCode[1] >> 8);
				input[0][6] = (uint8_t)(kCode[1] >> 16);
				input[0][7] = (uint8_t)(kCode[1] >> 24);
				input[0][8] = (uint8_t)kCode[2];
				j = 9;
				break;				
		}
	for (i=0; i<j; ++i)
		{	
			if (input[2][i] == input[1][i] && input[1][i] == input[0][i])
				in[i] = input[2][i];
		}
}


/*========================         ����źŴ���          ===================================
	1������ź�: ֻ����70����ť�ƣ�������2����վ�ӣ�1�����ط���������������ĳ�����
	2���ߵ�ƽ����
==============================================================================================*/
void Out_Prog(void){
	uint8_t i, j, temp;
	uint16_t shift = 0;
	uint8_t out_state[MAX_IN_BYTE];
	static uint8_t out_old[MAX_IN_BYTE] = {0xFF,0xFF,0xFF,0,0,0,0,0,0};

	for(i=0; i<mMax_InByte; i++)
		out_state[i] = out[i] ^ out_polarity[i];		// read input state; invert if desired
	if(Check_InChange(out_state, out_old) == 0)	return;

	HC595_LUCK_DIS();

	if( mExtern_Number )
		{//������չ��
			for(i=mExtern_Number; i>0; i--)
				{
					shift = (((uint16_t)out_state[i * 2 + 2] << 8) | out_state[i * 2 + 1]) << 2;
					temp = out_state[i * 2] & 0xC0; 	//ȡ����2λ
					shift |= (temp >> 6);
					out_state[i * 2 + 2] = (uint8_t)(shift >> 8);
					out_state[i * 2 + 1] = ((uint8_t)shift);
				}
			for(i = mExtern_Number * 2 + 3; i > 0; i--)
				{
					temp = 0;
					for(j=0; j<8; j++)
						{
							temp <<= 1;
							if(out_state[i-1] & (1 << j))
								temp |= 0x01;
						}
					SPI_SendOneByte(temp);
				}
			HC595_LUCK_EN();
			__NOP();__NOP();
			HC595_LUCK_DIS();
		}
	else
		{//û����չ��
			for(i=3; i>0; i--)
				{
					temp = 0;
					for(j=0; j<8; j++)
						{
							temp <<= 1;
							if(out_state[i-1] & (1 << j))
								temp |= 0x01;
						}
					SPI_SendOneByte(temp);
				}
			HC595_LUCK_EN();
			__NOP();__NOP();
			HC595_LUCK_DIS();
		}
	for(i=0; i<mMax_InByte; i++)
		out_old[i] = out_state[i];
}

void test_mode(void){
	uint8_t i, j;
	uint8_t help;
	uint8_t instate[MAX_IN_BYTE];
	uint8_t testno = 0;
	uint8_t pBuf[5];

	for(i=0; i<MAX_IN_BYTE; i++)
	{
		in_polarity[i] = 0;
		out_polarity[i] = 0;
		outpush[i] = 0;
		instate[i] = 0;
	}

	for (i = 1; i < MAX_IO_TYPE; i++)
  	virt_in  [i] = 0;
	virt_in  [IO_BASIC_FUNC] = TEST_IO;
	heartbeat = 200;

//	RCSTA				= 0x00;					//enable serial port
//	TRISCbits.TRISC6	= 0;			//RC6 is output
//	TRISCbits.TRISC7	= 0;			//RC7 is output
//	LATCbits.LATC6 = 0;
//	LATCbits.LATC7 = 0;

	//����ģʽĬ��Ϊ3����չ��
	mExtern_Number = 3;
	mInOut_Number = (mExtern_Number*16 + 22) % (MAX_IN + 1);
	mMax_InByte = (mInOut_Number >> 3) + 1;

	while (1)
		{
			if (!heartbeat)
				{
//					TXB1D0    = nmtstate;							/* write data uint8_t for HEARTBEAT		*/
//					TXB1CONbits.TXREQ = 1;							// transmit request
					heartbeat = 200;
				}
			ClrWdt();

			if ( Check_InChange(instate, (uint8_t *)in) )					// input state changed
				{
					for(i=0; i<mInOut_Number; i++)
						{
							help = bit_select (instate[i/8], i%8);
							if (help != bit_select (in[i/8], i%8))
								{// input has changed
									virt_in[IO_BASIC_FUNC] = CAR_CALL;
									virt_in[IO_SUB_FUNC] = i + 1;
									virt_in[IO_LIFT] = LIFT1;
									virt_in[IO_STATE] = help;

									if (help)
										{
											transmit_in (virt_in);
											bit_reset(out[i/8], i % 8);
										}
									else
										bit_set(out[i/8], i % 8);
								}
						}

					for(i=0; i<mMax_InByte; i++)
						instate[i] = in[i];
				}
			if (rc)
				{
					if (rx [ro][0] == PDO_OUT)
						{
							if(rx [ro][2] == CAR_CALL)
								{
									for (i = 0; i < MAX_IO_TYPE; i++) 				// read output function
										virt_out [i] = rx[ro][i + 2]; // write to virtual output mapping
									j = virt_out[IO_SUB_FUNC] - 1;

									// virtual output matches with physical
									if (virt_out[IO_STATE] & 0x01)
										{// set acknowledgement
											bit_set (out[j/8], j%8);				// set physical output
										}
									else
										{// reset acknowledgement
											bit_reset (out[j/8], j%8);				// clear physical output
										}
								}
							else if(rx [ro][2] == ARRIVAL_INDICATION)
								{//��վ��
									if(rx [ro][IO_STATE + 2])
										{
											bit_set (out[2], 6);				// set physical output
											bit_set (out[2], 7);				// set physical output
//											LATCbits.LATC6 = 1;
//											LATCbits.LATC7 = 1;
										}
									else
										{
											bit_reset (out[2], 6);				// set physical output
											bit_reset (out[2], 7);				// set physical output
//											LATCbits.LATC6 = 0;
//											LATCbits.LATC7 = 0;
										}
								}
							else if((rx [ro][2] == SPECIAL_FUNC) && (rx [ro][3] == BUZZER))
								{
									if(rx [ro][IO_STATE + 2])
										buzzer |= BUZ_WORKING;
									else
										buzzer &= ~BUZ_WORKING;
								}
						}
					else if ((rx [ro][0] == NMT) && (rx [ro][2] == RESET_NODE))
						{
							merker = 0;
							HAL_NVIC_SystemReset();
							//Reset();
						}
					if (ro == (RX_SIZE-1))						/* increment RX message read pointer	*/
						ro = 0;
					else
						ro++;
					rc--;										/* decrement RX counter					*/
				}

			if(bTime.Time_10ms)
				{
					ReadInput();		//��ȡ���еİ�ť����
					Out_Prog(); 					//�������е����(������ʾ���)
					bTime.Time_10ms = 0;
					if((++testno % 10) == 0)
						{
							pBuf[0] = 0xA0;
							pBuf[1]++;
						}
				}
		}
}

