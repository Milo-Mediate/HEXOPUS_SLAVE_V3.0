/**
 * @file can_manager.c
 * @brief Manage CAN communication
 * @author milo
 * @date 2025-03-20
 */
#include "tim.h"
#include "fdcan.h"

#include "timer_function.h"
#include "can_plot.h"
#include "can_manager.h"
#include "can_parameters.h"
#include "can_LTC1660.h"
#include "can_TLC5916.h"
#include "serial_manager.h"
#include "parameters_func.h"
#include "app_hw_definition.h"
#include "app_TLC5916.h"
#include "app_algorithm.h"
#include "app_DSP_algorithm.h"
#include "machine_state.h"
#include "logging.h"
//#include "can_DAC.h"


#define MAX_PAYLOAD_CAN 64
/**
 * @brief Convert a byte length into FDCAN DLC encoding.
 * @param len Payload length in bytes (0..64).
 * @return HAL-compatible DLC field for TxHeader_.DataLength.
 */

//uint8_t sens_to_plot = 0;
static bool plotting = false;

uint32_t dlc_from_len_opt(uint8_t len) {
	static const uint8_t dlc_lut[65] = {
	/* 0..8 */
	0, 1, 2, 3, 4, 5, 6, 7, 8,
	/* 9..12 */
	[9 ... 12] = FDCAN_DLC_BYTES_12,
	/* 13..16 */
	[13 ... 16] = FDCAN_DLC_BYTES_16,
	/* 17..20 */
	[17 ... 20] = FDCAN_DLC_BYTES_20,
	/* 21..24 */
	[21 ... 24] = FDCAN_DLC_BYTES_24,
	/* 25..32 */
	[25 ... 32] = FDCAN_DLC_BYTES_32,
	/* 33..48 */
	[33 ... 48] = FDCAN_DLC_BYTES_48,
	/* 49..64 */
	[49 ... 64] = FDCAN_DLC_BYTES_64, };

	if (len > 64)
		return FDCAN_DLC_BYTES_64; /* or assert / error */

	return dlc_lut[len];
}
/**
 * @brief Convert a byte length into FDCAN DLC encoding.
 * @param len Payload length in bytes (0..64).
 * @return HAL-compatible DLC field for TxHeader_.DataLength.
 */
static uint32_t dlc_from_len(uint8_t len) {
	if (len <= 8)
		return len; /* Classic CAN uses 0..8 */
	else if (len <= 12)
		return FDCAN_DLC_BYTES_12;
	else if (len <= 16)
		return FDCAN_DLC_BYTES_16;
	else if (len <= 20)
		return FDCAN_DLC_BYTES_20;
	else if (len <= 24)
		return FDCAN_DLC_BYTES_24;
	else if (len <= 32)
		return FDCAN_DLC_BYTES_32;
	else if (len <= 48)
		return FDCAN_DLC_BYTES_48;
	else
		return FDCAN_DLC_BYTES_64;
}

void FDCANTx(uint16_t CAN_ID, uint8_t *TxData, uint8_t len) {
	FDCAN_TxHeaderTypeDef TxHeader = {
		.Identifier          = CAN_ID,
		.IdType              = FDCAN_STANDARD_ID,
		.TxFrameType         = FDCAN_DATA_FRAME,
		.DataLength          = dlc_from_len(len),
		.ErrorStateIndicator = FDCAN_ESI_ACTIVE,
		.BitRateSwitch       = FDCAN_BRS_OFF,
		.FDFormat            = FDCAN_FD_CAN,
		.TxEventFifoControl  = FDCAN_NO_TX_EVENTS,
		.MessageMarker       = 0
	};
	uint8_t txbuff[64];
	memcpy(txbuff, TxData, len);
	txbuff[len] = 0;

	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, txbuff) != HAL_OK) {
		Error_Handler();
	}
}



void on_new_can_data(FDCAN_HandleTypeDef *hfdcan, uint32_t rx_fifo_conf) {
	FDCAN_RxHeaderTypeDef RxHeader;
	uint8_t RxData[MAX_PAYLOAD_CAN];
	/* Retreive Rx messages from RX FIFO0 */

	if (HAL_FDCAN_GetRxMessage(hfdcan, rx_fifo_conf, &RxHeader, RxData)
			!= HAL_OK) {
		/* Reception Error */
		Error_Handler();
	}
	uint16_t CAN_ID = RxHeader.Identifier;
	if ((CAN_ID & 0x0f0) == SLAVE_ID_1 || (CAN_ID & 0x0f0) == SLAVE_ID_2) { // TODO cambiare con il filtro HW del CAN

		FDCAN_parse(hfdcan, CAN_ID, RxData, RxHeader.DataLength);

	}
}


