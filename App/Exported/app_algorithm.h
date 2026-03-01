/*
 * app_algorithm.h
 *
 *  Created on: Feb 6, 2026
 *      Author: emili
 */

#ifndef EXPORTED_APP_ALGORITHM_H_
#define EXPORTED_APP_ALGORITHM_H_

#define TAU_1                      0.001f // TODO: da prendere dalla flash
#define TAU_2                      0.025f  // TODO: da prendere dalla flash
#define TAU_TH                     0.05f
#define DEFAULT_TH_TS              0.02048f
#define DEFAULT_GAIN               1.75f
#define DEFAULT_DELTA              2.0f
#define DEFAULT_NUM_CYCLE          200
#define DEFAULT_TH                 4000
#define DEFAULT_TS                 0.00004f
#define DEFAULT_TH_BUF_SIZE        1024U

#include "dynamic_threshold_algorithm_test_diff.h"
#include "app_hw_definition.h"

extern Sensor_t real_sensors[NUM_SENS/2];
extern Sensor_t dummy_sensors[NUM_SENS/2];
extern Sensor_TH_t real_thresholds[NUM_SENS/2];
extern Sensor_TH_t dummy_thresholds[NUM_SENS/2];

typedef enum {
	FILTER_TH_1,
	FILTER_TH_2,
} Signal;

extern Sensor_t * const * const all_sensors;

void dynamic_threshold_algorithm_test();
void reset_thresholds(uint8_t sensor);
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

bool check_sensor_status(uint8_t sensor);
bool check_all_sensors_status();
void build_txdata_alg(uint8_t index, float32_t *data);

float32_t get_plot_data(uint8_t index);

//void sprint_sensors_value(char *buffer, uint8_t value);
#endif /* EXPORTED_APP_ALGORITHM_H_ */
