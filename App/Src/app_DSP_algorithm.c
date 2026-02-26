#include <app_DSP_algorithm.h>
#include "serial_manager.h"
#include "can_manager.h"
#include "EMA_constant.h"
#include "dsp_threshold.h"
#include "app_hw_definition.h"
#include "ADC_function.h"
#include "logging.h"

#define DSP_TAU                     0.002f
#define DEFAULT_DSP_TS              0.000025f
#define DEFAULT_DSP_TH_BUF_SIZE     512
#define DEFAULT_DSP_GAIN_1          1.7f
#define DEFAULT_DSP_GAIN_2          47.5f

static G_Filter_t dsp_sensors[NUM_DSP];
static DSP_TH_t dsp_thresholds[NUM_DSP];

void dsp_init() {
	for (uint8_t i = 0U; i < NUM_DSP; i++) {
		dsp_sensors[i].k_dsp = EMA_GetAlpha(DSP_TAU, DEFAULT_DSP_TS);
	}
}

void dsp_threshold_init() {
	for (uint8_t i = 0U; i < NUM_DSP; i++) {
		dsp_thresholds[i].buffer_len = DEFAULT_DSP_TH_BUF_SIZE;
		dsp_thresholds[i].sum = 0;
		dsp_thresholds[i].max = 0;
		dsp_thresholds[i].gain_1 = DEFAULT_DSP_GAIN_1;
		dsp_thresholds[i].gain_2 = DEFAULT_DSP_GAIN_2;
		dsp_thresholds[i].is_updated = true;
		dsp_thresholds[i].enable = false;
	}
}

void set_sensor_dsp_th_1(uint8_t sens, float32_t new_th)
{
	dsp_thresholds[sens].value_1 = new_th;
}

float32_t get_sensor_dsp_th_1(uint8_t sens)
{
	return dsp_thresholds[sens].value_1;
}

void set_sensor_dsp_th_2(uint8_t sens, float32_t new_th)
{
	dsp_thresholds[sens].value_2 = new_th;
}

float32_t get_sensor_dsp_th_2(uint8_t sens)
{
	return dsp_thresholds[sens].value_2;
}

G_Filter_error DSP_algorithm() {
	for (uint8_t i = 0; i < NUM_DSP; i++)
	{
		float32_t new_val = read_adc(ADC_2, i);
		if (goertzel_10kHz(&dsp_sensors[i], new_val) != FILTER_OK)
			return FILTER_ERROR;
		dsp_sensors[i].pow_fil = (dsp_sensors[i].power) * (dsp_sensors[i].k_dsp) + (dsp_sensors[i].pow_fil) * (1.0f - dsp_sensors[i].k_dsp);
		dsp_sensors[i].index = (dsp_sensors[i].index + 1) % NUM_SAMPLES_FILTER;
		if (dsp_sensors[i].pow_fil > dsp_thresholds[i].value_1)
		{
			if (dsp_sensors[i].pow_fil > dsp_thresholds[i].value_2)
			{
				dsp_sensors[i].status = ABOVE_TH_2;
			} else {
				dsp_sensors[i].status = BETWEEN_THS;
			}
		}
		else
		{
			dsp_sensors[i].status = BELOW_TH_1;
		}

		if (!dsp_thresholds[i].is_updated)
			update_dsp_th(&dsp_thresholds[i], dsp_sensors[i].pow_fil);
	}
	return FILTER_OK;
}

DSP_TH_status set_outdated_dsp_th(uint8_t sens) {
	if(sens >= NUM_DSP)
		return DSP_TH_OUT_OF_RANGE;
	dsp_thresholds[sens].is_updated = false;
	return DSP_TH_OK;
}

DSP_Status check_DSP_status(uint8_t sensor) {
	DSP_Status status = dsp_sensors[sensor].status;
	return status;
}

void enable_dsp_threshold(uint8_t sensor) { //ToDo: modificare per diagnostica
//	if (sensor > sizeof(all_sensors) - 1)
//		return;
	dsp_thresholds[sensor].enable = true;

}

void disable_dsp_threshold(uint8_t sensor) { //ToDo: modificare per diagnostica
//	if (sensor > sizeof(all_sensors) - 1)
//		return;
	dsp_thresholds[sensor].enable = false;

}