/**
 * @brief  Transmit a CAN frame using global TX header/data.
 * @param  CAN_ID 11-bit identifier.
 * @param  len    Number of payload bytes.
 */
//void FDCANTx(uint16_t CAN_ID, uint8_t len) {
//	TxHeader_.Identifier = CAN_ID;
//	TxHeader_.IdType = FDCAN_STANDARD_ID;
//	TxHeader_.TxFrameType = FDCAN_DATA_FRAME;
//	TxHeader_.DataLength = dlc_from_len(len);
//	TxHeader_.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
//	TxHeader_.BitRateSwitch = FDCAN_BRS_OFF;
//	TxHeader_.FDFormat = FDCAN_FD_CAN;
//	TxHeader_.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
//	TxHeader_.MessageMarker = 0;
//	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader_, TxData_)
//			!= HAL_OK) {
//		Error_Handler();
//	}
//	memset(TxData_, 0, sizeof(TxData_));
//}


#ifndef MASTER

#include "global.h"

void send_slave_status(const uint16_t CAN_ID)
{
	uint8_t status = get_machine_state();
	bool stop_1 = get_stop_1();
	bool stop_2 = get_stop_2();

	uint8_t TxData[4] = { CMD_GET_SLAVE_STATUS, status, stop_1, stop_2};
	FDCANTx(CAN_ID, TxData, sizeof(TxData));
}

void send_stop(const uint16_t CAN_ID)
{
	uint8_t TxData[2] = { CMD_STOP };
	FDCANTx(CAN_ID, TxData, sizeof(TxData));
}

void send_params(const uint16_t CAN_ID, uint8_t sens)
{
	uint8_t TxData[20] = { CMD_GET_PARAMETERS };
	TxData[1] = sens;
	memcpy(&TxData[2], &real_thresholds[sens].gain, sizeof(float));
	memcpy(&TxData[6], &real_sensors[sens].num_cycle_max, sizeof(uint16_t));
	memcpy(&TxData[8], &real_sensors[sens].delta, sizeof(float));
	memcpy(&TxData[12], &dsp_thresholds[sens].gain_1, sizeof(float));
	memcpy(&TxData[16], &dsp_thresholds[sens].gain_2, sizeof(float));
	FDCANTx(CAN_ID, TxData, sizeof(TxData));
}

