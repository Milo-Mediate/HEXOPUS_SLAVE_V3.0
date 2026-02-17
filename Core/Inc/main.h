/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32h5xx_hal.h"

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
#define XOR_RB_1_Pin GPIO_PIN_5
#define XOR_RB_1_GPIO_Port GPIOE
#define XOR_RB_2_Pin GPIO_PIN_6
#define XOR_RB_2_GPIO_Port GPIOE
#define XOR_RB_5_Pin GPIO_PIN_1
#define XOR_RB_5_GPIO_Port GPIOF
#define XOR_RB_6_Pin GPIO_PIN_2
#define XOR_RB_6_GPIO_Port GPIOF
#define XOR_RB_7_Pin GPIO_PIN_3
#define XOR_RB_7_GPIO_Port GPIOF
#define XOR_RB_9_Pin GPIO_PIN_4
#define XOR_RB_9_GPIO_Port GPIOF
#define LED_YELLOW_SWITCH_Pin GPIO_PIN_6
#define LED_YELLOW_SWITCH_GPIO_Port GPIOF
#define LED_BLUE_SWITCH_Pin GPIO_PIN_7
#define LED_BLUE_SWITCH_GPIO_Port GPIOF
#define XOR_RB_4_Pin GPIO_PIN_8
#define XOR_RB_4_GPIO_Port GPIOF
#define XOR_RB_3_Pin GPIO_PIN_9
#define XOR_RB_3_GPIO_Port GPIOF
#define XOR_RB_11_Pin GPIO_PIN_10
#define XOR_RB_11_GPIO_Port GPIOF
#define CAP_SEN_6_Pin GPIO_PIN_0
#define CAP_SEN_6_GPIO_Port GPIOC
#define DUM_SEN_6_Pin GPIO_PIN_1
#define DUM_SEN_6_GPIO_Port GPIOC
#define DSP_SEN_2_Pin GPIO_PIN_3
#define DSP_SEN_2_GPIO_Port GPIOC
#define DUM_SEN_1_Pin GPIO_PIN_0
#define DUM_SEN_1_GPIO_Port GPIOA
#define CAP_SEN_1_Pin GPIO_PIN_1
#define CAP_SEN_1_GPIO_Port GPIOA
#define DSP_SEN_3_Pin GPIO_PIN_2
#define DSP_SEN_3_GPIO_Port GPIOA
#define DSP_SEN_4_Pin GPIO_PIN_3
#define DSP_SEN_4_GPIO_Port GPIOA
#define DSP_SEN_5_Pin GPIO_PIN_4
#define DSP_SEN_5_GPIO_Port GPIOA
#define DSP_SEN_6_Pin GPIO_PIN_5
#define DSP_SEN_6_GPIO_Port GPIOA
#define CAP_SEN_3_Pin GPIO_PIN_6
#define CAP_SEN_3_GPIO_Port GPIOA
#define DUM_SEN_3_Pin GPIO_PIN_7
#define DUM_SEN_3_GPIO_Port GPIOA
#define CAP_SEN_4_Pin GPIO_PIN_4
#define CAP_SEN_4_GPIO_Port GPIOC
#define DUM_SEN_4_Pin GPIO_PIN_5
#define DUM_SEN_4_GPIO_Port GPIOC
#define DUM_SEN_5_Pin GPIO_PIN_0
#define DUM_SEN_5_GPIO_Port GPIOB
#define CAP_SEN_5_Pin GPIO_PIN_1
#define CAP_SEN_5_GPIO_Port GPIOB
#define CAP_SEN_2_Pin GPIO_PIN_11
#define CAP_SEN_2_GPIO_Port GPIOF
#define DUM_SEN_2_Pin GPIO_PIN_12
#define DUM_SEN_2_GPIO_Port GPIOF
#define DSP_SEN_1_Pin GPIO_PIN_14
#define DSP_SEN_1_GPIO_Port GPIOF
#define LTC1660_MOSI_Pin GPIO_PIN_1
#define LTC1660_MOSI_GPIO_Port GPIOG
#define RESET_INPUT_Pin GPIO_PIN_10
#define RESET_INPUT_GPIO_Port GPIOB
#define FDCAN_AUX_RX_Pin GPIO_PIN_12
#define FDCAN_AUX_RX_GPIO_Port GPIOB
#define XOR_RB_8_Pin GPIO_PIN_15
#define XOR_RB_8_GPIO_Port GPIOB
#define LTC1660_CS_2_Pin GPIO_PIN_11
#define LTC1660_CS_2_GPIO_Port GPIOD
#define LTC1660_CLR_Pin GPIO_PIN_12
#define LTC1660_CLR_GPIO_Port GPIOD
#define LTC1660_CS_Pin GPIO_PIN_13
#define LTC1660_CS_GPIO_Port GPIOD
#define TLC5916_CS_Pin GPIO_PIN_14
#define TLC5916_CS_GPIO_Port GPIOD
#define TLC5916_LE_Pin GPIO_PIN_15
#define TLC5916_LE_GPIO_Port GPIOD
#define DIRECT_DDS_Pin GPIO_PIN_9
#define DIRECT_DDS_GPIO_Port GPIOC
#define FDCAN_RX_Pin GPIO_PIN_11
#define FDCAN_RX_GPIO_Port GPIOA
#define FDCAN_TX_Pin GPIO_PIN_12
#define FDCAN_TX_GPIO_Port GPIOA
#define TLC5916_SCK_Pin GPIO_PIN_10
#define TLC5916_SCK_GPIO_Port GPIOC
#define TLC5916_MISO_Pin GPIO_PIN_11
#define TLC5916_MISO_GPIO_Port GPIOC
#define TLC5916_MOSI_Pin GPIO_PIN_12
#define TLC5916_MOSI_GPIO_Port GPIOC
#define LTC1660_SCK_Pin GPIO_PIN_3
#define LTC1660_SCK_GPIO_Port GPIOD
#define XOR_RB_10_Pin GPIO_PIN_7
#define XOR_RB_10_GPIO_Port GPIOD
#define XOR_RB_12_Pin GPIO_PIN_11
#define XOR_RB_12_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
