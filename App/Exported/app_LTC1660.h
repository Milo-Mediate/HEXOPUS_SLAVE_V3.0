/*
 * app_LTC1660.h
 *
 *  Created on: Feb 5, 2026
 *      Author: emili
 */

#ifndef EXPORTED_APP_LTC1660_H_
#define EXPORTED_APP_LTC1660_H_

#include "LTC1660.h"
#include "app_hw_definition.h"
#include "spi.h"

extern DAC_t dacs[MAX_DAC_INSTALLED];

void app_dac_init();
uint16_t app_get_dac_channel(uint8_t dev, uint8_t channel);
dac_cal_status_t calibration_init();
dac_cal_status_t app_calibration_dac(DAC_t *dac);
void app_set_dac_channel(uint8_t dev, uint8_t channel, uint16_t value);

//uint16_t get_dac_2_channel(uint8_t channel);
#endif /* EXPORTED_APP_LTC1660_H_ */
