/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define	_MAIN_C_
#include "AllHeader.h"
#include "flash.h"
#include "stdio.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAXLONGVALUE 2000000000;
#define MAXKEYBUFF          	2
#define KEYTIMOUT            1500				//ms
#define LEDTIMEOUT     500						//ms

#define USER_DEBUG 0

int Keytimout = INT32_MAX;
uint32_t lastest_call_time;
int LedOfftimout = INT32_MAX;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
USART_HandleTypeDef husart2;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t instate[MAX_IN_BYTE];
uint8_t instate_Pre[MAX_IN_BYTE];
uint32_t inold_Pre;
uint8_t help;
uint8_t virt_key[2] = {0,0};
uint8_t virt_key_cnt =0;

uint32_t TIM1_inter,TIM2_inter;
uint32_t TIM1_inter_store,TIM2_inter_store;
uint32_t time10ms,time500ms =0;
char FloorName[TOTAL_FLOOR][2] ={0};
char IOName[13] ={'0','1','2','3','4','5','6','7','8','9','G','B','C'};

uint8_t targetfloor =0;
uint8_t targetfloor_reg =0;
uint16_t Led_virt = 0xFFFF;

static uint32_t time1_cnt = 0;

uint32_t inspection_time;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	uint8_t i = 0;

	static uint32_t time0_cnt = 0;
	static uint32_t reloadtime =0;
	if(htim->Instance == htim1.Instance)
	{
		bTime.Time_10ms = 1;
		time1_cnt++;
		time10ms = HAL_GetTick() - reloadtime;
		reloadtime = HAL_GetTick();
		Keytimout --;
		LedOfftimout --;
		if((time1_cnt % 10) == 0)
			{//100ms
				bTime.Time_100ms = 1;
				if (bBusOffTimer)
					-- bBusOffTimer;
				else
					{
						if(bFunc.buz_set_id)
							{
								Disable_BUZ();
								bFunc.buz_set_id = 0;
							}
					}
			}
		if((time1_cnt % 200) == 0)
			bTime.Time_2s = 1;
	}
	else if(htim->Instance == htim2.Instance)
	{
					bTime.Time_500ms = true;
					bTime.flash_floor_500ms = true;
					time0_cnt++;

					if (!nmtstate)						// only during Boot up
						{
							if (nmtwait)					// waiting time for first heartbeat
								nmtwait--;
						}
					if (heartbeat)						// decrement heartbeat timer
						heartbeat--;

					if (sdo_timer)			// check for SDO transfer time out
						sdo_timer--;
					if (hsetime)
						{
							hsetime--;
							if (!hsetime) 		// HSE heartbeat time out
								bFunc.hsecheck = true; 	// HSE check necessary
						}
					if((time0_cnt % 10) == 0)
						bTime.Time_5s = 1;
					if(att_alarm_timer)
						{
							--att_alarm_timer;
							if(!att_alarm_timer)
								{
									if(bTime.Time_alarm_fg)
										buzzer &= ~BUZ_WORKING;
									bFunc.buz_state = !bFunc.buz_state;
								}
						}
					if(buz_alarm_totaltimer)
						--buz_alarm_totaltimer;

					for (i = 0; i < mInOut_Number; i++)
						{// search output parameter list
							if (outpar [i][IO_BASIC_FUNC] == ARRIVAL_INDICATION)
							if (outpar [i][IO_ACK])
								{// arrival indication is on, ������������ź�
									outpar [i][IO_ACK]--; 			//decrement time counter
									if (!outpar [i][IO_ACK])		//time out
										{
											bit_reset (out[i/8], i % 8);			// switch off output
											outpar [i][IO_STATE] = 0;
										}
								}
						}

	}
}
void check_hse (uint8_t mode){
	uint8_t help;
	uint8_t i;

	help = 0;
	if (!hsetime)								/* 5s no heartbeat from HSE				*/
		{
			help = 1;
			hsetime  = HSETIME;						/* restart HSE heartbeat timer			*/
			display[BUF_TEN] = 'N';			//A_BETR;
			display[BUF_UNIT] = 'C';		//A_BETR;
			display[BUF_ARROW] = 0;
			display[BUF_MESSAGE] = 0;
			hse_heartbeat = 0;
			mDisp_buf[BUF_TEN + 1] = display[BUF_TEN];
			mDisp_buf[BUF_UNIT + 1] = display[BUF_UNIT];
			mDisp_buf[BUF_ARROW + 1] = display[BUF_ARROW];
		}

	if (help && mode)											/* one or more HSE not available		*/
		{
			for (i = 0; i < mInOut_Number; i++)					/* check all call indications			*/
				{
					if (check_for_call (outpar [i][IO_BASIC_FUNC]))
						{											/* output is call acknowledgement		*/
				  		outpar [i][IO_ACK] &= ~help;
							if (!outpar [i][IO_ACK])				/* all acknowledgements cancelled		*/
								{
									bit_reset (out[i/8], i%8);					/* clear output							*/
									outpar [i][IO_STATE] = 0;
								}
						}
					else										/* all other output functions			*/
						{
							if (outpar [i][IO_LIFT] & help)			/* output for this lift					*/
								{
									if ((outpar [i][IO_BASIC_FUNC] == SPECIAL_FUNC) &&
									    (outpar [i][IO_SUB_FUNC]	  == OUT_OF_ORDER))
										{
											bit_set (out[i/8], i%8);				/* set physical output					*/
					 						outpar [i][IO_STATE] = 1;
										}
									else
										{
											bit_reset (out[i/8], i%8);				/* reset physical output				*/
											outpar [i][IO_STATE] = 0;
										}
								}
						}
				}
		}
}
void ClrWdt (void)
{
#if ! USER_DEBUG
	HAL_IWDG_Refresh(&hiwdg);
	HAL_WWDG_Refresh(&hwwdg);
#endif
}
int Find_target_Floor(int len)
{
	if(len ==1)
	{
		virt_key[1] = virt_key[0];
		virt_key[0] = 0;
	}
	else
	{
		if(virt_key[0] == '0')
			virt_key[0] =0;
	}
	for(int icc=0;icc<TOTAL_FLOOR;icc++)
	{
		if((FloorName[icc][0] == virt_key[0]) && (FloorName[icc][1] == virt_key[1]) )
		{
			targetfloor = icc+1;
			return targetfloor;
		}
	}
	return -1;
}
extern void Usart_send_frame(void);
uint8_t checkwatchdog =0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_DMA_Init();
  MX_USART2_Init();
  /* USER CODE BEGIN 2 */
  USART2_TX_LOW;
 uint8_t data[2] ={0xB0,0};
 HAL_TIM_Base_Start(&htim3);