void enable_all_dsp_threshold() {
	for(uint8_t sens = 0; sens < NUM_DSP; sens++)
	{
		enable_dsp_threshold(sens);
	}
}

void disable_all_dsp_threshold() {
	for(uint8_t sens = 0; sens < NUM_DSP; sens++)
	{
		disable_dsp_threshold(sens);
	}
}

/**
 * @brief Evaluate RMS vs thresholds with hysteresis (pre-stop + stop).
 * @note
 * - Uses counters ::count_pre_stop_ / ::count_pre_start_ / ::count_start_.
 * - Updates ::status_DSP_pre_stop_ and ::status_DSP_stop_.
 */
void rms_output(void) {
//	float32_t val;
//
//	for (uint8_t i = 0U; i < NUM_SENS; i++) {
//		val = pow_fil_[i];
//
//		/* Pre-stop evaluation with debounce */
//		if ((val > threshold_DSP_1st_[i]) && (flag_th_DSP_[i] != 0U)) {
//			if (count_pre_stop_[i] >= count_dim_stop_) {
//				status_DSP_pre_stop_[i] = 1U;
//			} else {
//				count_pre_stop_[i]++;
//			}
//			count_pre_start_[i] = 0U;
//		} else {
//			if (count_pre_start_[i] >= count_dim_start_) {
//				status_DSP_pre_stop_[i] = 0U;
//			} else {
//				count_pre_start_[i]++;
//			}
//			count_pre_stop_[i] = 0U;
//		}
//
//		/* Stop evaluation with debounce */
//		if ((val > threshold_DSP_2nd_[i]) && (flag_th_DSP_[i] != 0U)) {
//			status_DSP_stop_[i] = 1U;
//			count_start_[i] = 0U;
//		} else {
//			if (count_start_[i] >= count_dim_start_) {
//				status_DSP_stop_[i] = 0U;
//			} else {
//				count_start_[i]++;
//			}
//		}
//	}
}

/**
 * @brief Consolidate DSP status across sensors and send CAN events on edges.
 * @details
 * - Excludes sensors 1 and 5 from evaluation (policy-based).
 * - Sends CMD_PRE_STOP/CMD_PRE_START and CMD_STOP/CMD_START on state changes.
 */
void control_DSP_status(void) {
//	uint8_t result_pre_stop = 0U;
//	uint8_t result_stop = 0U;

//	for (uint8_t i = 0U; i < NUM_SENS; i++) {
//		if (status_DSP_pre_stop_[i] != 0U) {
//			result_pre_stop++;
//		}
//		if (status_DSP_stop_[i] != 0U) {
//			result_stop++;
//		}
//	}
//
//	/* PRE-STOP edges */
//	if (result_pre_stop != 0U) {
//		if (flag_msg_DSP_pre_stop_sent_ == 0U) {
//
//			flag_msg_DSP_pre_stop_sent_ = 1U;
//
//			TxData_[0] = CMD_PRE_STOP;
//			FDCANTx(SLAVE_ID_, 1U);
//		}
//	} else {
//		if (flag_msg_DSP_pre_stop_sent_ != 0U) {
//
//			flag_msg_DSP_pre_stop_sent_ = 0U;
//
//			TxData_[0] = CMD_PRE_START;
//			FDCANTx(SLAVE_ID_, 1U);
//		}
//	}
//
//	/* STOP edges */
//	if (result_stop != 0U) {
//		if (flag_msg_DSP_stop_sent_ == 0U) {
//			flag_msg_DSP_stop_sent_ = 1U;
//
//			TxData_[0] = CMD_STOP;
//			FDCANTx(SLAVE_ID_, 1U);
//		}
//	} else {
//		if (flag_msg_DSP_stop_sent_ != 0U) {
//			flag_msg_DSP_stop_sent_ = 0U;
//
//			TxData_[0] = CMD_START;
//			FDCANTx(SLAVE_ID_, 1U);
//		}
//	}
}

void build_txdata_DSP_alg(uint8_t index, float32_t *data) {
	data[4] = dsp_sensors[index].pow_fil;
	data[5] = dsp_thresholds[index].value_1;
	data[6] = dsp_thresholds[index].value_2;
}

void print_DSP(uint8_t sens) {
	char data_to_send[100];
	sprintf(data_to_send, "dsp_%d = %f\r\n", sens, dsp_sensors[sens].pow_fil);
	UART_Print(data_to_send);
}
