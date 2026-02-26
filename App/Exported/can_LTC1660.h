/*
 * can_LTC1660.h
 *
 *  Created on: Feb 4, 2026
 *      Author: emili
 */

#ifndef EXPORTED_CAN_LTC1660_H_
#define EXPORTED_CAN_LTC1660_H_

#include "stm32h5xx_hal.h"
#include "LTC1660.h"
#include "definition.h"

HAL_StatusTypeDef can_get_dac_1_channel(const uint8_t *RxData);
HAL_StatusTypeDef can_get_dac_2_channel(const uint8_t *RxData);
dac_cal_status_t can_auto_set_dac_1_channel(const uint8_t *RxData);
dac_cal_status_t can_auto_set_dac_2_channel(const uint8_t *RxData);
void can_set_dac_1_channel(const uint8_t *RxData);
void can_set_dac_2_channel(const uint8_t *RxData);
#endif /* EXPORTED_CAN_LTC1660_H_ */