void FDCAN_parse(FDCAN_HandleTypeDef *hfdcan, uint16_t CAN_ID, const uint8_t *RxData, uint8_t data_len) {
	uint8_t command = RxData[0];
	switch (command) {
	case CMD_GET_INFO:
		// TODO
		break;

	case CMD_GET_PARAMETERS:
		set_machine_state(SEND_PARAMS);
		break;

	case CMD_END_CAL:
		// TODO
		break;

	case CMD_SET_SLAVE_STATUS:
		// TODO: machine state
		break;

	case CMD_SET_CAL_STATUS:
		// TODO
		break;

	case CMD_GET_CAL_STATUS:
		// TODO
		break;

	case CMD_SET_SEN_STATUS:
		// TODO: ridondante?
		break;

	case CMD_GET_SEN_STATUS:
		// TODO: ridondante?
		break;

	case CMD_READY:
		// TODO: Errore, comando che può ricevere solo la master
		break;

	case CMD_HEARTBEAT:
		// TODO: Errore, comando che può ricevere solo la master
		break;
	case CMD_FLASH_WRITE:
		// TODO
		break;
	case CMD_FLASH_READ:
		// TODO
		break;
	case CMD_FLASH_ERASE:
		// TODO
		break;
// TODO solo master e hexopad
//		case CMD_SET_LED_DIAG_1:
//		set_led_1(RxData[1]);
//		break;
//	case CMD_GET_LED_DIAG_1:
//		can_get_led_1();
//		break;
//	case CMD_SET_LED_DIAG_2:
//		set_led_2(RxData[1]);
//		break;
//	case CMD_GET_LED_DIAG_2:
//		can_get_led_2();
//		break;
//	case CMD_SET_LED_MUTING:
//		set_led_muting(RxData[1]);
//		break;
//	case CMD_GET_LED_MUTING:
//		can_get_led_muting();
//		break;
	case CMD_SET_LED_SENSOR:
		can_set_tlc_status(RxData);
		break;
	case CMD_GET_LED_SENSOR:
		can_get_tlc_status();
		break;
	case CMD_CAN_SCAN:
		// TODO: controllare se serve?
		break;

	case CMD_STOP:
		// TODO: portare lo stato in STOP
		break;
	case CMD_AUTOSET_V_REF_1:
		// TODO: auto_set_dac_value(dac_1, RxData);
		break;
	case CMD_AUTOSET_V_REF_2:
		// TODO: auto_set_dac_value(dac_1, RxData);
		break;
	case CMD_SET_V_REF_1:
		can_set_dac_1_channel(RxData);
		break;
	case CMD_GET_V_REF_1:
		can_get_dac_1_channel(RxData);
		break;
	case CMD_SET_V_REF_2:
		can_set_dac_2_channel(RxData);
		break;
	case CMD_GET_V_REF_2:
		can_get_dac_2_channel(RxData);
		break;
	case CMD_SET_TH1:
		set_dsp_th_1(RxData);
		break;
	case CMD_GET_TH1:
		get_dsp_th_1(RxData);
		break;
	case CMD_SET_TH2:
		set_dsp_th_2(RxData);
		break;
	case CMD_GET_TH2:
		get_dsp_th_2(RxData);
		break;
	case CMD_SET_GAIN:
		set_gain(RxData);
		break;
	case CMD_GET_GAIN:
		can_get_gain(RxData);
		break;
	case CMD_SET_DELTA_V:
		set_delta(RxData);
		break;
	case CMD_GET_DELTA_V:
		can_get_delta(RxData);
		break;
	case CMD_SET_TAU_1:
		set_tau_1(RxData);
		break;
	case CMD_GET_TAU_1:
		can_get_tau_1(RxData);
		break;
	case CMD_SET_TAU_2:
		set_tau_2(RxData);
		break;
	case CMD_GET_TAU_2:
		can_get_tau_2(RxData);
		break;
	case CMD_SET_TAU_TH:
		set_tau_th(RxData);
		break;
	case CMD_GET_TAU_TH:
		can_get_tau_th(RxData);
		break;
	case CMD_SET_NUM_CYCLE:
		set_num_cycle(RxData);
		break;
	case CMD_GET_NUM_CYCLE:
		can_get_num_cycle(RxData);
		break;
	case CMD_ALG_ON:
		turn_on_algorithm(RxData);
		break;
	case CMD_ALG_OFF:
		turn_off_algorithm(RxData);
		break;
	case CMD_DSP_ALG_ON:
		turn_on_dsp_algorithm(RxData);
		break;
	case CMD_DSP_ALG_OFF:
		turn_off_dsp_algorithm(RxData);
		break;
	case CMD_SERIAL_PLOT:
		// TODO errore se proviene da CAN
		break;

	case CMD_CAN_MSG_DEBUG:
//		sprintf(data_to_send, "MSG FDCAN RECEIVED\r\n");
//		UART_Print(data_to_send);
		break;
	case CMD_PLOT_ON:
		if (!plotting) {
			timer_on(&htim4);
			plotting = true;
		}
		break;
	case CMD_PLOT_OFF:
		timer_off(&htim4);
		plotting = false;
//		sprintf(data_to_send, "PLOT OFF\r\n");
//		UART_Print(data_to_send);
		break;

	case CMD_GET_SLAVE_STATUS:
		send_slave_status(CAN_ID);
		break;

//	case CMD_SELECT_SENSOR:
//		sensor_selected_ = RxData[1];
//		if (sensor_selected_ >= NUM_SENS) {
//			sensor_selected_ = 1;
//		}
////		sprintf(data_to_send, "Sensor selected: %u \r\n", sensor_selected_);
////		UART_Print(data_to_send);
//		break;

	case CMD_SELECT_SIGNAL:
		signal_selected_ = RxData[1];
//		sprintf(data_to_send, "Signal selected: %u \r\n", signal_selected_);
//		UART_Print(data_to_send);
		break;
	case CMD_ROBOT_SET_TH:
		for (uint8_t i = 0; i < NUM_DSP; i++) {
			set_sensor_delta(2*i, 33);
		}
		break;
	case CMD_ROBOT_RESET_TH:
		for (uint8_t i = 0; i < NUM_DSP; i++) {
			set_sensor_delta(2*i, 1);
		}
		break;
	case CMD_RESET_SLAVE:
		set_machine_state(SLAVE_RESET);

		break;
//	case CMD_:
//		// TODO: template new command
//		break;
	default:
		break;
	}
}
#endif

