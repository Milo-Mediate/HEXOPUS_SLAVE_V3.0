/**
 * @file serial_manager.c
 * @brief Serial communication command parser and utilities (MASTER/SLAVE).
 * @details
 * Parses host commands received over UART and performs actions locally
 * or forwards compact frames over FDCAN to MASTER/SLAVE devices.
 *
 * ## Build variants
 * - MASTER: routes configuration to slaves, RTC/OSSD controls, info dump.
 * - SLAVE : device local controls (AD9833, LEDs), DSP params, debug.
 *
 * ## MISRA Notes
 * - Function prototypes use `void` when no parameters.
 * - Replaced most `sprintf` with `snprintf` and bounded copies.
 * - Added initializations for `len` in GET paths (was uninitialized).
 * - Guarded array copies with `sizeof(...)`; casting noted where needed.
 *
 * @author milo
 * @date   2025-03-20
 */

#include "serial_manager.h"
#include "can_manager.h"
#include "logging.h"
#include <string.h>   /* strlen, memcpy */
#include <stdio.h>    /* snprintf */

#ifdef HEXOPAD
#include "pot_manager.h"
#endif

#ifdef HEXOPUS
#include "LTC1660.h"
#include <app_DSP_algorithm.h>
#endif


//#ifndef MASTER

/**
 * @brief Parse and execute a received serial packet (SLAVE).
 * @param[in] package_len Length in bytes of the received packet.
 */
SerialMsgStatus process_serial_data(const uint8_t *buffer, size_t buffer_len) {
	uint8_t command = buffer[0];
	// Todo Implementare invio differenziale in funzione di variabile debug
	//uint8_t CAN_ID = 0x010;   // TODO: replace with device_selected_
//	uint8_t len = 0;
//	uint8_t current_state;
//	uint8_t current_slave_id;
//	uint8_t current_slave_index = 0;
//	uint8_t current_sensor;
//	uint8_t desired_sensor;
//	uint8_t current_choice;
//	uint8_t debug = 0;
	char data_to_send[100];
//	char data_to_check[UART_BUFFER_IN];
//	uint8_t aux2, aux3, aux4, aux5, aux6, aux7;
//	uint8_t crc_pass = 0;
//	uint16_t crc_current = 0;
//	uint16_t crc_received = (buffer[buffer_len-2] << 8) | buffer[buffer_len-1];


	switch (command) {
	case CMD_GET_INFO:
		// TODO: leggere le info di sistema
		break;

	case CMD_GET_PARAMETERS:
		// TODO: leggere i parametri di configurazione
		break;

//	case CMD_END_CALIBRATION:
//		// TODO: Errore, comando che può ricevere solo la master
//		break;
//
//	case CMD_GET_CALIBRATION_STATUS:
//		// TODO: leggere stato e i valori dei canali del potenziometro
//		break;
//
//	case CMD_GET_SENSOR_STATUS:
//		// TODO: ridondante?
//		break;
//	case CMD_SLAVE_STATUS:
//		// TODO: mandare lo stoto della macchina
//		break;
	case CMD_READY:
		// TODO:Errore, comando che può ricevere solo la master
		break;
	case CMD_HEARTBEAT:
		// TODO: rimandare CMD_HEARTBEAT
		break;
	case CMD_FLASH_WRITE:
		// TODO: scrivere flash
		break;
	case CMD_FLASH_READ:
		// TODO: leggere la flash
		break;
	case CMD_FLASH_ERASE:
		// TODO: azzerare flash
		break;
	case CMD_SET_LED_DIAG_1:
		// TODO:
		break;
	case CMD_GET_LED_DIAG_1:
		// TODO:
		break;
	case CMD_SET_LED_DIAG_2:
		// TODO:
		break;
	case CMD_GET_LED_DIAG_2:
		// TODO:
		break;
	case CMD_SET_LED_MUTING:
		// TODO:
		break;
	case CMD_GET_LED_MUTING:
		// TODO:
		break;
	case CMD_CAN_SCAN:
		// TODO: controllare se serve?
		break;
//	case CMD_CALIBRATE_SYSTEM:
//		// TODO:
//		break;
//
//	case CMD_STOP:
//		// TODO: portare lo stato in STOP
//		break;
//	case CMD_START:
//		// TODO: portare lo stato in STOP
//		break;
//	case CMD_AUTOSET_POT:
//		// TODO: auto settare la DAC
//		break;
//	case CMD_SET_V_REF:
//		// TODO: impostare un preciso valore della DAC
//		break;
//	case CMD_GET_V_REF:
//		// TODO: leggere il valore della DAC impostato
//		break;
	case CMD_SET_TH1:
		// TODO:
		break;
	case CMD_GET_TH1:
		// TODO:
		break;
	case CMD_SET_TH2:
		// TODO:
		break;
	case CMD_GET_TH2:
		// TODO:
		break;
	case CMD_SET_GAIN:
		// gains_[sensor_selected_] = f_aux;
		break;
	case CMD_GET_GAIN:
		// TODO:
		break;
	case CMD_SET_DELTA_V:
		// TODO:
		break;
	case CMD_GET_DELTA_V:
		// TODO:
		break;
	case CMD_SET_TAU_1:
		// TODO:
		break;
	case CMD_GET_TAU_1:
		// TODO:
		break;
	case CMD_SET_TAU_2:
		// TODO:
		break;
	case CMD_GET_TAU_2:
		// TODO:
		break;
	case CMD_SET_TAU_TH:
		// TODO:
		break;
	case CMD_GET_TAU_TH:
		// TODO:
		break;
	case CMD_SET_NUM_CYCLE:
		// TODO:
		break;
	case CMD_GET_NUM_CYCLE:
		// TODO:
		break;
	case CMD_ALG_ON:
		// TODO:
		break;
	case CMD_ALG_OFF:
		// TODO:
		break;
	case CMD_DSP_ALG_ON:
		// TODO:
		break;
	case CMD_DSP_ALG_OFF:
		// TODO:
		break;
	case CMD_SERIAL_PLOT:
		// TODO errore se  proviene da CAN
		break;
	case CMD_CAN_MSG_DEBUG:
		sprintf(data_to_send, "MSG FDCAN RECEIVED\r\n");
		UART_Print(data_to_send);
		break;
	case CMD_PLOT_ON:
//		flag_serial_plot_ = 1;
		sprintf(data_to_send, "PLOT ON\r\n");
		UART_Print(data_to_send);
		break;
	case CMD_PLOT_OFF:
//		flag_serial_plot_ = 0;
		sprintf(data_to_send, "PLOT OFF\r\n");
		UART_Print(data_to_send);
		break;
	case CMD_SELECT_SENSOR:
//		// TODO: sensor_selected_ = valore seriale;
//		if (sensor_selected_ >= NUM_SENS) {
//			sensor_selected_ = 1;
//		}
//		sprintf(data_to_send, "Sensor selected: %u \r\n", sensor_selected_);
//		UART_Print(data_to_send);
		break;

	case CMD_SELECT_SIGNAL:
		// TODO: signal_select_ = valore seriale;
//		sprintf(data_to_send, "Signal selected: %u \r\n", signal_selected_);
//		UART_Print(data_to_send);
		break;

		//	case CMD_:
		//		// TODO:
		//		break;
	default:
		break;
	}
	return 0;
}

