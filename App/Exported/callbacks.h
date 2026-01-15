#ifndef __CALLBACKS_H
#define __CALLBACKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdint.h>

// Rising edge handler
extern void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin);

// Falling edge handler
extern void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin);

/**
 * @brief  UART RX complete callback.
 * @param  huart UART handle pointer.
 * @details Assembles CRLF-terminated command and calls @ref process_serial_data().
 */
extern void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

/**
 * @brief  FDCAN RX FIFO0 callback.
 * @param  hfdcan FDCAN handle pointer.
 * @param  RxFifo0ITs Interrupt flags.
 * @details Retrieves CAN frame and calls @ref FDCANRx().
 */
extern void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);

/**
 * @brief  Timer period elapsed callback.
 * @param  htim Timer handle pointer.
 * @details TIM2 calls @ref check_slave_status(), TIM4 calls @ref plot_control().
 */
extern void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif

#endif /* __CALLBACKS_H */
