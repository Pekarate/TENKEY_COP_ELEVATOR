//
//#define	_MAIN_C_
//#include	"AllHeader.h"
//
////#define	D_DEBUG
//
//// Set configuration bits
//#pragma config OSC = HSPLL		// Main oscillator high speed resonator
//#ifdef D_DEBUG
//#pragma config WDT = OFF			// Watchdog timer off
//#pragma config DEBUG = ON			// enable debug mode
//#pragma config CP0 = OFF
//#pragma config CP1 = OFF
//#else
//#pragma config WDT = ON				//Watchdog timer off
//#pragma config DEBUG = OFF		// disable debug mode
//#pragma config CP0 = ON
//#pragma config CP1 = ON
//#endif
//#pragma config BOREN = BOHW		// Brown out reset enabled
//#pragma config BORV = 2				// Brown out voltage 2.8V
//#pragma config WDTPS = 4096		// watchdog postscaler 1 : 1024
//#pragma config LVP = OFF			// Low voltage programming off
//
////check if a HSE heartbeat time out occured
//// If so, set or reset outputs and displays for this Lift
//void check_hse (BYTE mode){
//	BYTE help;
//	BYTE i;
//
//	help = 0;
//	if (!hsetime)								/* 5s no heartbeat from HSE				*/
//		{
//			help = 1;
//			hsetime  = HSETIME;						/* restart HSE heartbeat timer			*/
//			display[BUF_TEN] = 'N';			//A_BETR;
//			display[BUF_UNIT] = 'C';		//A_BETR;
//			display[BUF_ARROW] = 0;
//			display[BUF_MESSAGE] = 0;
//			hse_heartbeat = 0;
//			mDisp_buf[BUF_TEN + 1] = display[BUF_TEN];
//			mDisp_buf[BUF_UNIT + 1] = display[BUF_UNIT];
//			mDisp_buf[BUF_ARROW + 1] = display[BUF_ARROW];
//		}
//
//	if (help && mode)											/* one or more HSE not available		*/
//		{
//			for (i = 0; i < mInOut_Number; i++)					/* check all call indications			*/
//				{
//					if (check_for_call (outpar [i][IO_BASIC_FUNC]))
//						{											/* output is call acknowledgement		*/
//				  		outpar [i][IO_ACK] &= ~help;
//							if (!outpar [i][IO_ACK])				/* all acknowledgements cancelled		*/
//								{
//									bit_reset (out[i/8], i%8);					/* clear output							*/
//									outpar [i][IO_STATE] = 0;
//								}
//						}
//					else										/* all other output functions			*/
//						{
//							if (outpar [i][IO_LIFT] & help)			/* output for this lift					*/
//								{
//									if ((outpar [i][IO_BASIC_FUNC] == SPECIAL_FUNC) &&
//									    (outpar [i][IO_SUB_FUNC]	  == OUT_OF_ORDER))
//										{
//											bit_set (out[i/8], i%8);				/* set physical output					*/
//					 						outpar [i][IO_STATE] = 1;
//										}
//									else
//										{
//											bit_reset (out[i/8], i%8);				/* reset physical output				*/
//											outpar [i][IO_STATE] = 0;
//										}
//								}
//						}
//				}
//		}
//}
//#define MAXLONGVALUE 2000000000;
//#define MAXKEYBUFF          4
//#define KEYTIMOUT            1500				//ms
//long Keytimout = MAXLONGVALUE;
//
//void main_sub(void){
//	BYTE i, j;
//	BYTE instate[MAX_IN_BYTE];
//	BYTE instate_Pre[MAX_IN_BYTE];
//	DWORD inold_Pre;
//	BYTE help;
//	BYTE virt_key[3];
//	BYTE virt_key_cnt =0;
//	if (merker == RC_MERKER)									// restart by after Rx counter error
//		{
//			errorregister |= ER_COMMUNICATION;						// set error bits
//		}
//	else if (RCONbits.POR)										// restart not by normal power on
//		{
//			if (!RCONbits.TO)										// restart by watchdog timer
//				{
//					errorregister |= ER_GENERIC;			// set error bits
//					errorcode = E_INTERNAL_SW;				// set error code
//				}
//		}
//	RCONbits.POR = 1;
//	RCONbits.BOR = 1;
//	merker = WD_MERKER;						// set variable for restart
//	ClrWdt();												// reset watchdog timer
//
//	Init_Ports();			//��ʼ��I/O����
//	Init_SPI ();					// init serial synchronous interface
//
//	nmtstate  	= BOOT_UP;		// set CAN NMT state to Boot up
//	nmtwait = 2;	// waiting time to 1. heartbeat in INI-Mode waiting time	1s
//
//	//��ʼ���ر����а�ť��
//	HC595_LUCK_DIS();
//	for(i = 0; i < MAX_IN_BYTE; i++)
//		SPI_SendOneByte(0x00);
//
//	HC595_LUCK_EN();
//	Nop();
//	HC595_LUCK_DIS();
//
//	hsetime = HSETIME;
//	for(i = 0; i < MAX_IN_BYTE; i++)
//		{
//			out[i] = 0;
//			in[i] = 0x00;
//			input[0][i] = 0x00;
//			input[1][i] = 0x00;
//			input[2][i] = 0x00;
//			inold[i] = 0x00;
//			instate[i] = 0;
//			in_polarity[i] = 0;
//			out_polarity[i] = 0;
//		}
//	for (i = 0; i < MAX_IN; i++)
//		{
//			inpar  [i][IO_STATE] = 0;
//			outpar [i][IO_STATE] = 0;
//			inpar  [i][IO_BASIC_FUNC] = 0;
//			outpar [i][IO_BASIC_FUNC] = 0;
//			outpar [i][IO_ACK] = 0;
//		}
////�����ʾ������
//	for(i = 0; i < 4; i++)
//		display[i] = 0;
//	for(i = 0; i < 5; i++)
//		mDisp_buf[i] = 0;
//
//	mArrow_State = 0;
//	mExtern_Number = 0;
//	INTCON = 0;						// global interrupts disable
//	RCONbits.IPEN = 1;		// Enable interrupt priority levels
//
//	Init_Timer0 ();				// initialize timer 0
//	Init_Timer1 ();				// initialize timer 1
//	Init_UART();					//��ʼ������
//	ClrWdt();							// reset watchdog timer
//	INTCONbits.GIEH = 1;	// global interrupts enable
//	INTCONbits.GIEL = 1;	// low priority interrupts enable
//	while (nmtwait)				// waiting time to 1. heartbeat
//		{
//			ClrWdt();
//			if(bTime.Time_10ms)
//				{
//					ReadInput();
//					bTime.Time_10ms = 0;
//				}
//		}
//	node_id = Get_NodeID();
//
////���ò�����
//	i = 0;
//	baudrate = BPS_125K;
//	Init_Can (baudrate);	// initialize Onchip CAN
//	ClrWdt ();
//
//	heartbeat = HEARTBEAT_TIME;
//	TXB1CONbits.TXREQ = 1;				// ��������
//	nmtstate = PRE_OP;						// set state pre-operational
//	disp_lift = LIFT1;
//
//	while (nmtstate == PRE_OP)
//	{//�ȴ������������ָ��
//		ClrWdt ();
//		if (rc)					// Message in receive buffer
//			read_rx ();		// read and handle message
//		if (sdo_index && !sdo_timer)
//			{// SDO segment transfer time out
//				sdo_index = 0;
//				abort_sdo (SDO_TIMEOUT);						//send SDO abort request
//			}
//
//		if (bFunc.hsecheck)
//			{// HSE heartbeat check necessary
//				bFunc.hsecheck = false;
//				check_hse (0);									// check if a HSE is not available
//			}
//		if ((!heartbeat) && (hse_heartbeat) && (!bBusOffTimer))	//time to send heartbeat message
//			{
//				heartbeat = HEARTBEAT_TIME;
//				TXB1D0    = nmtstate;							//write data byte for HEARTBEAT
//				TXB1CONbits.TXREQ = 1;							//transmit request
//			}
//
//		if (errorcode)
//			{// error occured
//				transmit_error ();	// send emergency message
//				errorregister = 0;	// reset error
//				errorcode = 0;
//			}
//		if ((merker == BS_MERKER) && (!bBusOffTimer))
//			{
//				Init_Can(baudrate);
//				errorregister |= ER_COMMUNICATION;			// set error bits
//				errorcode = E_BUS_OFF_A;									// set error code
//				merker = 0;
//			}
//		if(nmtstate == TEST_MODE)
//			{
//				test_mode();
//			}
//	}
//
//	hsetime = HSETIME ;		//start timer with different times
//	bFunc.hsecheck = false;					//no HSE check now
//	ClrWdt();							//reset watchdog timer
//
//	mInOut_Number = (mExtern_Number * 16 + 22) % (MAX_IN + 1);
//	mMax_InByte = (mInOut_Number >> 3) + 1;
//
//	set_io_config ();
//	Arrow_Status();
//
//	while (1)
//		{
//			ClrWdt ();											// reset watchdog timer
//			if (rc)													// Message in receive buffer
//				read_rx ();										// read and handle message
//			if (errorcode)
//				{// error occured
//					transmit_error ();					// send emergency message
//					errorregister = 0;					// reset error
//					errorcode = 0;
//				}
//			if ((merker == BS_MERKER) && (!bBusOffTimer))
//				{
//					Init_Can(baudrate);
//					errorregister |= ER_COMMUNICATION;			// set error bits
//					errorcode = E_BUS_OFF_A;									// set error code
//					merker = 0;
//				}
//
//			if (sdo_index && !sdo_timer)
//				{// SDO segment transfer time out
//					sdo_index = 0;
//					abort_sdo (SDO_TIMEOUT);		// send SDO abort request
//				}
//			if ((!heartbeat) && (hse_heartbeat) && (!bBusOffTimer))	//time to send heartbeat message
//				{// time to send heartbeat message
//					heartbeat = HEARTBEAT_TIME;
//					TXB1D0    = nmtstate;								// write data byte for HEARTBEAT
//					TXB1CONbits.TXREQ = 1;							// transmit request
//				}
//			if (bFunc.hsecheck)
//				{// HSE heartbeat check necessary
//					bFunc.hsecheck = false;
//					check_hse (1);									// check if a HSE is not available
//				}
//
//			if(bTime.Time_100ms)
//				{
//					Display_device();			//100ms ����һ����ʾ��Ϣ
//					if(!(buzzer & BUZ_WORKING))
//						Disable_BUZ();
//					else if(buzzer & BUZ_PULSE)
//						{
//							if(bFunc.buz_state)
//								Enable_BUZ();
//							else if(!att_alarm_timer)
//								{
//									Disable_BUZ();
//									att_alarm_timer = buz_alarm_timer;
//								}
//							if(!buz_alarm_totaltimer)
//								buzzer &= !(BUZ_WORKING | BUZ_PULSE);
//						}
//					else
//						Enable_BUZ();
//					bTime.Time_100ms = 0;
//				}
//			if(bTime.Time_2s)
//				{
//					Arrow_Status();
//					bTime.Time_2s = 0;
//				}
//			if(bTime.Time_10ms)
//				{
//					ReadInput();		//��ȡ���еİ�ť����
//					Out_Prog(); 					//�������е����(������ʾ���)
//					bTime.Time_10ms = 0;
//				}
//			for(i = 0; i < mMax_InByte; i++)
//		 		instate_Pre[i] = in[i] ^ in_polarity[i];		// read input state; invert if desired
//
//			WORD keytmp = ((DWORD)((instate_Pre[0] | ((DWORD)instate_Pre[1]<<8) |((DWORD)instate_Pre[1]<<16))>>2)&0xFF); //BTN start IN3
//			if(inold_Pre != keytmp)
//			{
//				for (i = 0; i < 16; i++)
//				{
//					if((!(keytmp>>i)&0x01) && ((inold_Pre>>i) &0x01))   //now 0,pre 1
//					{
//						if( i< 10)
//						{
//							Keytimout = KEYTIMOUT/10;
//							virt_key[virt_key_cnt] = i;
//							virt_key_cnt++ ;
//							if ( virt_key_cnt == MAXKEYBUFF )
//							{
//								virt_key_cnt = MAXKEYBUFF-1;
//                                                                              BYTE  jt =0;
//								for( jt =0 ; jt < MAXKEYBUFF -1 ; jt ++)
//								{
//									virt_key[jt] = virt_key[jt+1];
//								}
//							}
//						}
//
//					}
//				}
//				inold_Pre = keytmp;
//			}
//			if(Keytimout <= 0)
//			{
//				BYTE tagetfloor =0;
//				if(virt_key_cnt!=0)
//				{
//					if(virt_key_cnt==1)
//					{
//						tagetfloor = virt_key[0];
//					}
//					else
//					{
//						tagetfloor = virt_key[virt_key_cnt-2]*10 +virt_key[virt_key_cnt - 1];
//					}
//				}
//				if((tagetfloor !=  0) && (tagetfloor < 55))
//				{
//                                                BYTE cntt =0;
//					for ( cntt = 0; cntt < mInOut_Number; cntt++)
//					{
//						if (inpar [i][IO_BASIC_FUNC] == CAR_CALL)
//						{
//							if(inpar [i][IO_SUB_FUNC] == tagetfloor)
//							bit_set(instate[i/8],i%8);
//							break;
//						}
//					}
//				}
//				Keytimout = MAXLONGVALUE;
//			}
//			if (Check_InChange(instate, inold))					// input state changed
//				{
//					for (i = 0; i < mInOut_Number; i++)
//						{// check all inputs
//							help = bit_select (instate[i/8], i%8);
//							if (help != bit_select (inold[i/8], i%8))
//								{// input has changed
//									inpar [i][IO_STATE] = help;					// set input state
//									if (inpar [i][IO_BASIC_FUNC])				// input has a function
//										{
//											for (j = 0; j < MAX_IO_TYPE; j++)					// write input to virtual input object
//												virt_in [j] = inpar [i][j];
//											switch (inpar [i][IO_BASIC_FUNC])
//												{
//													case (CAR_CALL):						// standard car call
//														if (help)
//														{
//															transmit_in (virt_in);
//															bit_reset(instate[i/8],i%8);
//														}
//														break;
//
//													case (HALL_CALL):						// standard hall call
//														if (help)
//															{// landing call misuse
//																transmit_in (virt_in);
//															}
//														break;
//
//													case (LOAD_IN):							// load measurement input
//														transmit_load ();
//														break;
//
//													default:
//														transmit_in (virt_in);
//														break;
//												}
//					 					}
//								}
//						}
//					for(i=0; i<mMax_InByte; i++)
//						inold[i] = instate[i];
//				}
//		}
//}
//
//
///************************************************************************************************/
///* Low priority interrupt routine																*/
///* Timer 0 : 10 ms																				*/
///* Timer 1 : 1 ms (only MS3-C; Multiplexing of display)											*/
///* Timer 2 : 3 ms read inputs/set outputs, different timers										*/
///* Timer 3 : 20 ms (only PSE and simulation)													*/
///* USART   : Interrupt occurs when the TX buffer is empty										*/
///* Save=INTSAVELOCS: Save content of Tablepointer, Tablelatch (for access to ROM memory) and 	*/
///* and PROD (for multiply)																		*/
///************************************************************************************************/
//void interrupt low_priority InterruptHandlerLow(void){//10ms
//	BYTE i = 0;
//	static BYTE time1_cnt = 0;
//	static BYTE time0_cnt = 0;
//
//	if (PIR1bits.TMR1IF)				//check for Timer 1 overflow
//		{
//			PIR1bits.TMR1IF = 0;		// clear Timer 0 interrupt flag
//			TMR1H = T1H_10ms;
//			TMR1L = T1L_10ms;
//
//			bTime.Time_10ms = 1;
//			time1_cnt++;
//			Keytimout --;
//			if((time1_cnt % 10) == 0)
//				{//100ms
//					bTime.Time_100ms = 1;
//					if (bBusOffTimer)
//						-- bBusOffTimer;
//					else
//						{
//							if(bFunc.buz_set_id)
//								{
//									Disable_BUZ();
//									bFunc.buz_set_id = 0;
//								}
//						}
//				}
//			if((time1_cnt % 200) == 0)
//				bTime.Time_2s = 1;
//		}
//
//	if (INTCONbits.TMR0IF)				//check for Timer 0 overflow
//		{//500ms
//			INTCONbits.TMR0IF = 0;
//			TMR0H = T0H_500ms;
//			TMR0L = T0L_500ms;
//			bTime.Time_500ms = true;
//			bTime.flash_floor_500ms = true;
//			time0_cnt++;
//
//			if (!nmtstate)						// only during Boot up
//				{
//					if (nmtwait)					// waiting time for first heartbeat
//						nmtwait--;
//				}
//			if (heartbeat)						// decrement heartbeat timer
//				heartbeat--;
//
//			if (sdo_timer)			// check for SDO transfer time out
//				sdo_timer--;
//			if (hsetime)
//				{
//					hsetime--;
//					if (!hsetime) 		// HSE heartbeat time out
//						bFunc.hsecheck = true; 	// HSE check necessary
//				}
//			if((time0_cnt % 10) == 0)
//				bTime.Time_5s = 1;
//			if(att_alarm_timer)
//				{
//					--att_alarm_timer;
//					if(!att_alarm_timer)
//						{
//							if(bTime.Time_alarm_fg)
//								buzzer &= ~BUZ_WORKING;
//							bFunc.buz_state = !bFunc.buz_state;
//						}
//				}
//			if(buz_alarm_totaltimer)
//				--buz_alarm_totaltimer;
//
//			for (i = 0; i < mInOut_Number; i++)
//				{// search output parameter list
//					if (outpar [i][IO_BASIC_FUNC] == ARRIVAL_INDICATION)
//					if (outpar [i][IO_ACK])
//						{// arrival indication is on, ������������ź�
//							outpar [i][IO_ACK]--; 			//decrement time counter
//							if (!outpar [i][IO_ACK])		//time out
//								{
//									bit_reset (out[i/8], i % 8);			// switch off output
//									outpar [i][IO_STATE] = 0;
//								}
//						}
//				}
//		}
//	if (PIR1bits.TXIF)
//		{
//			PIR1bits.TXIF = 0;
//			if (uart_send_len)
//				{
//					TXREG = uart_send_buff[uart_send_ro++];
//					if (uart_send_ro == UART_BUFF_SIZE)
//						uart_send_ro = 0;
//					-- uart_send_len;
//				}
//			else
//				{
//					bFunc.uart_send_busy = false;
//					PIE1bits.TXIE		= 0;
//				}
//		}
//}
//
//