/**
 * @brief Print parameters of a given sensor (SLAVE).
 * @param[in] sensor Sensor index.
 */
//void get_parameter(uint8_t sensor) {
//	char data_to_send[64];
//
//	(void) snprintf(data_to_send, sizeof(data_to_send),
//			"PARAMETERS SENSOR N: %u\r\n", sensor);
//	UART_Print(data_to_send);
//
//	(void) snprintf(data_to_send, sizeof(data_to_send), "Gain       : %f\r\n",
//			gains_[sensor]);
//	UART_Print(data_to_send);
//	(void) snprintf(data_to_send, sizeof(data_to_send), "Delta V    : %f\r\n",
//			deltas_[sensor]);
//	UART_Print(data_to_send);
//	(void) snprintf(data_to_send, sizeof(data_to_send), "Buffer 1   : %u\r\n",
//			TH1_buffs_sizes_[sensor]);
//	UART_Print(data_to_send);
//	(void) snprintf(data_to_send, sizeof(data_to_send), "Buffer 2   : %u\r\n",
//			TH2_buffs_sizes_[sensor]);
//	UART_Print(data_to_send);
//	(void) snprintf(data_to_send, sizeof(data_to_send), "Cycle      : %u\r\n",
//			num_cycles_sizes_[sensor]);
//	UART_Print(data_to_send);
//}
//#endif /* !MASTER */

/* ------------------------------------------------------------------------- */
/**
 * @brief Send firmware info/version over UART.
 */
void get_info(void) {
	char data_to_send[64];
	(void) snprintf(data_to_send, sizeof(data_to_send),
			"Current Version Firmware: %s\r\n", VERSION);
	UART_Print(data_to_send);
}

/**
 * @brief (DEPRECATED) Send two float arrays (MVG and TH) as a single UART frame.
 * @param[in] huart     UART handle.
 * @param[in] MVG_array Pointer to moving-average values.
 * @param[in] TH_array  Pointer to thresholds.
 * @param[in] num_sens  Number of sensors (array length).
 * @note Uses a VLA buffer for speed; consider chunked transmit to avoid VLA.
 */
//void SendFloatArray(UART_HandleTypeDef *huart, float *MVG_array,
//		float *TH_array, uint8_t num_sens) {
//	/* VLA: (2 * num_sens * sizeof(float) + 2) */
////	uint16_t payload_len = (uint16_t) (2U * (uint16_t) num_sens
////			* (uint16_t) sizeof(float));
////	uint8_t buffer_len = (uint8_t) (payload_len + 2U);
//	uint8_t buffer[/* VLA */2 * 8 * sizeof(float) + 2]; /* If NUM_SENS<=8, replace with fixed; else keep VLA */
//
//	/* Fallback: if num_sens > 8, cap copy to avoid overflow of fixed buffer above */
//#if defined(NUM_SENS) && (NUM_SENS <= 8)
//	(void) memcpy(buffer, MVG_array, (size_t) (num_sens * sizeof(float)));
//	(void) memcpy(buffer + (num_sens * (uint8_t) sizeof(float)), TH_array,
//			(size_t) (num_sens * sizeof(float)));
//	buffer[(num_sens * (uint8_t) sizeof(float)) * 2U] = '\r';
//	buffer[(num_sens * (uint8_t) sizeof(float)) * 2U + 1] = '\n';
//	HAL_UART_Transmit(huart, buffer, (uint16_t) sizeof(buffer), 100U);
//#else
//    /* Generic (less MISRA-friendly): true VLA path */
//    uint8_t *dyn = (uint8_t *)alloca(buffer_len); /* or malloc/free in RTOS task */
//    if (dyn == NULL) { return; }
//    (void)memcpy(dyn, MVG_array, (size_t)(num_sens * sizeof(float)));
//    (void)memcpy(dyn + (num_sens * sizeof(float)),
//                 TH_array, (size_t)(num_sens * sizeof(float)));
//    dyn[payload_len + 0U] = '\r';
//    dyn[payload_len + 1U] = '\n';
//    HAL_UART_Transmit(huart, dyn, (uint16_t)(payload_len + 2U), 100U);
//#endif
//}