// mDisp_buf[1] = 'B';
// mDisp_buf[2] = '1';
// mDisp_buf[3] = 0;
//  while(1)
//  {
//	  Usart_send_frame();
//	  //HAL_USART_Transmit(&husart2, &data, 1, 10);
////	  husart2.Instance->DR =0xB0;
//
//	  Display_device();
//	  HAL_Delay(50);
//  }
  ClrWdt();							//reset watchdog timer
  Flash_Read_Bytes((uint8_t *)&FloorName, DATA_START_ADDRESS, sizeof(FloorName));
  __HAL_DBGMCU_FREEZE_WWDG();
  __HAL_DBGMCU_FREEZE_IWDG();
 	//Flash_ReadChar(data,DATA_START_ADDRESS,LENGTH_START_ADDRESS);

  mMax_InByte =3;
  memset((void*)out,0xFF,8);
  Led_virt = 0xFFFF;
  Out_Prog();
  for(int ct =0;ct<200;ct++)
  {
		  ClrWdt ();
		  HAL_Delay(1);
  }

  memset((void*)out,0x00,8);
  Led_virt = 0x00;
  Out_Prog();
  	uint8_t i, j;

	if (merker == RC_MERKER)									// restart by after Rx counter error
		{
			errorregister |= ER_COMMUNICATION;						// set error bits
		}
	nmtstate  	= BOOT_UP;		// set CAN NMT state to Boot up
	nmtwait = 2;	// waiting time to 1. heartbeat in INI-Mode waiting time	1s

	hsetime = HSETIME;
	for(i = 0; i < MAX_IN_BYTE; i++)
	{
		out[i] = 0;
		in[i] = 0x00;
		input[0][i] = 0x00;
		input[1][i] = 0x00;
		input[2][i] = 0x00;
		inold[i] = 0x00;
		instate[i] = 0;
		in_polarity[i] = 0;
		out_polarity[i] = 0;
	}
	for (i = 0; i < MAX_IN; i++)
	{
		inpar  [i][IO_STATE] = 0;
		outpar [i][IO_STATE] = 0;
		inpar  [i][IO_BASIC_FUNC] = 0;
		outpar [i][IO_BASIC_FUNC] = 0;
		outpar [i][IO_ACK] = 0;
	}
	for(i = 0; i < 4; i++)
		display[i] = 0;
	for(i = 0; i < 5; i++)
		mDisp_buf[i] = 0;

	mArrow_State = 0;
	mExtern_Number = 0;
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim2);
	while (nmtwait)				// waiting time to 1. heartbeat
		{
		 ClrWdt();							//reset watchdog timer
			if(bTime.Time_10ms)
				{
					ReadInput();
					bTime.Time_10ms = 0;
				}
		}
	node_id = Get_NodeID();

	Init_Can();


	heartbeat = HEARTBEAT_TIME;

	nmtstate = PRE_OP;						// set state pre-operational
	disp_lift = LIFT1;

	while (nmtstate == PRE_OP)
	{//�ȴ������������ָ��
		ClrWdt ();
		if (rc)					// Message in receive buffer
			read_rx ();		// read and handle message
		if (sdo_index && !sdo_timer)
			{// SDO segment transfer time out
				sdo_index = 0;
				abort_sdo (SDO_TIMEOUT);						//send SDO abort request
			}

		if (bFunc.hsecheck)
			{// HSE heartbeat check necessary
				bFunc.hsecheck = false;
				check_hse (0);									// check if a HSE is not available
			}
		if ((!heartbeat) && (hse_heartbeat) && (!bBusOffTimer))	//time to send heartbeat message
			{
				heartbeat = HEARTBEAT_TIME;
				CAN_transmit_heartbeat();
			}

		if (errorcode)
			{// error occured
				transmit_error ();	// send emergency message
				errorregister = 0;	// reset error
				errorcode = 0;
			}
		if ((merker == BS_MERKER) && (!bBusOffTimer))
			{
				Init_Can();
				errorregister |= ER_COMMUNICATION;			// set error bits
				errorcode = E_BUS_OFF_A;									// set error code
				merker = 0;
			}
		if(nmtstate == TEST_MODE)
			{
				test_mode();
			}
	}
	hsetime = HSETIME ;		//start timer with different times
	bFunc.hsecheck = false;					//no HSE check now
	ClrWdt();							//reset watchdog timer

	mInOut_Number = (mExtern_Number * 16 + 22) % (MAX_IN + 1);
	mMax_InByte = (mInOut_Number >> 3) + 1;

	set_io_config ();
	Arrow_Status();
	int cntt =0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  ClrWdt();							//reset watchdog timer
	  if(checkwatchdog)
	  {
		  while(1);
	  }
	  Usart_send_frame();
		if (rc)													// Message in receive buffer
			read_rx ();										// read and handle message
		if ((!heartbeat) && (hse_heartbeat) && (!bBusOffTimer))	//time to send heartbeat message
			{// time to send heartbeat message
				heartbeat = HEARTBEAT_TIME;
				CAN_transmit_heartbeat();
			}
		if (bFunc.hsecheck)
			{// HSE heartbeat check necessary
				bFunc.hsecheck = false;
				check_hse (1);									// check if a HSE is not available
			}

		if(bTime.Time_100ms)
			{
				Display_device();			//100ms ����һ����ʾ��Ϣ
				if(!(buzzer & BUZ_WORKING))
					Disable_BUZ();
				else if(buzzer & BUZ_PULSE)
					{
						if(bFunc.buz_state)
							Enable_BUZ();
						else if(!att_alarm_timer)
							{
								Disable_BUZ();
								att_alarm_timer = buz_alarm_timer;
							}
						if(!buz_alarm_totaltimer)
							buzzer &= !(BUZ_WORKING | BUZ_PULSE);
					}
				else
					Enable_BUZ();
				bTime.Time_100ms = 0;
			}
		if(bTime.Time_2s)
			{
				Arrow_Status();
				bTime.Time_2s = 0;
			}
		if(bTime.Time_10ms)
			{
				ReadInput();		//��ȡ���еİ�ť����
				Out_Prog(); 					//�������е����(������ʾ���)
				bTime.Time_10ms = 0;
			}

		for(i = 0; i < mMax_InByte; i++)
			instate[i] = in[i] ^ in_polarity[i];		// read input state; invert if desired
