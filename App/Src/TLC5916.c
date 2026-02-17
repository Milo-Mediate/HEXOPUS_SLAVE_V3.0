/*
 * TLC5916.c
 *
 *  Created on: Feb 5, 2026
 *      Author: emili
 */
#include "TLC5916.h"

static uint8_t led_status = 0;

void TLC5916_Init() {
	HAL_GPIO_WritePin(TLC5916_CS_GPIO_Port, TLC5916_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(TLC5916_LE_GPIO_Port, TLC5916_LE_Pin, GPIO_PIN_RESET);
}

HAL_StatusTypeDef TLC5916_Write_Status(SPI_HandleTypeDef *hspi, const uint8_t new_status)
{
	HAL_GPIO_WritePin(TLC5916_CS_GPIO_Port, TLC5916_CS_Pin, GPIO_PIN_SET); /* OE = High (uscite OFF) */
	HAL_GPIO_WritePin(TLC5916_LE_GPIO_Port, TLC5916_LE_Pin, GPIO_PIN_RESET); /* LE = Low (latch chiuso) */

	HAL_StatusTypeDef st = HAL_SPI_Transmit(hspi, &new_status, 1, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(TLC5916_LE_GPIO_Port, TLC5916_LE_Pin, GPIO_PIN_SET);
	for (volatile uint32_t i = 0; i < 32; ++i)
	{
		__NOP();
	}
	HAL_GPIO_WritePin(TLC5916_LE_GPIO_Port, TLC5916_LE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(TLC5916_CS_GPIO_Port, TLC5916_CS_Pin, GPIO_PIN_RESET);
	return st;
}

uint8_t TLC5916_Read_Status() {
	return led_status;
}

HAL_StatusTypeDef Turn_On_Led(SPI_HandleTypeDef *hspi, const uint8_t idx)
{
	led_status |= (1 << idx);
	return TLC5916_Write_Status(hspi,led_status);
}

HAL_StatusTypeDef Turn_Off_Led(SPI_HandleTypeDef *hspi, const uint8_t idx)
{
	led_status &= ~(1 << idx);
	return TLC5916_Write_Status(hspi,led_status);
}
