/**
 * @file plot_manager.c
 * @brief Manage debug plotting via UART and CAN bus.
 * @details
 * Provides the function ::plot_control() that formats and sends sensor data
 * for plotting, depending on build target:
 * - HEXOPAD: sends moving averages and thresholds of two sensors.
 * - HEXOPUS: sends selected EMA / RMS / power values of multiple sensors.
 *
 * ## Concurrency
 * - Globals (e.g. `flag_serial_plot_`, `flag_can_plot_`, buffers) are updated
 *   in ISR and main contexts. Protect if used concurrently.
 *
 * @details
 *  - Uses @ref UART_Print() for human-readable streaming.
 *  - Optionally uses @ref FDCANTx() to send binary pairs (MVG/TH) on CAN.
 *  - Selects behavior depending on product role (HEXOPAD or MASTER).
 * @author milo
 * @date   2025-09-22
 */

#include "plot_manager.h"
#include "serial_manager.h"

#ifdef HEXOPAD
#include "can_manager.h"
#define PLOT_CAN_LEN 32

#endif /* HEXOPAD */

#ifdef HEXOPUS
#include "can_manager.h"

/**
 * @brief Plot control (HEXOPUS build).
 * @details
 * - If `flag_serial_plot_` is set, prints selected signal set to UART.
 * - If `flag_can_plot_` is set, sends selected signal array via CAN.
 * - Selection depends on `signal_selected_` global.
 */
//void plot_control() {
//	char debug[100];
//	uint8_t signal = signal_selected_;
//	float32_t signal_to_send[8] = { 0.0F };
//	uint8_t len;
//	switch (signal) {
//	case 0:
//		memcpy(signal_to_send, EMA_, sizeof(EMA_));
//		break;
//	case 1:
//		memcpy(signal_to_send, EMA_rms_, sizeof(EMA_rms_));
//		break;
//	case 2:
//		memcpy(signal_to_send, EMA_rms_, sizeof(EMA_rms_));
//		signal_to_send[0] = EMA_rms_[sensor_selected_];
//		signal_to_send[1] = threshold_DSP_pre_stop_[sensor_selected_];
//		signal_to_send[2] = threshold_DSP_stop_[sensor_selected_];
//		break;
//	case 3:
//		signal_to_send[0] = sec_fil_val_[0];
//		signal_to_send[1] = thresholds_[0];
//		signal_to_send[2] = sec_fil_val_[1];
//		signal_to_send[3] = thresholds_[1];
//		signal_to_send[4] = sec_fil_val_[2];
//		signal_to_send[5] = thresholds_[2];
//		signal_to_send[6] = sec_fil_val_[3];
//		signal_to_send[7] = thresholds_[3];
//		break;
//	case 4:
//		signal_to_send[0] = sec_fil_val_[4];
//		signal_to_send[1] = thresholds_[4];
//		signal_to_send[2] = sec_fil_val_[5];
//		signal_to_send[3] = thresholds_[5];
//		signal_to_send[4] = sec_fil_val_[6];
//		signal_to_send[5] = thresholds_[6];
//		signal_to_send[6] = sec_fil_val_[7];
//		signal_to_send[7] = thresholds_[7];
//		break;
//	case 5:
//		memcpy(signal_to_send, power_, sizeof(power_));
//		break;
//	default:
//		break;
//	}
//	if (flag_serial_plot_) {
//		sprintf(debug, "%.1f;%.1f;%.1f;%.1f;%.1f;%.1f;%.1f;%.1f\r\n",
//				signal_to_send[0], signal_to_send[1], signal_to_send[2],
//				signal_to_send[3], signal_to_send[4], signal_to_send[5],
//				signal_to_send[6], signal_to_send[7]);
//		UART_Print(debug);
//	}
//	if (flag_can_plot_) {
//		len = 32;
//		memcpy(&TxData_, &signal_to_send, sizeof(signal_to_send));
//		FDCANTx(CAN_PLOT_ID, len);
//	}
////		count_can_plot_ = (count_can_plot_ + 1) % 4;
//}
#endif /* HEXOPUS */
