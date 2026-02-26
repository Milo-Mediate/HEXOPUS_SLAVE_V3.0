/*
 * DSP_algorithm.h
 *
 *  Created on: Jul 8, 2025
 *      Author: emili
 */

#ifndef EXPORTED_APP_DSP_ALGORITHM_H_
#define EXPORTED_APP_DSP_ALGORITHM_H_

#include "arm_math.h"
#include "filter_goertzel.h"
#include <stdbool.h>

void dsp_init();
void dsp_threshold_init();
G_Filter_error DSP_algorithm();
void DSP_threshold();
extern void rms_output();
extern DSP_Status check_DSP_status(uint8_t sensor);
void build_txdata_DSP_alg(uint8_t index, float32_t *data);
void print_DSP(uint8_t sens);
#endif /* SRC_DSP_ALGORITHM_H_ */
