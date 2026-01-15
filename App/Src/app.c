#include "app.h"

#include "main.h"

#include "global.h"
#include "plot_manager.h"
#include "can_manager.h"
#include "MCP4551.h"
#include "event_manager.h"
#include "logging.h"
#include "crc_hw.h"
#include "app_serial.h"
#include "serial.h"
#include "serial_manager.h"

static char data_to_send[100];
float f_to_send;
uint16_t flash_check_crc;
uint16_t event_check_crc;
uint8_t debug = 0;
uint32_t current_cycle = 0;

void app_init()
{
	// HAL_Delay(5000); // ToDo inserire logica di controllo XOR_RB

	/* Peripherals start and interrupt management*/
	HAL_GPIO_EXTI_IRQHandler(XOR_RB_1_Pin);
	HAL_GPIO_EXTI_IRQHandler(XOR_RB_2_Pin);
	HAL_GPIO_EXTI_IRQHandler(XOR_RB_3_Pin);
	HAL_GPIO_EXTI_IRQHandler(XOR_RB_4_Pin);
	HAL_GPIO_EXTI_IRQHandler(XOR_RB_5_Pin);
	HAL_GPIO_EXTI_IRQHandler(XOR_RB_7_Pin);
	HAL_GPIO_EXTI_IRQHandler(XOR_RB_8_Pin);
	HAL_GPIO_EXTI_IRQHandler(XOR_RB_9_Pin);
	HAL_GPIO_EXTI_IRQHandler(XOR_RB_10_Pin);
	HAL_GPIO_EXTI_IRQHandler(XOR_RB_11_Pin);
	HAL_GPIO_EXTI_IRQHandler(XOR_RB_12_Pin);

	if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE,
			0) != HAL_OK) {
		// Notification Error
		Error_Handler();
	}

// TODO: inserire i TIM HAL_TIM_Base_Start_IT(&htim2);

// init_serial();

// TODO: inserire i PWM HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);


}

void app_main()
{
	uint32_t current_cycle_counter_ = 0;

	while (1) {

// 		ToDo: poll_serial();

		MS_ = EXTERNAL; // ToDo Rimuovere dopo aver finito il check sulla seriale
		switch (MS_) {
			case PRE_SETUP:
				break;

			case SETUP:
				break;

			case RUNNING:
				break;

			case EXTERNAL:
				break;

			case EXPIRED:
				break;

			case FAULT:
				break;

			case IDLE:
				break;

			case MUTING:
				break;

			case STOP:
				break;

			default:
				break;
		}
	}
}
