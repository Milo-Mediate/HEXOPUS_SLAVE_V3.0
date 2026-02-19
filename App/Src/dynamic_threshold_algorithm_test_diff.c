#include "dynamic_threshold_algorithm_test_diff.h"
// #include "buffer_manager.h"


static void put_TH1(Sensor_TH_t *threshold, float32_t val) {
	threshold->sum += val;
	threshold->count += 1;
	if (threshold->max < val)
		threshold->max = val;
}

static void reset_TH1(Sensor_TH_t *threshold, uint16_t new_size) {
	threshold->buffer_len = new_size;
	threshold->sum = 0;
	threshold->count = 0;
	threshold->max = 0;
}

void update_sensor(float32_t new_val, Sensor_t *sensor, Sensor_TH_t *threshold, Edge_Detector_t *edge_det) {
	sensor->filter_1 = (new_val) * sensor->k_1 + (sensor->filter_1) * (1.0f - sensor->k_1);
	sensor->filter_2 = (sensor->filter_1) * (sensor->k_2) + (sensor->filter_2) * (1.0f - sensor->k_2);

	if (edge_det->value >= edge_det->threshold) {
		edge_det->status = 1;
		edge_det->count_t = 0;
	} else if (edge_det->value <= -edge_det->threshold) {
		edge_det->status = 1;
		edge_det->count_t = 0;
	}
//	if (edge_det->status == 1) {
//		edge_det->count_t += 1;
//		if (edge_det->count_t == edge_det->t_blind) {
//			edge_det->count_t = 0;
//			sensor->filter_2 = sensor->filter_1;
//			threshold->value = sensor->filter_2 + 3;
//			reset_TH1(threshold, threshold->buffer_len);
//			edge_det->status = 0;
//		}
//		// TODO: if (obstacle && edge) reset manually;
//	} else {

		// if (threshold->value < thresholds_min_[i]) {
		// 	threshold->value = thresholds_min_[i];
		// }
	if ((sensor->filter_2 < threshold->value + sensor->delta)
			|| !(threshold->enable)) {
		sensor->status = 0;
		sensor->num_cycle_count = 0;
		put_TH1(threshold, sensor->filter_2);
		if (threshold->count == threshold->buffer_len) {
			float32_t temp_mean_th = threshold->sum / (threshold->buffer_len);
			float32_t temp_th = (threshold->gain * (threshold->max - temp_mean_th) + temp_mean_th);
			reset_TH1(threshold, threshold->buffer_len);
			threshold->value = (temp_th) * (threshold->k)
					+ (threshold->value) * (1.0f - threshold->k);
		}
	} // else
	if ((sensor->filter_2 > threshold->value + sensor->delta)
			&& threshold->enable) {
		if (sensor->num_cycle_count >= sensor->num_cycle_max) {
			sensor->status = 1; // status_sensor_stop_ |= (1 << i);
		} else {
			sensor->num_cycle_count++;
		}
//		}
	}
//
//	// DERIVATIVE CALCULATION
//	edge_det->value = sensor->filter_1 - edge_det->buffer[edge_det->count_delay];
//	edge_det->buffer[edge_det->count_delay] = sensor->filter_1;
//	edge_det->count_delay += 1;
//
//	if (edge_det->count_delay == edge_det->delay_max) {
//		// edge_det->value = sensor->filter_1 - prev_val_[i];
//		edge_det->count_delay = 0;
//	}
}

float32_t EMA_GetAlpha(float32_t tau, float32_t ts)
{
    if (tau <= 0.0f) return 1.0f; // Instant response (no filtering)
    float32_t alpha = 1.0f - expf(-ts / tau);
    return alpha;
}

