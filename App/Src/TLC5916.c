/*
 * TLC5916.c
 *
 *  Created on: Feb 18, 2026
 *      Author: emili
 */
#include "TLC5916.h"
#include "stm32h5xx_hal.h"
#include <stdio.h>

void TLC5916_init(SPI_HandleTypeDef *hspi, TLC5916_t *tlc,
		GPIO_TypeDef *oe_port, uint16_t oe_pin, GPIO_TypeDef *le_port,
		uint16_t le_pin) {
	tlc->led_status = 0;
	tlc->hspi = hspi;
	tlc->oe_port = oe_port;
	tlc->oe_pin = oe_pin;
	tlc->le_port = le_port;
	tlc->le_pin = le_pin;

	HAL_GPIO_WritePin(tlc->oe_port, tlc->oe_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(tlc->le_port, tlc->le_pin, GPIO_PIN_RESET);
}

static inline void TLC5916_FrameBegin(TLC5916_t *tlc) {
	HAL_GPIO_WritePin(tlc->oe_port, tlc->oe_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(tlc->le_port, tlc->le_pin, GPIO_PIN_SET);
}

static inline void TLC5916_FrameEnd(TLC5916_t *tlc) {
	HAL_GPIO_WritePin(tlc->le_port, tlc->le_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(tlc->oe_port, tlc->oe_pin, GPIO_PIN_RESET);
}

static TLC5916_error_t TLC5916_WriteStatus(TLC5916_t *tlc, const uint8_t new_status) {
	TLC5916_FrameBegin(tlc);

	HAL_StatusTypeDef st = HAL_SPI_Transmit(tlc->hspi, &new_status, 1, HAL_MAX_DELAY);

	TLC5916_FrameEnd(tlc);
	if (st != HAL_OK)
		return TLC5916_SPI_ERROR;
	return TLC5916_OK;
}

TLC5916_error_t TLC5916_TurnOnLed(TLC5916_t *tlc, const uint8_t idx) {
	if (idx >= 8)
		return TLC5916_OUT_OF_RANGE;
	tlc->led_status |= (1u << idx);
	return TLC5916_WriteStatus(tlc, tlc->led_status);
}

TLC5916_error_t TLC5916_TurnOffLed(TLC5916_t *tlc, const uint8_t idx) {
	if (idx >= 8)
		return TLC5916_OUT_OF_RANGE;
	tlc->led_status &= ~(1u << idx);
	return TLC5916_WriteStatus(tlc, tlc->led_status);
}

TLC5916_error_t TLC5916_SetAll(TLC5916_t *tlc, uint8_t status) {
	tlc->led_status = status;
	return TLC5916_WriteStatus(tlc, tlc->led_status);
}

uint8_t TLC5916_GetStatus (TLC5916_t *tlc) {
	return tlc->led_status;
}

