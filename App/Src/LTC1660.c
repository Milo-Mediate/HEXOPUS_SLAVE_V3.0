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
#include <stdbool.h>
#include <stdio.h>   /* snprintf */
#include <string.h>  /* memcpy */

#define N_CH               8U
#define DAC_MIN            0U
#define DAC_MAX            1023U
#define ADC_MIN_TGT        250UL
#define ADC_MAX_TGT        650UL
#define SETTLE_MS          1U
#define ADC_MEAN_SHIFT     7U

static uint32_t read_adc_mean(uint8_t sens) {
	const uint8_t mean_samples = 1 << ADC_MEAN_SHIFT;
	uint32_t sum = 0;
	for (uint32_t i = 0; i < mean_samples; i++) {
		while (flag_adc_new_sample_ == false) { /* spin */
		}
		flag_adc_new_sample_ = false;

		sum += adc1_data_[sens];
	}
	return sum >> ADC_MEAN_SHIFT;
}

/**
 * @brief Program a single LTC1660 channel with a 10-bit value.
 * @param[in] channel Channel index in range [1..8].
 * @param[in] value   10-bit DAC code (0..1023).
 * @note
 * - Builds the 16-bit frame manually with shifts/masks.
 * - Transmits one word via HAL SPI.
 */

void set_dac_channel(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port,
		uint16_t cs_pin, uint8_t channel, uint16_t value) {
	// ToDo: inserire LOG
	uint16_t txWord = 1U;
	if ((channel > 0U) && (channel <= N_CH)) {
		txWord = (uint16_t) (((uint16_t) (channel & 0x0F) << 12)
				| ((uint16_t) (value & 0x03FF) << 2));
		HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_RESET);
		HAL_Delay(SETTLE_MS);
		(void) HAL_SPI_Transmit(hspi, (uint8_t*) &txWord, 1U, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);
		HAL_Delay(SETTLE_MS);
	}
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
void auto_set_dac_value(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port,
		uint16_t cs_pin, uint8_t sens) {
	/* Range guard */
	if (sens > 7U) {
		send_event();
		return;
	}
	uint8_t channel = (uint8_t) (sens + 1U);

	uint16_t lo = DAC_MIN;
	uint16_t hi = DAC_MAX;
	uint16_t mid = 0;

	uint16_t dac_tick = 0U;
	uint32_t sum_adc = 0UL;
	uint32_t mean_adc = 0UL;

	//ToDo: inserire la parte di lettura
	for (uint8_t iter = 0; iter < 12U; iter++) {
		mid = (uint16_t) ((lo + hi) >> 1);
		set_dac_channel(hspi, cs_port, cs_pin, channel, dac_tick);
		HAL_Delay(SETTLE_MS);
		for (uint8_t i = 0U; i < ADC_MEAN_SAMPLES; i++) {
			sum_adc += adc1_data_[sens];
		}
		mean_adc = (uint32_t) (sum_adc / (uint32_t) ADC_MEAN_SAMPLES);
		if ((mean_adc >= ADC_MIN_TGT) && (mean_adc <= ADC_MAX_TGT)) {
			return;
		}
		if (mean_adc > ADC_MAX_TGT) {
			lo = mid + 1U;
		} else {
			hi = mid - 1U;
		}
	}


//	while (((mean_adc < ADC_MIN_TGT) || (mean_adc > ADC_MAX_TGT))
//			&& (dac_tick <= DAC_MAX)) {
//		sum_adc = 0UL;
//		set_dac_channel(hspi, cs_port, cs_pin, channel, dac_tick);
//		HAL_Delay(SETTLE_MS);
//		for (uint8_t i = 0U; i < ADC_MEAN_SAMPLES; i++) {
//			sum_adc += adc1_data_[sens];
//		}
//		mean_adc = (uint32_t) (sum_adc / (uint32_t) ADC_MEAN_SAMPLES);
//
//		dac_tick++;
//	}

//	char msg[80];
//	if (dac_tick >= DAC_MAX_TICK_DEFAULT) {
//		(void) snprintf(msg, sizeof(msg), "CHANNEL: %u - ERROR CALIBRATION\r\n", (unsigned) channel);
//	} else {
//		(void) snprintf(msg, sizeof(msg), "CHANNEL: %u - CALIBRATION SUCCESS - DAC TICK: %u\r\n", (unsigned) channel, (unsigned) dac_tick);
//	}
//	UART_Print(msg);
}

