/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins
     PB12   ------> FDCAN2_RX
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, LED_YELLOW_SWITCH_Pin|LED_BLUE_SWITCH_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LTC1660_CS_2_GPIO_Port, LTC1660_CS_2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LTC1660_CLR_Pin|LTC1660_CS_Pin|TLC5916_CS_Pin|TLC5916_LE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : XOR_RB_1_Pin XOR_RB_2_Pin */
  GPIO_InitStruct.Pin = XOR_RB_1_Pin|XOR_RB_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : XOR_RB_5_Pin XOR_RB_6_Pin XOR_RB_7_Pin XOR_RB_9_Pin
                           XOR_RB_4_Pin XOR_RB_3_Pin XOR_RB_11_Pin */
  GPIO_InitStruct.Pin = XOR_RB_5_Pin|XOR_RB_6_Pin|XOR_RB_7_Pin|XOR_RB_9_Pin
                          |XOR_RB_4_Pin|XOR_RB_3_Pin|XOR_RB_11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_YELLOW_SWITCH_Pin LED_BLUE_SWITCH_Pin */
  GPIO_InitStruct.Pin = LED_YELLOW_SWITCH_Pin|LED_BLUE_SWITCH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : RESET_INPUT_Pin */
  GPIO_InitStruct.Pin = RESET_INPUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RESET_INPUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : FDCAN_AUX_RX_Pin */
  GPIO_InitStruct.Pin = FDCAN_AUX_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
  HAL_GPIO_Init(FDCAN_AUX_RX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : XOR_RB_8_Pin */
  GPIO_InitStruct.Pin = XOR_RB_8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(XOR_RB_8_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LTC1660_CS_2_Pin LTC1660_CLR_Pin LTC1660_CS_Pin TLC5916_CS_Pin
                           TLC5916_LE_Pin */
  GPIO_InitStruct.Pin = LTC1660_CS_2_Pin|LTC1660_CLR_Pin|LTC1660_CS_Pin|TLC5916_CS_Pin
                          |TLC5916_LE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : XOR_RB_10_Pin */
  GPIO_InitStruct.Pin = XOR_RB_10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(XOR_RB_10_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : XOR_RB_12_Pin */
  GPIO_InitStruct.Pin = XOR_RB_12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(XOR_RB_12_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
