/*
 * parameters_func.h
 *
 *  Created on: Jan 23, 2026
 *      Author: emili
 */

#ifndef EXPORTED_PARAMETERS_FUNC_H_
#define EXPORTED_PARAMETERS_FUNC_H_

#include "arm_math.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "definition.h"

#define FLOAT_LEN          4
#define UINT16_T_LEN       2

void set_gain(const uint8_t *RxData);

float32_t get_gain(const uint8_t *RxData);

void set_delta(const uint8_t *RxData);

float32_t get_delta(const uint8_t *RxData);

void set_tau_1(const uint8_t *RxData);

float32_t get_tau_1(const uint8_t *RxData);

void set_tau_2(const uint8_t *RxData);

float32_t get_tau_2(const uint8_t *RxData);

void set_tau_th(const uint8_t *RxData);

float32_t get_tau_th(const uint8_t *RxData);

void set_num_cycle(const uint8_t *RxData);

uint16_t get_num_cycle(const uint8_t *RxData);

void set_dsp_th_1(const uint8_t *RxData);

float32_t get_dsp_th_1(const uint8_t *RxData);

void set_dsp_th_2(const uint8_t *RxData);

float32_t get_dsp_th_2(const uint8_t *RxData);

void turn_on_algorithm(const uint8_t *RxData);

void turn_off_algorithm(const uint8_t *RxData);

void turn_on_dsp_algorithm(const uint8_t *RxData);

void turn_off_dsp_algorithm(const uint8_t *RxData);

#endif /* EXPORTED_PARAMETERS_FUNC_H_ */


