/*
 * ADC_function.h
 *
 *  Created on: Feb 3, 2026
 *      Author: emili
 */

#ifndef EXPORTED_ADC_FUNCTION_H_
#define EXPORTED_ADC_FUNCTION_H_
#include "stm32h5xx_hal.h"

#include <stdint.h>


typedef enum
{
    ADC_1,
	ADC_2,
} ADC_Port;

typedef enum
{
    ADC_OK,
    ADC_ERROR,
    ADC_BUSY,
    ADC_TIMEOUT,
    ADC_INIT_ERROR_START_DMA,
    ADC_INIT_ERROR_START,
} ADC_Status;

ADC_Status init_adc(ADC_Port port, ADC_HandleTypeDef *hadc);
uint32_t read_adc(ADC_Port port, uint8_t sens);
uint32_t read_adc_mean(ADC_Port port, uint8_t sens);

#endif /* EXPORTED_ADC_FUNCTION_H_ */
