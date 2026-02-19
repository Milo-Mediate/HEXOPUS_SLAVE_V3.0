/*
 * can_LTC1660.c
 *
 *  Created on: Feb 4, 2026
 *      Author: emili
 */

#include "can_manager.h"
#include "can_LTC1660.h"
#include "LTC1660.h"
#include "app_LTC1660.h"
#include "app_hw_definition.h"

static HAL_StatusTypeDef can_get_dac_channel(uint8_t command, uint16_t value)
{
	uint8_t TxData[3] = { command };
	memcpy(&TxData[1], &value, sizeof(value));
	FDCANTx(SLAVE_ID_, TxData, sizeof(TxData));
	// TODO: return FDCANTx(SLAVE_ID_, TxData, sizeof(TxData));
	return HAL_OK;
}

HAL_StatusTypeDef can_get_dac_1_channel(uint8_t channel)
{
	return can_get_dac_channel(CMD_GET_V_REF_1, app_get_dac_channel(0, channel));
}

HAL_StatusTypeDef can_get_dac_2_channel(uint8_t channel)
{
	return can_get_dac_channel(CMD_GET_V_REF_2, app_get_dac_channel(1, channel));
}

//dac_cal_status_t can_auto_set_dac_value(DAC_t *dac, uint8_t channel) {
//
//}
