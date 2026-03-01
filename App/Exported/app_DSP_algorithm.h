/*
 * DSP_algorithm.h
 *
 *  Created on: Jul 8, 2025
 *      Author: emili
 */

#ifndef EXPORTED_APP_DSP_ALGORITHM_H_
#define EXPORTED_APP_DSP_ALGORITHM_H_

#include "arm_math.h"
#include "dsp_threshold.h"
#include "filter_goertzel.h"
#include "app_hw_definition.h"
#include <stdbool.h>

#define DSP_TAU                     0.002f
#define DEFAULT_DSP_TS              0.000025f
#define DEFAULT_DSP_TH_BUF_SIZE     512
#define DEFAULT_DSP_GAIN_1          1.7f
#define DEFAULT_DSP_GAIN_2          45.0f

extern G_Filter_t dsp_sensors[NUM_DSP];
extern DSP_TH_t dsp_thresholds[NUM_DSP];

void dsp_init();
void dsp_threshold_init();

void set_sensor_dsp_th_1(uint8_t sens, float32_t new_th);
float32_t get_sensor_dsp_th_1(uint8_t sens);
void set_sensor_dsp_th_2(uint8_t sens, float32_t new_th);
float32_t get_sensor_dsp_th_2(uint8_t sens);

G_Filter_error DSP_algorithm();
void DSP_threshold();
extern void rms_output();
DSP_TH_status set_outdated_dsp_th(uint8_t sens);
extern DSP_Status check_DSP_status(uint8_t sensor);
bool check_all_DSP_status_1();
bool check_all_DSP_status_2();
void enable_dsp_threshold(uint8_t sensor);
void disable_dsp_threshold(uint8_t sensor);
void enable_all_dsp_threshold();
void disable_all_dsp_threshold();
void build_txdata_DSP_alg(uint8_t index, float32_t *data);
void print_DSP(uint8_t sens);
#endif /* SRC_DSP_ALGORITHM_H_ */
