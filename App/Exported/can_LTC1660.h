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

HAL_StatusTypeDef can_get_dac_1_channel(uint8_t channel);
HAL_StatusTypeDef can_get_dac_2_channel(uint8_t channel);

#endif /* EXPORTED_CAN_LTC1660_H_ */
