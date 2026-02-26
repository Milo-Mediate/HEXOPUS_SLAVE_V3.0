/*
 * can_plot.c
 *
 *  Created on: Feb 10, 2026
 *      Author: emili
 */

#include "can_plot.h"
#include "can_manager.h"
#include "app_algorithm.h"
#include "app_DSP_algorithm.h"
#include "app_hw_definition.h"
#include "definition.h"
#include "logging.h"

#define PLOT_ID                    0x300
#define PLOT_PAYLOAD               28
#define CAN_PAYLOAD                32
#define FLOAT_BUFF_PLOT_LEN        7

uint8_t sens_to_plot = 0;

static HAL_StatusTypeDef can_send_buff( uint8_t sens, float32_t *data)
{
	uint8_t TxData[CAN_PAYLOAD] = { sens + CMD_PLOT_SENSOR_1};
	memcpy(&TxData[1], data, PLOT_PAYLOAD);
	FDCANTx(PLOT_ID, TxData, sizeof(TxData));
	return HAL_OK;
}

void set_sensor_to_plot(uint8_t sens) {
	sens_to_plot = sens;
}


void can_plot_sens(uint8_t sens)
{
	float32_t data[FLOAT_BUFF_PLOT_LEN];
	build_txdata_alg(sens_to_plot, data);
	build_txdata_DSP_alg(sens_to_plot, data);
	can_send_buff(sens_to_plot, data);
//	lf_error("%.2f:%.2f:%.2f:%.2f:%.2f:%.2f:%.2f",
//			data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
//	for (uint8_t sens = 0U; sens < NUM_SENS / 2; sens++)
//	{
//		float32_t data[FLOAT_BUFF_PLOT_LEN];
//		build_txdata_alg(sens, data);
//		build_txdata_DSP_alg(sens, data);
//		can_send_buff(sens, data);
//		// send_buff(txdata, TXDATA_LEN);
//	}
}
