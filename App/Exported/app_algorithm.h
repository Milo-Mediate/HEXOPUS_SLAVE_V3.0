/*
 * app_algorithm.h
 *
 *  Created on: Feb 6, 2026
 *      Author: emili
 */

#ifndef EXPORTED_APP_ALGORITHM_H_
#define EXPORTED_APP_ALGORITHM_H_


#include "dynamic_threshold_algorithm_test_diff.h"

typedef enum {
	FILTER_TH_1,
	FILTER_TH_2,
} Signal;

extern Sensor_t * const * const all_sensors;

void dynamic_threshold_algorithm_test();
void reset_thresholds();
void sensor_init();
void threshold_init();

void set_threshold_gain(uint8_t sensor, float32_t gain);
bool get_threshold_gain(uint8_t sensor, float32_t *gain);

void set_sensor_delta(uint8_t sensor, float32_t delta);
bool get_sensor_delta(uint8_t sensor, float32_t *delta);

void set_sensor_tau_1(uint8_t sensor, float32_t tau_1);
bool get_sensor_tau_1(uint8_t sensor, float32_t *tau_1);

void set_sensor_tau_2(uint8_t sensor, float32_t tau_2);
bool get_sensor_tau_2(uint8_t sensor, float32_t *tau_2);

void set_threshold_tau_th(uint8_t sensor, float32_t tau_th);
bool get_threshold_tau_th(uint8_t sensor, float32_t *tau_th);

void set_sensor_num_cycle(uint8_t sensor, uint16_t num_cycle);
bool get_sensor_num_cycle(uint8_t sensor, uint16_t *num_cycle);

void enable_threshold(uint8_t sensor);
void disable_threshold(uint8_t sensor);
void enable_all_threshold();
void disable_all_threshold();

bool check_status(uint8_t sensor);
void build_txdata(uint8_t index, float32_t *data);

float32_t get_plot_data(uint8_t index);

//void sprint_sensors_value(char *buffer, uint8_t value);
#endif /* EXPORTED_APP_ALGORITHM_H_ */
