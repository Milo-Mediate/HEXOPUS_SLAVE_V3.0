/*
 * ADC_function.c
 *
 *  Created on: Feb 3, 2026
 *      Author: emili
 */

#include "ADC_function.h"
#include "app_hw_definition.h"
#include "logging.h"
#include <stdio.h>

#include <assert.h>
#define ASSERT(condition, message) assert(condition && message)

#define ADC_MEAN_SHIFT     7U
#define ADC_MEAN_SAMPLES   (1 << ADC_MEAN_SHIFT)

#define ARRAY_LEN(a) (sizeof(a) / sizeof(a[0]))

static volatile uint32_t adc1_data[NUM_SENS];
static volatile uint32_t adc2_data[NUM_DSP];

static const uint16_t adc_dma_len[] = { ARRAY_LEN(adc1_data), ARRAY_LEN(adc2_data) };

static const uint32_t volatile * adcs[] = { adc1_data, adc2_data };

#define ASSERT_PORT(port) assert((port) < ARRAY_LEN(adcs) && "port must be 0 or 1")

ADC_Status init_adc(ADC_Port port, ADC_HandleTypeDef *hadc)
{
	ASSERT_PORT(port);

	HAL_StatusTypeDef status;

	status = HAL_ADC_Start_DMA(hadc, (uint32_t *)adcs[port], adc_dma_len[port]);
	if (status != HAL_OK)
		return ADC_INIT_ERROR_START_DMA;

//	status = HAL_ADC_Start(hadc);
//	if (status != HAL_OK)
//		return status;

	return ADC_OK;
}

inline uint32_t read_adc(ADC_Port port, uint8_t sens)
{
	ASSERT_PORT(port);
	assert(sens <= adc_dma_len[port]);

	return adcs[port][sens];
}

uint32_t read_adc_mean(ADC_Port port, uint8_t sens)
{
	ASSERT_PORT(port);
	assert(sens <= adc_dma_len[port]);

	const volatile uint32_t *adc = adcs[port];
	uint32_t sum = 0;
	for (uint32_t i = 0; i < ADC_MEAN_SAMPLES; i++)
	{
//		while (flag_adc_new_sample_ == false)
//		{ /* spin */
//		}
//		flag_adc_new_sample_ = false;
		sum += adc[sens];
	}
	return sum >> ADC_MEAN_SHIFT;
}

