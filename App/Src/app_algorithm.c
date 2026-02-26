/*
 * app_algorithm.c
 *
 *  Created on: Feb 6, 2026
 *      Author: emili
 */
#include "dynamic_threshold_algorithm_test_diff.h"
#include "app_algorithm.h"
#include "EMA_constant.h"
#include "ADC_function.h"
#include "app_hw_definition.h"

#define TAU_1                      0.001f // TODO: da prendere dalla flash
#define TAU_2                      0.025f  // TODO: da prendere dalla flash
#define TAU_TH                     0.05f
#define DEFAULT_TH_TS              0.02048f
#define DEFAULT_GAIN               1.5f
#define DEFAULT_DELTA              1.0f
#define DEFAULT_NUM_CYCLE          200
#define DEFAULT_TH                 4000
#define DEFAULT_TS                 0.00004f
#define DEFAULT_TH_BUF_SIZE        1024U

#define MAX_NUM_SENS               18


//
//Sensor_t all_sensors[MAX_NUM_SENS];
//Sensor_TH_t all_thresholds[MAX_NUM_SENS];
//Edge_Detector_t edge_det[MAX_NUM_SENS];

static Sensor_t * all_sensors_storage[NUM_SENS];          // <-- scrivibile
Sensor_t * const * const all_sensors = all_sensors_storage; // <-- vista read-only
Sensor_t real_sensors[NUM_SENS/2];
Sensor_t dummy_sensors[NUM_SENS/2];
__attribute__((constructor))
static void fill_sensors_array(void)
{
	for (int i = 0; i < NUM_SENS/2; i++) {
		all_sensors_storage[(2*i)] = &real_sensors[i];
		all_sensors_storage[(2*i) + 1] = &dummy_sensors[i];
	}
}

static Sensor_TH_t * all_thresholds_storage[NUM_SENS];          // <-- scrivibile
Sensor_TH_t * const * const all_thresholds = all_thresholds_storage; // <-- vista read-only
Sensor_TH_t real_thresholds[NUM_SENS/2];
Sensor_TH_t dummy_thresholds[NUM_SENS/2];
__attribute__((constructor))
static void fill_thresholds_array(void)
{
	for (int i = 0; i < NUM_SENS/2; i++) {
		all_thresholds_storage[(2*i)] = &real_thresholds[i];
		all_thresholds_storage[(2*i) + 1] = &dummy_thresholds[i];
	}
}


Edge_Detector_t edge_det[NUM_SENS];

void dynamic_threshold_algorithm_test() {
	for (uint8_t i = 0; i < NUM_SENS; i++) {
		float32_t new_val = read_adc(ADC_1, i);
		Sensor_t *sensor = all_sensors[i];
		Sensor_TH_t *threshold = all_thresholds[i];
		Edge_Detector_t *ed = &edge_det[i];
		update_sensor(new_val, sensor, threshold, ed);
	}
}

void reset_thresholds()
{
	all_thresholds[3]->value = all_sensors[3]->filter_2 + all_sensors[3]->delta;
	all_thresholds[2]->value = all_sensors[2]->filter_2 + all_sensors[2]->delta;
}

void sensor_init() {
	for (uint8_t i = 0U; i < NUM_SENS; i++) {
		all_sensors[i]->delta = DEFAULT_DELTA;
		all_sensors[i]->num_cycle_max = DEFAULT_NUM_CYCLE;
		all_sensors[i]->k_1 = EMA_GetAlpha(TAU_1, DEFAULT_TS);
		all_sensors[i]->k_2 = EMA_GetAlpha(TAU_2, DEFAULT_TS);
	}
}

void threshold_init() {
	for (uint8_t i = 0U; i < NUM_SENS; i++) {
		all_thresholds[i]->buffer_len = DEFAULT_TH_BUF_SIZE;
		all_thresholds[i]->gain = DEFAULT_GAIN;
		all_thresholds[i]->value = DEFAULT_TH;
		all_thresholds[i]->k = EMA_GetAlpha(TAU_TH, DEFAULT_TH_TS);
	}
}

void set_threshold_gain(uint8_t sensor, float32_t gain) {
	if (sensor > sizeof(all_thresholds) - 1)
		return;
//	if (gain < GAIN_MIN || gain > GAIN_MAX) return;
	all_thresholds[sensor]->gain = gain;
}

