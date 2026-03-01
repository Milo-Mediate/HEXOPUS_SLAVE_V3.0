/*
 * can_parameters.c
 *
 *  Created on: Feb 4, 2026
 *      Author: emili
 */

#include "can_manager.h"
#include "can_parameters.h"
#include "parameters_func.h"
#include "app_hw_definition.h"

//static HAL_StatusTypeDef can_get_T_parameters<typename T>(uint8_t command, T value)
//{
//	constexpr sT = sizeof(T);
//	uint8_t TxData[sT+1] = { command };
//	memcpy(&TxData[1], &value, sT);
//    FDCANTx(SLAVE_ID_, TxData, sizeof(TxData));
//    // TODO: return FDCANTx(SLAVE_ID_, TxData, sizeof(TxData));
//    return HAL_OK;
//}

static HAL_StatusTypeDef can_get_float32_parameters(uint8_t command, float32_t value)
{
	uint8_t TxData[5] = { command };
	memcpy(&TxData[1], &value, FLOAT_LEN);
	FDCANTx(SLAVE_ID_1, TxData, sizeof(TxData));

    // TODO: return FDCANTx(SLAVE_ID_, TxData, sizeof(TxData));
    return HAL_OK;
}

static HAL_StatusTypeDef can_get_uint16_parameters(uint8_t command, uint16_t value)
{
	uint8_t TxData[3] = { command };
	memcpy(&TxData[1], &value, UINT16_T_LEN);
    FDCANTx(SLAVE_ID_1, TxData, sizeof(TxData));
    // TODO: return FDCANTx(SLAVE_ID_, TxData, sizeof(TxData));
    return HAL_OK;
}

HAL_StatusTypeDef can_get_gain(const uint8_t *RxData)
{
    return can_get_float32_parameters(CMD_GET_GAIN, get_gain(RxData));
}

HAL_StatusTypeDef can_get_delta(const uint8_t *RxData)
{
    return can_get_float32_parameters(CMD_GET_DELTA_V, get_delta(RxData));
}

HAL_StatusTypeDef can_get_tau_1(const uint8_t *RxData)
{
    return can_get_float32_parameters(CMD_GET_TAU_1, get_tau_1(RxData));
}

HAL_StatusTypeDef can_get_tau_2(const uint8_t *RxData)
{
    return can_get_float32_parameters(CMD_GET_TAU_2, get_tau_2(RxData));
}

HAL_StatusTypeDef can_get_tau_th(const uint8_t *RxData)
{
    return can_get_float32_parameters(CMD_GET_TAU_TH, get_tau_th(RxData));
}

HAL_StatusTypeDef can_get_num_cycle(const uint8_t *RxData)
{
    return can_get_uint16_parameters(CMD_GET_NUM_CYCLE, get_num_cycle(RxData));
}

HAL_StatusTypeDef can_get_dsp_th_1(const uint8_t *RxData)
{
	return can_get_float32_parameters(CMD_GET_TH1, get_dsp_th_1(RxData));
}

HAL_StatusTypeDef can_get_dsp_th_2(const uint8_t *RxData)
{
	return can_get_float32_parameters(CMD_GET_TH2, get_dsp_th_2(RxData));
}
