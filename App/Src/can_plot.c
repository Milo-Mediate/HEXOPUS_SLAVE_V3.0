/*
 * can_plot.c
 *
 *  Created on: Feb 10, 2026
 *      Author: emili
 */

#include "can_plot.h"
#include "can_manager.h"
#include "app_algorithm.h"
#include "app_hw_definition.h"
#include "definition.h"




#define PLOT_ID                    0x300
#define PLOT_PAYLOAD               28
#define CAN_PAYLOAD                32
#define FLOAT_BUFF_PLOT_LEN        7



static HAL_StatusTypeDef can_send_buff( uint8_t sens, float32_t *data)
{
	uint8_t TxData[CAN_PAYLOAD] = { CMD_PLOT_SENSOR_1 + sens};
	memcpy(&TxData[1], &data[0], PLOT_PAYLOAD);
	FDCANTx(PLOT_ID, TxData, sizeof(TxData));
	return HAL_OK;
}


void can_plot_sens()
{
	for (uint8_t sens = 0U; sens < NUM_SENS / 2; sens++)
	{
		float32_t data[FLOAT_BUFF_PLOT_LEN];
		build_txdata(sens, data);
		can_send_buff(sens, data);
		// send_buff(txdata, TXDATA_LEN);
	}
}
