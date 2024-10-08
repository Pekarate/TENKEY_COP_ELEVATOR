/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Buzzer_Out_Pin GPIO_PIN_0
#define Buzzer_Out_GPIO_Port GPIOA
#define HC595_SEL_Pin GPIO_PIN_1
#define HC595_SEL_GPIO_Port GPIOA
#define SCK_Pin GPIO_PIN_5
#define SCK_GPIO_Port GPIOA
#define MISO_Pin GPIO_PIN_6
#define MISO_GPIO_Port GPIOA
#define MOSI_Pin GPIO_PIN_7
#define MOSI_GPIO_Port GPIOA
#define IC_IN1_Pin GPIO_PIN_0
#define IC_IN1_GPIO_Port GPIOB
#define IC_IN2_Pin GPIO_PIN_1
#define IC_IN2_GPIO_Port GPIOB
#define IC_IN3_Pin GPIO_PIN_2
#define IC_IN3_GPIO_Port GPIOB
#define IC_IN11_Pin GPIO_PIN_10
#define IC_IN11_GPIO_Port GPIOB
#define IC_IN12_Pin GPIO_PIN_11
#define IC_IN12_GPIO_Port GPIOB
#define IC_IN13_Pin GPIO_PIN_12
#define IC_IN13_GPIO_Port GPIOB
#define IC_IN14_Pin GPIO_PIN_13
#define IC_IN14_GPIO_Port GPIOB
#define NODE_ID_Pin GPIO_PIN_14
#define NODE_ID_GPIO_Port GPIOB
#define UART_DE_Pin GPIO_PIN_8
#define UART_DE_GPIO_Port GPIOA
#define HC166_SEL_Pin GPIO_PIN_15
#define HC166_SEL_GPIO_Port GPIOA
#define IC_IN4_Pin GPIO_PIN_3
#define IC_IN4_GPIO_Port GPIOB
#define IC_IN5_Pin GPIO_PIN_4
#define IC_IN5_GPIO_Port GPIOB
#define IC_IN6_Pin GPIO_PIN_5
#define IC_IN6_GPIO_Port GPIOB
#define IC_IN7_Pin GPIO_PIN_6
#define IC_IN7_GPIO_Port GPIOB
#define IC_IN8_Pin GPIO_PIN_7
#define IC_IN8_GPIO_Port GPIOB
#define IC_IN9_Pin GPIO_PIN_8
#define IC_IN9_GPIO_Port GPIOB
#define IC_IN10_Pin GPIO_PIN_9
#define IC_IN10_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define SCK (*(volatile uint32_t *)0x42210194)
#define PIN_A6 (*(volatile uint32_t *)0x42210118)  //i
#define PIN_A7 (*(volatile uint32_t *)0x4221019C)
#define HC959_SEl (*(volatile uint32_t *)0x42210184)  //PA1_O
#define HC166_SEl (*(volatile uint32_t *)0x422101BC)  //PA15_O
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
