/*
 * can_TLC5916.c
 *
 *  Created on: 19 feb 2026
 *      Author: emili
 */

#include "can_manager.h"
#include "can_TLC5916.h"
#include "TLC5916.h"
#include "app_hw_definition.h"
#include "definition.h"
#include "app_TLC5916.h"


HAL_StatusTypeDef can_get_tlc_status()
{
	uint8_t TxData[2] = { CMD_GET_LED_SENSOR };
	TxData[1] = app_get_tlc_status();
	FDCANTx(SLAVE_ID_1, TxData, sizeof(TxData));
	// TODO: return FDCANTx(SLAVE_ID_, TxData, sizeof(TxData));
	return HAL_OK;
}

void can_set_tlc_status(const uint8_t *RxData)
{
	uint8_t status = RxData[1];
	app_TLC5916_setAll(status);
}
