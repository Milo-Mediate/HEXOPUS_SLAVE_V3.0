#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H
/**
 * @file    can_manager.h
 * @brief   FDCAN communication API (Master/Slave)
 * @ingroup drivers
 *
 * @details
 *  Public API for sending/receiving FDCAN frames. The module relies on
 *  global handles/buffers defined in @ref global.h.
 */
//#include "global.h"
#include "stm32h5xx_hal.h"
#include <stdint.h>
#include "arm_math.h"


extern uint8_t sens_to_plot;


/**
 * @brief Convert a byte length into FDCAN DLC encoding.
 * @param len Payload length in bytes (0..64).
 * @return HAL-compatible DLC field for TxHeader_.DataLength.
 */
//uint32_t dlc_from_len(uint8_t len);


/**
 * @brief  Transmit a standard 11-bit CAN frame on @ref hfdcan1.
 * @param  CAN_ID 11-bit identifier (0x000..0x7FF).
 * @param  len    Payload length in bytes (0..64). Internally mapped to DLC.
 */
extern void FDCANTx(uint16_t CAN_ID, uint8_t *TxData, uint8_t len);

extern void on_new_can_data(FDCAN_HandleTypeDef *hfdcan, uint32_t rx_fifo_conf);

void FDCAN_parse(FDCAN_HandleTypeDef *hfdcan, uint16_t CAN_ID, const uint8_t *RxData, uint8_t data_len);

/**
 * @brief  Receive handler: demultiplexes incoming bus commands.
 * @note   Called from @ref HAL_FDCAN_RxFifo0Callback().
 */
extern void FDCANRx(void);


#endif /* CAN_MANAGER_H */

