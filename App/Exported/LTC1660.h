/*
 * LTC1660.h
 *
 *  Created on: Jul 2, 2025
 *      Author: gsant
 */

#ifndef SRC_LTC1660_H_
#define SRC_LTC1660_H_

#include "global.h"

//#include "event_manager.h"
typedef enum {
    DAC_CAL_OK = 0,
    DAC_CAL_PARAM_ERR,
    DAC_CAL_TIMEOUT,
    DAC_CAL_OUT_OF_RANGE
} dac_cal_status_t;

void set_dac_channel(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin, uint8_t channel, uint16_t value);
extern void auto_set_dac_value(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin, uint8_t sens);

#endif /* SRC_LTC1660_H_ */
