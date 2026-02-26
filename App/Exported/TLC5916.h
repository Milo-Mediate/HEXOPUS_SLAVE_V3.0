/*
 * TLC5916.h
 *
 *  Created on: 17 feb 2026
 *      Author: emili
 */

#ifndef EXPORTED_TLC5916_H_
#define EXPORTED_TLC5916_H_

#include <stdio.h>
#include "stm32h5xx_hal.h"

typedef enum {
	TLC5916_OK = 0,
	TLC5916_OUT_OF_RANGE,
	TLC5916_SPI_ERROR
} TLC5916_error_t;

typedef struct {
	uint8_t led_status;
	SPI_HandleTypeDef *hspi;
	GPIO_TypeDef *oe_port;
	uint16_t oe_pin;
	GPIO_TypeDef *le_port;
	uint16_t le_pin;
} TLC5916_t;



//void TLC5916_Frame_Begin(TLC5916_t *tlc);
//void TLC5916_Frame_End(TLC5916_t *tlc);
//TLC5916_error_t TLC5916_WriteStatus(TLC5916_t *tlc, const uint8_t new_status);
void TLC5916_init(SPI_HandleTypeDef *hspi, TLC5916_t *tlc,
		GPIO_TypeDef *oe_port, uint16_t oe_pin, GPIO_TypeDef *le_port,
		uint16_t le_pin);
TLC5916_error_t TLC5916_TurnOnLed(TLC5916_t *tlc, const uint8_t idx);
TLC5916_error_t TLC5916_TurnOffLed(TLC5916_t *tlc, const uint8_t idx);
TLC5916_error_t TLC5916_SetAll(TLC5916_t *tlc, uint8_t status);

#endif /* EXPORTED_TLC5916_H_ */
