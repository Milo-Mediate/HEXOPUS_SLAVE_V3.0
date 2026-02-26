/*
 * LTC1660.h
 *
 *  Created on: Jul 2, 2025
 *      Author: gsant
 */

#ifndef SRC_LTC1660_H_
#define SRC_LTC1660_H_
#include <stdint.h>
#include <stdbool.h>
//#include "global.h"
//#include "stm32h5xx_hal_def.h"
//#include "main.h"
#include "stm32h5xx_hal.h"
#define N_CH               8U

//#include "event_manager.h"
typedef enum {
	DAC_CAL_TO_DO = 0,
    DAC_CAL_OK,
    DAC_CAL_CHANNEL_ERR,
	DAC_CAL_ERR,
    DAC_CAL_TIMEOUT,
    DAC_CAL_OUT_OF_RANGE
} dac_cal_status_t;

typedef struct {
	uint16_t values[N_CH];
	dac_cal_status_t status[N_CH];
	SPI_HandleTypeDef *hspi;
	GPIO_TypeDef *cs_port;
	uint16_t cs_pin;
	uint8_t num_channels;
	uint8_t ID;
} DAC_t;

bool set_dac_channel(DAC_t *dac, uint8_t channel, uint16_t new_value);
dac_cal_status_t auto_set_channel_value(DAC_t *dac, uint8_t channel);
uint16_t get_dac_channel (const DAC_t *dac, uint8_t channel);
dac_cal_status_t calibration_dac(DAC_t *dac);
dac_cal_status_t dac_init(SPI_HandleTypeDef *hspi, DAC_t *dac, GPIO_TypeDef *port, uint16_t pin, uint8_t num_ch, uint8_t id);

#endif /* SRC_LTC1660_H_ */
