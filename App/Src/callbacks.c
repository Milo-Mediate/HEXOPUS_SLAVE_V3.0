#include "callbacks.h"

#include <stdbool.h>
#include "global.h"
#include "can_manager.h"
#include "can_plot.h"
#include "event_manager.h"
#include "logging.h"
#include "plot_manager.h"
#include "serial_manager.h"
#include "app_algorithm.h"
#include "app_DSP_algorithm.h"
#include "app_hw_definition.h"
#include "app_TLC5916.h"
#include "stm32h5xx.h"
#include "machine_state.h"

static char data_to_send[100];
static uint8_t sens = 0;

// Rising edge handler
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {

}

// Falling edge handler
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin) {

}

/**
 * @brief  UART RX complete callback.
 * @param  huart UART handle pointer.
 * @details Assembles CRLF-terminated command and calls @ref process_serial_data().
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		//on_new_serial_data();
	}
}

/**
 * @brief  FDCAN RX FIFO0 callback.
 * @param  hfdcan FDCAN handle pointer.
 * @param  RxFifo0ITs Interrupt flags.
 * @details Retrieves CAN frame and calls @ref FDCANRx().
 */

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
	if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
		on_new_can_data(hfdcan, FDCAN_RX_FIFO0);
	}
	if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
		/* Notification Error */
		Error_Handler();
	}
}
/**
 * @brief  Timer period elapsed callback.
 * @param  htim Timer handle pointer.
 * @details TIM2 calls @ref check_slave_status(), TIM4 calls @ref plot_control().
 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2) {
		dynamic_threshold_algorithm_test();
	}
	if (htim->Instance == TIM3) {
		DSP_algorithm();
	}
	if (htim->Instance == TIM4) {
		can_plot_sens(sens);
		sens = (sens + 1) % NUM_DSP;
	}
	if (htim->Instance == TIM5)
	{
		bool stop_1 = check_all_DSP_status_1();
		bool stop_2 = check_all_DSP_status_2();
		send_stop(SLAVE_ID_1, stop_1, stop_2);

//			set_machine_state(STOP);
//			set_stop_1(true);
//		} else {
//			set_stop_1(false);
////			set_msg_1(false);
//		}
//		if(check_all_DSP_status_2())
//		{
//			set_machine_state(STOP);
//			set_stop_2(true);
//		} else {
//			set_stop_2(false);
//			set_msg_2(false);
//		}
	}
	if (htim->Instance == TIM12) {

	}
}

/**
 * @brief ADC conversion complete callback.
 *
 * This callback is invoked by the HAL when the ADC DMA transfer
 * has completed filling the entire conversion buffer.
 *
 * Typical use case:
 * - ADC configured in scan mode with DMA enabled.
 * - DMA transfer size matches the number of ADC channels.
 * - Each invocation indicates that all elements of the ADC data buffer
 *   contain fresh conversion results.
 *
 * In DMA circular mode, this callback is called at the end of each
 * complete conversion sequence (i.e., once per full buffer update).
 *
 * @param hadc Pointer to the ADC handle that triggered the callback.
 *
 * @note This function increments a global update counter used to
 *       signal the availability of new ADC samples to other parts
 *       of the application (e.g., calibration routines).
 * @note If data cache (D-Cache) is enabled on the MCU, ensure that
 *       the ADC DMA buffer is placed in a non-cacheable memory region
 *       or that the cache is properly invalidated before reading
 *       the buffer contents.
 */
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
//{
//	if (hadc->Instance == hadc1)
//		flag_adc_new_sample_ = true;
//}

