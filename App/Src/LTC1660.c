/**
 * @file LTC1660.c
 * @brief Driver helpers to program the LTC1660 8-channel 10-bit DAC.
 * @details
 * - ::set_dac_channel() builds the 16-bit command word and transmits it via SPI.
 * - ::auto_set_dac_value() performs a coarse auto-calibration loop to bias ADC readings
 *   into a target window by sweeping the DAC code.
 *
 * ## Command word (16 bit)
 * [15:12] = channel (1..8) encoded in upper nibble
 * [11:0]  = 10-bit value left-aligned (value << 6)
 *
 * @author milo
 * @date   2026-01-09
 */

#include "LTC1660.h"
#include "serial_manager.h"
#include "event_manager.h"
#include "ADC_function.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>   /* snprintf */
#include <string.h>  /* memcpy */

#define DAC_RES            10U
#define DAC_MIN            0U
#define DAC_MAX            ((1 << DAC_RES) - 1)
#define ADC_MIN_TGT        450UL
#define ADC_MAX_TGT        750UL
#define SETTLE_MS          2U


#define LTC1660_WD(channel, value) ((uint16_t) (((uint16_t) ((channel + 1) & 0x0F) << 12) | ((uint16_t) (value & 0x03FF) << 2)))
// TODO: _Static_assert su questa macro


/**
 * @brief Program a single LTC1660 channel with a 10-bit value.
 * @param[in] channel Channel index in range [1..8].
 * @param[in] value   10-bit DAC code (0..1023).
 * @note
 * - Builds the 16-bit frame manually with shifts/masks.
 * - Transmits one word via HAL SPI.
 */
bool set_dac_channel(DAC_t *dac, uint8_t channel, uint16_t new_value) {
	if (channel > N_CH)
		return false;
	uint16_t txWord = LTC1660_WD(channel, new_value);
	HAL_GPIO_WritePin(dac->cs_port, dac->cs_pin, GPIO_PIN_RESET);
	HAL_Delay(SETTLE_MS);
	(void) HAL_SPI_Transmit(dac->hspi, (uint8_t*) &txWord, 1U, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(dac->cs_port, dac->cs_pin, GPIO_PIN_SET);
	HAL_Delay(SETTLE_MS);
	dac->values[channel] = new_value;
	return true;
}


uint16_t get_dac_channel(const DAC_t *dac, uint8_t channel)
{
	if (channel > N_CH)
		return -1;
	return dac->values[channel];
}


/**
 * @brief Sweep DAC code until ADC mean falls inside a target window.
 * @param[in] sensor [0..7].
 * @note
 * - Uses `adc1_data_[sens]` as feedback.
 * - Averages @ref ADC_MEAN_SAMPLES samples per step.
 * - Target window [@ref ADC_TARGET_MIN_DEFAULT, @ref ADC_TARGET_MAX_DEFAULT].
 * - On success prints final DAC code; on failure prints an error.
 */
dac_cal_status_t auto_set_channel_value(DAC_t *dac, uint8_t channel) {
	/* Range guard */
	if (channel > N_CH - 1) {
		//send_event(); // TODO: implementare funzione
		return DAC_CAL_TO_DO;
	}

	uint16_t lo = DAC_MIN;
	uint16_t hi = DAC_MAX;
	uint16_t mid = 0;

	for (uint8_t i = 0; i < DAC_RES; i++) {

		mid = (uint16_t) ((lo + hi) >> 1);
		set_dac_channel(dac, channel, mid);
		HAL_Delay(250);
		uint32_t mean_adc = read_adc_mean(0, channel);
		if ((mean_adc >= ADC_MIN_TGT) && (mean_adc <= ADC_MAX_TGT)) {
			return DAC_CAL_OK;
		}
		if (mean_adc > ADC_MAX_TGT) {
			lo = mid + 1U;
		} else {
			hi = mid - 1U;
		}
	}
//	return DAC_CAL_OK;
	return DAC_CAL_OUT_OF_RANGE;
}

dac_cal_status_t calibration_dac(DAC_t *dac)
{
	for (uint8_t i = 0U; i < N_CH; i++)
	{
		if (auto_set_channel_value(dac, i) != DAC_CAL_OK)
			return DAC_CAL_ERR;
	}
	return DAC_CAL_OK;
}

dac_cal_status_t dac_init(SPI_HandleTypeDef *hspi, DAC_t *dac, GPIO_TypeDef *port, uint16_t pin, uint8_t num_ch, uint8_t id)
{
	if (num_ch > N_CH)
		return DAC_CAL_CHANNEL_ERR;
	dac->hspi         = hspi;
	dac->cs_port      = port;
	dac->cs_pin       = pin;
	dac->num_channels = num_ch;
	dac->ID           = id;
	return DAC_CAL_OK;
}
