/*
 * dynamic_threshold_algorithm_test_diff.h
 *
 *  Created on: Feb 6, 2026
 *      Author: emili
 */

#ifndef EXPORTED_DYNAMIC_THRESHOLD_ALGORITHM_TEST_DIFF_H_
#define EXPORTED_DYNAMIC_THRESHOLD_ALGORITHM_TEST_DIFF_H_

#include "arm_math.h"

#include <stdbool.h>


#define EDGE_DETECTOR_BUFFER_SIZE 100

typedef struct {
	float32_t filter_1; //ist_fil_val_
	float32_t filter_2; //sec_fil_val_
	bool      status; // status_sensor_stop_
	float32_t delta; //deltas_
	uint16_t  num_cycle_max; //num_cycles_sizes_
	uint16_t  num_cycle_count; //num_cycles_cont_
	float32_t k_1; //ist_k_
	float32_t k_2; //sec_k_
	uint16_t  dac_val;

}Sensor_t;

typedef struct {
	uint16_t  buffer_len; //TH1_buffs_sizes_
	uint16_t  count; //TH1_cont_
	float32_t sum; //TH1_sums_
	float32_t max; //TH1_max_
	float32_t value; //thresholds_
	float32_t gain; //gains_
	float32_t k; //th_k_
	bool      enable; //flag_th_sensor_
}Sensor_TH_t;

typedef struct {
	float32_t value; //der_EMA_
	float32_t buffer[EDGE_DETECTOR_BUFFER_SIZE]; //der_buff_
	uint8_t   delay_max; //dim_decim_
	uint8_t   count_delay; //count_decim_
	uint16_t  count_t; //count_t_blind_
	uint16_t  t_blind; //dim_t_blind_
	float32_t threshold; //threshold_edge_detector_
	bool      status; //flag_edge_detection_

}Edge_Detector_t;


extern void update_sensor(float32_t new_val, Sensor_t *sensor, Sensor_TH_t *threshold,
		Edge_Detector_t *edge_det);
extern void dynamic_threshold_algorithm_test();
extern float32_t EMA_GetAlpha(float32_t tau, float32_t ts);

#endif /* EXPORTED_DYNAMIC_THRESHOLD_ALGORITHM_TEST_DIFF_H_ */