bool get_threshold_gain(uint8_t sensor, float32_t *gain) {
	if (sensor > sizeof(all_thresholds) - 1)
		return false;
	if (!gain)
		return false;
	*gain = all_thresholds[sensor]->gain;
	return true;
}

void set_sensor_delta(uint8_t sensor, float32_t delta) {
	if (sensor > sizeof(all_sensors) - 1)
		return;
	all_sensors[sensor]->delta = delta;
}
bool get_sensor_delta(uint8_t sensor, float32_t *delta) {
	if (sensor > sizeof(all_sensors) - 1)
		return false;
	if (!delta)
		return false;
	*delta = all_sensors[sensor]->delta;
	return true;
}

void set_sensor_tau_1(uint8_t sensor, float32_t tau_1) {
	if (sensor > sizeof(all_sensors) - 1)
		return;
	all_sensors[sensor]->k_1 = EMA_GetAlpha(tau_1, DEFAULT_TS);
}
bool get_sensor_tau_1(uint8_t sensor, float32_t *tau_1) {
	if (sensor > sizeof(all_sensors) - 1)
		return false;
	if (!tau_1)
		return false;
	*tau_1 = all_sensors[sensor]->k_1;
	return true;
}

void set_sensor_tau_2(uint8_t sensor, float32_t tau_2) {
	if (sensor > sizeof(all_sensors) - 1)
		return;
	all_sensors[sensor]->k_2 = EMA_GetAlpha(tau_2, DEFAULT_TS);
}
bool get_sensor_tau_2(uint8_t sensor, float32_t *tau_2) {
	if (sensor > sizeof(all_sensors) - 1)
		return false;
	if (!tau_2)
		return false;
	*tau_2 = all_sensors[sensor]->k_2;
	return true;
}

void set_threshold_tau_th(uint8_t sensor, float32_t tau_th) {
	if (sensor > sizeof(all_thresholds) - 1)
		return;
	all_thresholds[sensor]->k = EMA_GetAlpha(tau_th, DEFAULT_TH_TS);
}
bool get_threshold_tau_th(uint8_t sensor, float32_t *tau_th) {
	if (sensor > sizeof(all_thresholds) - 1)
		return false;
	if (!tau_th)
		return false;
	*tau_th = all_thresholds[sensor]->k;
	return true;
}

void set_sensor_num_cycle(uint8_t sensor, uint16_t num_cycle) {
	if (sensor > sizeof(all_sensors) - 1)
		return;
}
bool get_sensor_num_cycle(uint8_t sensor, uint16_t *num_cycle) {
	if (sensor > sizeof(all_sensors) - 1)
		return false;
	if (!num_cycle)
		return false;
	*num_cycle = all_sensors[sensor]->num_cycle_max;
	return true;
}

void enable_threshold(uint8_t sensor) { //ToDo: modificare per diagnostica
//	if (sensor > sizeof(all_sensors) - 1)
//		return;
	all_thresholds[sensor]->enable = true;

}
void disable_threshold(uint8_t sensor) { //ToDo: modificare per diagnostica
	if (sensor > sizeof(all_sensors) - 1)
		return;
	all_thresholds[sensor]->enable = false;
}

void enable_all_threshold() {
	for(uint8_t sens = 0; sens < NUM_SENS; sens++) {
		enable_threshold(sens);
	}
}

void disable_all_threshold() {
	for(uint8_t sens = 0; sens < NUM_SENS; sens++) {
		disable_threshold(sens);
	}
}

bool check_status(uint8_t sensor) {
	bool status = all_sensors[sensor]->status;
	return status;
}

void build_txdata_alg(uint8_t index, float32_t *data)
{
	data[0] = real_sensors[index].filter_2;
	data[1] = real_thresholds[index].value;
	data[2] = dummy_sensors[index].filter_2;
	data[3] = dummy_thresholds[index].value;
	data[4] = 0.0f;
	data[5] = 0.0f;
	data[6] = 0.0f;
}

//void sprint_sensors_value(char *buffer, uint8_t value)
//{
//	switch (value) {
//	case FILTER_TH_1:
//		sprintf(buffer, "%f;%f;%f;%f\r\n", all_sensors[0]->filter_1, all_sensors[1]->filter_1, all_sensors[2]->filter_1, all_sensors[3]->filter_1);
//		break;
//	case FILTER_TH_2:
//		break;
//	}
////	UART_Print(data);
//}

