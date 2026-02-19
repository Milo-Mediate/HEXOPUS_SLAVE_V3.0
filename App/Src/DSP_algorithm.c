#include "DSP_algorithm.h"
#include "filter_goertzel.h"
#include "serial_manager.h"
#include "can_manager.h"
#include "EMA_constant.h"
#include "dsp_threshold.h"
#include "app_hw_definition.h"

static G_Filter_t dsp_sensors[NUM_DSP];
static DSP_TH_t dsp_thresholds[NUM_DSP];

G_Filter_error DSP_algorithm() {
	for (uint8_t i = 0; i < NUM_DSP; i++)
	{
		float32_t new_val = read_adc(ADC_2, i);
		if (goertzel_10kHz(&dsp, new_val) != FILTER_OK)
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

		if (dsp_thresholds[i].flag)
			update_dsp_th(&dsp_thresholds[i], dsp_sensors[i].pow_fil);
	}
	return FILTER_OK;
}


void DSP_algorithm(void) {
	float32_t new_sample;
	float32_t new_power[NUM_DSP] = { 0.0f };
	for (uint8_t i = 0U; i < NUM_DSP; i++) {
		new_sample = (float32_t) adc1_data_[i];
		/* Sliding Goertzel using sample leaving the window */
		new_power[i] = goertzel_10kHz(i, new_sample, filter_g_buffer_[i][filter_g_index_]);
		/* Update circular buffer for next step */
		filter_g_buffer_[i][filter_g_index_] = new_sample;
		/* Compute RMS (√(power / N)); keep 100.0f as normalization factor */
		//(void) arm_sqrt_f32(power_[i] / 100.0f, &new_rms);
		/* EMA on RMS with configured smoothing factor */
		pow_fil_[i] = (new_power[i]) * (dsp_k_[i])
				+ (pow_fil_[i] * (1 - dsp_k_[i]));
		/* While thresholds are not being (re)calculated, feed TH2 with EMA */
		if (flag_calculate_th_dsp_ == 0U) {
			put_TH2(i, pow_fil_[i]);
		}
	}
	/* Advance global index of the filter buffer */
	filter_g_index_ = (uint32_t) ((filter_g_index_ + 1U) % NUM_SAMPLES_FILTER);
}

/**
 * @brief Compute adaptive thresholds (pre-stop & stop) from TH2 buffers.
 * @note
 * - pre_stop = mean + 1.7f * (max - mean)
 * - stop     = 2.0f * pre_stop
 * - Resets TH2 after each calculation.
 */
void DSP_threshold(void) {
	/* If your platform requires nesting-aware IRQ control, replace with PRIMASK wrapper */
	__disable_irq();

	float32_t temp_mean_th;
	float32_t temp_th;

	for (uint8_t i = 0U; i < NUM_SENS; i++) {
		/* Guard: avoid division by zero if TH2_buffs_sizes_[i] is 0 */
		if (TH2_buffs_sizes_[i] == 0U) {
			continue;
		}

		temp_mean_th = TH2_sums_[i] / (float32_t) TH2_buffs_sizes_[i];
		temp_th = (1.7f * (TH2_max_[i] - temp_mean_th)) + temp_mean_th;

		threshold_DSP_1st_[i] = temp_th;
		threshold_DSP_2nd_[i] = temp_th * 100.0f;

		reset_TH2(i, 256U);
	}

	__enable_irq();
}

DSP_Status check_DSP_status(uint8_t sensor) {
	DSP_Status status = dsp_sensors[sensor]->status;
	return status;
}

/**
 * @brief Evaluate RMS vs thresholds with hysteresis (pre-stop + stop).
 * @note
 * - Uses counters ::count_pre_stop_ / ::count_pre_start_ / ::count_start_.
 * - Updates ::status_DSP_pre_stop_ and ::status_DSP_stop_.
 */
void rms_output(void) {
	float32_t val;

	for (uint8_t i = 0U; i < NUM_SENS; i++) {
		val = pow_fil_[i];

		/* Pre-stop evaluation with debounce */
		if ((val > threshold_DSP_1st_[i]) && (flag_th_DSP_[i] != 0U)) {
			if (count_pre_stop_[i] >= count_dim_stop_) {
				status_DSP_pre_stop_[i] = 1U;
			} else {
				count_pre_stop_[i]++;
			}
			count_pre_start_[i] = 0U;
		} else {
			if (count_pre_start_[i] >= count_dim_start_) {
				status_DSP_pre_stop_[i] = 0U;
			} else {
				count_pre_start_[i]++;
			}
			count_pre_stop_[i] = 0U;
		}

		/* Stop evaluation with debounce */
		if ((val > threshold_DSP_2nd_[i]) && (flag_th_DSP_[i] != 0U)) {
			status_DSP_stop_[i] = 1U;
			count_start_[i] = 0U;
		} else {
			if (count_start_[i] >= count_dim_start_) {
				status_DSP_stop_[i] = 0U;
			} else {
				count_start_[i]++;
			}
		}
	}
}

/**
 * @brief Consolidate DSP status across sensors and send CAN events on edges.
 * @details
 * - Excludes sensors 1 and 5 from evaluation (policy-based).
 * - Sends CMD_PRE_STOP/CMD_PRE_START and CMD_STOP/CMD_START on state changes.
 */
void control_DSP_status(void) {
	uint8_t result_pre_stop = 0U;
	uint8_t result_stop = 0U;

	for (uint8_t i = 0U; i < NUM_SENS; i++) {
		if (status_DSP_pre_stop_[i] != 0U) {
			result_pre_stop++;
		}
		if (status_DSP_stop_[i] != 0U) {
			result_stop++;
		}
	}

	/* PRE-STOP edges */
	if (result_pre_stop != 0U) {
		if (flag_msg_DSP_pre_stop_sent_ == 0U) {

			flag_msg_DSP_pre_stop_sent_ = 1U;

			TxData_[0] = CMD_PRE_STOP;
			FDCANTx(SLAVE_ID_, 1U);
		}
	} else {
		if (flag_msg_DSP_pre_stop_sent_ != 0U) {

			flag_msg_DSP_pre_stop_sent_ = 0U;

			TxData_[0] = CMD_PRE_START;
			FDCANTx(SLAVE_ID_, 1U);
		}
	}

	/* STOP edges */
	if (result_stop != 0U) {
		if (flag_msg_DSP_stop_sent_ == 0U) {
			flag_msg_DSP_stop_sent_ = 1U;

			TxData_[0] = CMD_STOP;
			FDCANTx(SLAVE_ID_, 1U);
		}
	} else {
		if (flag_msg_DSP_stop_sent_ != 0U) {
			flag_msg_DSP_stop_sent_ = 0U;

			TxData_[0] = CMD_START;
			FDCANTx(SLAVE_ID_, 1U);
		}
	}
}