//void dynamic_threshold_algorithm_test_diff() {
//	//char msg[50];
//
//	float32_t new_val[NUM_SENS] = { 0.0f };
//	float32_t temp_mean_th;
//	float32_t temp_th;
//	float32_t ist_diff = 0;
//	float32_t sec_diff = 0;
//	float32_t aux_arr[NUM_SENS] = { 0.0f };
//
//	if (flag_read_new_val_) {
//		for (uint8_t i = 0; i < NUM_SENS; i++) {
//			new_val[i] = adc1_data_[i];
//			ist_fil_val_[i] = (new_val[i]) * ist_k_[i]
//					+ (ist_fil_val_[i]) * (1 - ist_k_[i]);
//			sec_fil_val_[i] = (ist_fil_val_[i]) * (sec_k_[i])
//					+ (sec_fil_val_[i]) * (1 - sec_k_[i]);
//
////			// DERIVATIVE PART
////			der_EMA_[i] = ist_fil_val_[i] - der_buff_[count_decim_[i]];
////			der_buff_[count_decim_[i]] = ist_fil_val_[i];
////			count_decim_[i] += 1;
////			if (count_decim_[i] == dim_decim_) {
////				//der_EMA_[i] = ist_fil_val_[i] - prev_val_[i];
////				count_decim_[i] = 0;
////			}
//		}
//
//		ist_diff = (ist_fil_val_[0] - ist_fil_val_[1]);
//		sec_diff = (sec_fil_val_[0] - sec_fil_val_[1]);
//
//		aux_arr[0] = sec_fil_val_[0];
//		aux_arr[1] = sec_diff + 738.0f;
//
//		for (uint8_t i = 0; i < NUM_SENS; i++) {
//			put_TH1(i, aux_arr[i]);
//			if (TH1_cont_[i] == TH1_buffs_sizes_[i]) {
//				temp_mean_th = TH1_sums_[i] / (TH1_buffs_sizes_[i]);
//				temp_th = (gains_[i] * (TH1_max_[i] - temp_mean_th)
//						+ temp_mean_th);
//				reset_TH1(i, TH1_buffs_sizes_[i]);
//				thresholds_[i] = (temp_th) * (th_k_[i])
//						+ (thresholds_[i]) * (1 - th_k_[i]);
//				if (status_sensor_stop_[i] == 0) {
//					thresholds_aft_comm_[i] = thresholds_[i];
//				}
//			}
//			if ((aux_arr[i] < thresholds_aft_comm_[i] + deltas_[i])
//					|| !(flag_th_sensor_[i])) {
//				status_sensor_stop_[i] = 0;
//				num_cycles_cont_[i] = 0;
//				thresholds_[i] = thresholds_aft_comm_[i];
//			}
//			if ((aux_arr[i] > thresholds_aft_comm_[i] + deltas_[i])
//					&& flag_th_sensor_[i]) {
//				if (num_cycles_cont_[i] >= num_cycles_sizes_[i]) {
//					status_sensor_stop_[i] = 1;
//				} else {
//					num_cycles_cont_[i]++;
//				}
//			}
//		}
//
//		if (status_sensor_stop_[1] && status_sensor_stop_[0]) {
//			flag_stop_ = 1;
//		} else if ((status_sensor_stop_[1] ^ status_sensor_stop_[0])) {
//			count_diff_delay_ ++;
//			if (count_diff_delay_ >= dim_diff_delay_) {
//				flag_stop_ = 0;
//				count_diff_delay_ = 0;
//				thresholds_aft_comm_[0] = thresholds_[0];
//				thresholds_aft_comm_[1] = thresholds_[1];
//			}
//		} else {
//			flag_stop_ = 0;
//		}
//
//		// PLOTTING DATA
//		if (signal_selected_ == 0U) {
//			signal_to_send_[0] = aux_arr[0];
//			signal_to_send_[1] = thresholds_aft_comm_[0] + deltas_[0];
//			signal_to_send_[2] = aux_arr[1];
//			signal_to_send_[3] = thresholds_aft_comm_[1] + deltas_[1];
//			signal_to_send_[4] = sec_fil_val_[1];
//			signal_to_send_[5] = ist_diff;
//			signal_to_send_[6] = 0;
//			signal_to_send_[7] = 0;
//		} else {
//			signal_to_send_[0] = new_val[sensor_selected_];
//			signal_to_send_[1] = adc_MAX_[sensor_selected_];
//			signal_to_send_[2] = adc_Min_[sensor_selected_];
//			signal_to_send_[3] = ist_fil_val_[sensor_selected_];
//			signal_to_send_[4] = sec_fil_val_[sensor_selected_];
//			signal_to_send_[5] = thresholds_[sensor_selected_]
//					+ deltas_[sensor_selected_];
//			signal_to_send_[6] = der_EMA_[sensor_selected_];
//			signal_to_send_[7] = der_EMA_MAX[sensor_selected_];
//		}
//	}
//}