//		instate[0] = (instate_Pre[0] & 0x3);      //close and openbutton
//		uint16_t keytmp = (uint16_t)((instate_Pre[0] | ((uint16_t)instate_Pre[1]<<8)) >>2); //BTN start IN3
//		if(inold_Pre != keytmp)
//		{
//			for (i = 0; i < 16; i++)
//			{
//				if((!(keytmp>>i)&0x01) && ((inold_Pre>>i) &0x01))   //now 0,pre 1
//				{
//						Led_virt = (Led_virt|(1<<(i+2))) & 0xFFFC;
//						virt_key[virt_key_cnt] = IOName[i];
//						virt_in [IO_BASIC_FUNC] = CAR_CALL;
//						virt_in [IO_SUB_FUNC] = 0xCC;
//						virt_in [IO_LIFT] = ~LIFT1;   //remove G664
//						virt_in [IO_FLOOR] = 0;
//						virt_in [IO_DOOR] = 0;
//						virt_in [IO_STATE] = virt_key[virt_key_cnt];
//						virt_in [IO_ENABLE] = 0;
//						virt_in [IO_ACK] = 0;
//
//						transmit_in (virt_in);  //tran first package
//						virt_key_cnt++ ;
//						if ( virt_key_cnt == 2 )
//						{
//							Keytimout = -1;
//						}
//						else
//						{
//							Keytimout = KEYTIMOUT/10;
//							inspection_time = HAL_GetTick();
//						}
//						if(IOName[i] == 'C')
//						{
//							Keytimout = -1;
//						}
//
//				}
//			}
//			inold_Pre = keytmp;
//		}
//		if(LedOfftimout <0)
//		{
//			Led_virt = 0x00;
//			LedOfftimout = INT32_MAX;
//		}
//		if(Keytimout <= 0)
//		{
//			inspection_time = HAL_GetTick() - inspection_time;
//			if((virt_key[0] != 'C') && (virt_key[1] != 'C'))
//			{
//				targetfloor = Find_target_Floor(virt_key_cnt);
//				if((targetfloor !=  0) && (targetfloor <= TOTAL_FLOOR))
//				{
//
//					for ( cntt = 0; cntt < mInOut_Number; cntt++)
//					{
//						if (inpar [cntt][IO_BASIC_FUNC] == CAR_CALL)
//						{
//							if(inpar [cntt][IO_SUB_FUNC] == targetfloor)
//							{
//
//								bit_set(instate[cntt/8],cntt%8);
//								lastest_call_time = time1_cnt + KEYTIMOUT/10;
//								break;
//							}
//
//						}
//					}
//					if(cntt == mInOut_Number)
//					{
//						cntt = -1;
//					}
//				}
//			}
//			else
//			{
//				if(virt_key[0] == 'C')
//				{
//					//del target_floor_reg
//					if(time1_cnt < lastest_call_time)
//					{
//						targetfloor = targetfloor_reg;
//					}
//				}
//				else if(virt_key[1] == 'C')
//				{
//					//del target_floor virt_key[1]
//					virt_key_cnt =1;
//					targetfloor = Find_target_Floor(virt_key_cnt);
//				}
//				if(targetfloor>0)
//				{
//					uint8_t outindex ;
//					for ( outindex = 0; outindex < mInOut_Number; outindex++)
//					{
//						if (outpar [outindex][IO_BASIC_FUNC] == CAR_CALL)
//						{
//							if(outpar [outindex][IO_SUB_FUNC] == targetfloor)
//							{
//								break;
//							}
//
//						}
//					}
//					if(outindex != mInOut_Number)
//					{
//						if( bit_select(out[outindex/8],outindex%8))	//alread call
//						{
//							for ( cntt = 0; cntt < mInOut_Number; cntt++)
//							{
//								if (inpar [cntt][IO_BASIC_FUNC] == CAR_CALL)
//								{
//									if(inpar [cntt][IO_SUB_FUNC] == targetfloor)
//									{
//										bit_set(instate[cntt/8],cntt%8);
//										for (j = 0; j < MAX_IO_TYPE; j++)					// write input to virtual input object
//												virt_in [j] = inpar [cntt][j];
//										transmit_in (virt_in);  //tran first package
//										break;
//									}
//
//								}
//							}
//						}
//					}
//				}
//			}
//			virt_key_cnt =0;
//			Keytimout = MAXLONGVALUE;
//			LedOfftimout = LEDTIMEOUT/10;
//			virt_key[0] = 0;
//			virt_key[1] = 0;
//		}
		if (Check_InChange(instate, inold))					// input state changed
			{
				for (i = 0; i < mInOut_Number; i++)
					{// check all inputs
						help = bit_select (instate[i/8], i%8);
						if (help != bit_select (inold[i/8], i%8))
							{// input has changed
								inpar [i][IO_STATE] = help;					// set input state
								if (inpar [i][IO_BASIC_FUNC])				// input has a function
									{
										for (j = 0; j < MAX_IO_TYPE; j++)					// write input to virtual input object
											virt_in [j] = inpar [i][j];
										switch (inpar [i][IO_BASIC_FUNC])
											{
												case (CAR_CALL):						// standard car call
													if (help)
													{
														transmit_in (virt_in);
//														bit_reset(instate[i/8],i%8);
//														if(cntt >= 0)
//														{
//															bit_reset(instate[cntt/8],cntt%8);
//															targetfloor_reg = targetfloor;
//															targetfloor = -1;
//														}

													}
													break;

												case (HALL_CALL):						// standard hall call
													if (help)
														{// landing call misuse
															transmit_in (virt_in);
														}
													break;

												case (LOAD_IN):							// load measurement input
													transmit_load ();
													break;

												default:
													transmit_in (virt_in);
													break;
											}
				 					}
							}
					}
				for(i=0; i<mMax_InByte; i++)
					inold[i] = instate[i];
			}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 16;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_15TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = ENABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

  /* USER CODE END CAN_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 7199;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 99;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7199;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 71;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  husart2.Instance = USART2;
  husart2.Init.BaudRate = 9600;
  husart2.Init.WordLength = USART_WORDLENGTH_8B;
  husart2.Init.StopBits = USART_STOPBITS_1;
  husart2.Init.Parity = USART_PARITY_NONE;
  husart2.Init.Mode = USART_MODE_TX_RX;
  husart2.Init.CLKPolarity = USART_POLARITY_LOW;
  husart2.Init.CLKPhase = USART_PHASE_2EDGE;
  husart2.Init.CLKLastBit = USART_LASTBIT_DISABLE;
  if (HAL_USART_Init(&husart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Buzzer_Out_Pin|HC595_SEL_Pin|SCK_Pin|MOSI_Pin
                          |UART_DE_Pin|HC166_SEL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Buzzer_Out_Pin HC595_SEL_Pin MOSI_Pin UART_DE_Pin
                           HC166_SEL_Pin */
  GPIO_InitStruct.Pin = Buzzer_Out_Pin|HC595_SEL_Pin|MOSI_Pin|UART_DE_Pin
                          |HC166_SEL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SCK_Pin */
  GPIO_InitStruct.Pin = SCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SCK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MISO_Pin */
  GPIO_InitStruct.Pin = MISO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MISO_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : IC_IN1_Pin IC_IN2_Pin IC_IN3_Pin IC_IN11_Pin
                           IC_IN12_Pin IC_IN13_Pin IC_IN14_Pin IC_IN4_Pin
                           IC_IN5_Pin IC_IN6_Pin IC_IN7_Pin IC_IN8_Pin
                           IC_IN9_Pin IC_IN10_Pin */
  GPIO_InitStruct.Pin = IC_IN1_Pin|IC_IN2_Pin|IC_IN3_Pin|IC_IN11_Pin
                          |IC_IN12_Pin|IC_IN13_Pin|IC_IN14_Pin|IC_IN4_Pin
                          |IC_IN5_Pin|IC_IN6_Pin|IC_IN7_Pin|IC_IN8_Pin
                          |IC_IN9_Pin|IC_IN10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : NODE_ID_Pin */
  GPIO_InitStruct.Pin = NODE_ID_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(NODE_ID_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

