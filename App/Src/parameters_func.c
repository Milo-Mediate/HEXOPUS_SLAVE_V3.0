/*
 * parameters_func.c
 *
 *  Created on: Jan 23, 2026
 *      Author: emili
 */



#include "parameters_func.h"
#include "app_algorithm.h"
#include "app_DSP_algorithm.h"
#include "can_manager.h"
#include "event_manager.h"

//typedef struct {
//	uint8_t sensor;
//	float32_t gain;
//} SensorGain_t;

//bool gain_from_buffer(SensorGain_t *sg, const uint8_t *RxData) {
//	if (!RxData || !sg)
//		return false;
//
//	sg->sensor = RxData[1];
//	memcpy(&sg->gain, &RxData[2], sizeof(sg->gain));
//
//	return true;
//}
//
//void cmd_set_gain(const uint8_t *RxData) {
//	SensorGain_t sg;
//	if (!gain_from_buffer(&sg, RxData)) {
//		write_event();
//		serial_log();
//		return;
//	}
//
//	thresholds[sg.sensor].gain = sg.gain;
//	serial_log();
//}


void set_gain(const uint8_t *RxData) {
	uint8_t sensor = RxData[1];
	float32_t new_gain = 0;
	memcpy(&new_gain, &RxData[2], sizeof(float32_t));
//	thresholds[sensor].gain = new_gain;
	set_threshold_gain(sensor, new_gain);
}

float32_t get_gain(const uint8_t *RxData) {
	uint8_t sensor = RxData[1];
	float32_t act_gain;
	if (!get_threshold_gain(sensor, &act_gain))
	{
		send_event(EVENT_FAULT_GET_GAIN);
	}
	return act_gain;
}

//void get_gain(const uint8_t *RxData) {
//	uint8_t command = RxData[0];
//	uint8_t sensor = RxData[1];
//	uint8_t TxData[5] = { command };
//	memcpy(&TxData[1], &thresholds[sensor].gain, FLOAT_LEN);
//	FDCANTx(SLAVE_ID_, TxData, sizeof(TxData));
//}

void set_delta(const uint8_t *RxData) {
	uint8_t sensor = RxData[1];
	float32_t new_delta = 0;
	memcpy(&new_delta, &RxData[2], sizeof(float32_t));
//	sensors[sensor].delta = new_delta;
	set_sensor_delta(sensor, new_delta);
}

float32_t get_delta(const uint8_t *RxData) {
	uint8_t sensor = RxData[1];
	float32_t act_delta;
	if (!get_sensor_delta(sensor, &act_delta))
	{
		send_event(EVENT_FAULT_GET_DELTA);
	}
	return act_delta;
}

//void get_delta(const uint8_t *RxData) {
//	uint8_t command = RxData[0];
//	uint8_t sensor = RxData[1];
//	uint8_t TxData[5] = { command };
//	memcpy(&TxData[1], &sensors[sensor].delta, FLOAT_LEN);
//	FDCANTx(SLAVE_ID_, TxData, sizeof(TxData));
//}

void set_tau_1(const uint8_t *RxData) {
	uint8_t sensor = RxData[1];
	float32_t new_tau_1 = 0;
	memcpy(&new_tau_1, &RxData[2], sizeof(float32_t));
//	sensors[sensor].k_1 = EMA_GetAlpha(new_tau_1, TS);
	set_sensor_tau_1(sensor, new_tau_1);
}

float32_t get_tau_1(const uint8_t *RxData) {
	uint8_t sensor = RxData[1];
	float32_t act_tau_1;
	if (!get_sensor_tau_1(sensor, &act_tau_1))
	{
		send_event(EVENT_FAULT_GET_TAU_1);
	}
	return act_tau_1;
}

//void get_tau_1(const uint8_t *RxData) {
//	uint8_t command = RxData[0];
//	uint8_t sensor = RxData[1];
//	uint8_t TxData[5] = { command };
//	memcpy(&TxData[1], &sensors[sensor].k_1, FLOAT_LEN);
//	FDCANTx(SLAVE_ID_, TxData, sizeof(TxData));
//}

