/*
 * can_parameters.h
 *
 *  Created on: Feb 5, 2026
 *      Author: emili
 */

#ifndef EXPORTED_CAN_PARAMETERS_H_
#define EXPORTED_CAN_PARAMETERS_H_

#include "stm32h5xx_hal.h"
//
//static HAL_StatusTypeDef can_get_float32_parameters(uint8_t command, float32_t value);
//static HAL_StatusTypeDef can_get_uint16_parameters(uint8_t command, uint16_t value);

HAL_StatusTypeDef can_get_gain(const uint8_t *RxData);
HAL_StatusTypeDef can_get_delta(const uint8_t *RxData);
HAL_StatusTypeDef can_get_tau_1(const uint8_t *RxData);
HAL_StatusTypeDef can_get_tau_2(const uint8_t *RxData);
HAL_StatusTypeDef can_get_tau_th(const uint8_t *RxData);
HAL_StatusTypeDef can_get_num_cycle(const uint8_t *RxData);

#endif /* EXPORTED_CAN_PARAMETERS_H_ */