void set_tau_2(const uint8_t *RxData) {
	uint8_t sensor = RxData[1];
	float32_t new_tau_2 = 0;
	memcpy(&new_tau_2, &RxData[2], sizeof(float32_t));
//	sensors[sensor].k_2 = EMA_GetAlpha(new_tau_2, TS);
	set_sensor_tau_2(sensor, new_tau_2);
}

float32_t get_tau_2(const uint8_t *RxData) {
	uint8_t sensor = RxData[1];
	float32_t act_tau_2;
	if (!get_sensor_tau_2(sensor, &act_tau_2))
	{
		send_event(EVENT_FAULT_GET_TAU_2);
	}
	return act_tau_2;
}

//void get_tau_2(const uint8_t *RxData) {
//	uint8_t command = RxData[0];
//	uint8_t sensor = RxData[1];
//	uint8_t TxData[5] = { command };
//	memcpy(&TxData[1], &sensors[sensor].k_2, FLOAT_LEN);
//	FDCANTx(SLAVE_ID_, TxData, sizeof(TxData));
//}

void set_tau_th(const uint8_t *RxData) {
	uint8_t sensor = RxData[1];
	float32_t new_tau_th = 0;
	memcpy(&new_tau_th, &RxData[2], sizeof(float32_t));
//	thresholds[sensor].k = EMA_GetAlpha(new_tau_th, 0.02048f);
	set_threshold_tau_th(sensor, new_tau_th);
}

float32_t get_tau_th(const uint8_t *RxData) {
	uint8_t sensor = RxData[1];
	float32_t act_tau_th;
	if (!get_threshold_tau_th(sensor, &act_tau_th))
	{
		send_event(EVENT_FAULT_GET_TAU_TH);
	}
	return act_tau_th;
}

//void get_tau_th(const uint8_t *RxData) {
//	uint8_t command = RxData[0];
//	uint8_t sensor = RxData[1];
//	uint8_t TxData[5] = { command };
//	memcpy(&TxData[1], &thresholds[sensor].k, FLOAT_LEN);
//	FDCANTx(SLAVE_ID_, TxData, sizeof(TxData));
//}

void set_num_cycle(const uint8_t *RxData) {
	uint8_t sensor = RxData[1];
	uint16_t new_num_cycle = 0;
	memcpy(&new_num_cycle, &RxData[2], sizeof(uint16_t));
//	sensors[sensor].num_cycle_max = new_num_cycle;
//	sensors[sensor].num_cycle_count = 0;
	set_sensor_num_cycle(sensor, new_num_cycle);
}

uint16_t get_num_cycle(const uint8_t *RxData) {
	uint8_t sensor = RxData[1];
	uint16_t act_num_cycle;
	if (!get_sensor_num_cycle(sensor, &act_num_cycle))
	{
		send_event(EVENT_FAULT_GET_NUM_CYCLE);
	}
	return act_num_cycle;
}

void set_dsp_th_1(const uint8_t *RxData)
{
	uint8_t sensor = RxData[1];
	float32_t dsp_th_1;
	memcpy(&dsp_th_1, &RxData[2], sizeof(float32_t));
	set_sensor_dsp_th_1(sensor, dsp_th_1);
}

float32_t get_dsp_th_1(const uint8_t *RxData)
{
	uint8_t sensor = RxData[1];
	return get_sensor_dsp_th_1(sensor);

}

void set_dsp_th_2(const uint8_t *RxData)
{
	uint8_t sensor = RxData[1];
	float32_t dsp_th_2;
	memcpy(&dsp_th_2, &RxData[2], sizeof(float32_t));
	set_sensor_dsp_th_2(sensor, dsp_th_2);
}

float32_t get_dsp_th_2(const uint8_t *RxData)
{
	uint8_t sensor = RxData[1];
	return get_sensor_dsp_th_2(sensor);
}

void turn_on_algorithm(const uint8_t *RxData)
{
	uint8_t sensor = RxData[1];
	enable_threshold(sensor);
}

void turn_off_algorithm(const uint8_t *RxData)
{
	uint8_t sensor = RxData[1];
	disable_threshold(sensor);

}

void turn_on_dsp_algorithm(const uint8_t *RxData)
{
	uint8_t sensor = RxData[1];
	enable_dsp_threshold(sensor);
}

void turn_off_dsp_algorithm(const uint8_t *RxData)
{
	uint8_t sensor = RxData[1];
	disable_dsp_threshold(sensor);
}


