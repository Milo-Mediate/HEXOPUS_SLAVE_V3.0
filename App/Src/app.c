#include "app.h"

#include "main.h"

#include "plot_manager.h"
#include "can_manager.h"
#include "event_manager.h"
#include "logging.h"
#include "crc_hw.h"
#include "app_serial.h"
#include "serial.h"
#include "serial_manager.h"
#include "ADC_function.h"
#include "app_LTC1660.h"
#include "app_TLC5916.h"
#include "app_algorithm.h"
#include <app_DSP_algorithm.h>
#include "timer_function.h"


#include "stm32h5xx_hal.h"
#include "tim.h"
#include "fdcan.h"
#include "adc.h"

static char data_to_send[100];
float f_to_send;
uint16_t flash_check_crc;
uint16_t event_check_crc;
uint8_t debug = 0;
uint32_t current_cycle = 0;
uint8_t pippo = 0;

void app_init() {
//	 HAL_Delay(5000); // ToDo inserire logica di controllo XOR_RB

//	HAL_Delay(2000);
	enable_serial_logging();
	if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE,
			0) != HAL_OK) {
		// Notification Error
		Error_Handler();
	}
//
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);

	init_serial();
	init_adc(ADC_1, &hadc1);
	init_adc(ADC_2, &hadc2);

//	if (init_adc(ADC_2, &hadc2)!= ADC_OK)
//		MS_=EXTERNAL;
	HAL_Delay(1000);
//
	app_dac_init();

	sensor_init();
	threshold_init();

	dsp_init();
	dsp_threshold_init();

	app_TLC5916_init();

// TODO: inserire i TIM HAL_TIM_Base_Start_IT(&htim2);
// TODO: inserire i PWM HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

}

void app_main() {
	uint32_t current_cycle_counter_ = 0;
	uint32_t adc_values[NUM_SENS] = { 0 };
	uint8_t tlc_status = 0;

	while (1) {
// 		ToDo: poll_serial();
		switch (MS_) {
//			case PRE_SETUP:
//				break;

		case SETUP:
//			TODO: if (calibration_init() != DAC_CAL_OK) {
//				send_event(EVENT_FAULT_CAL);
//				MS_ = FAULT;
//				break;
//			}
			calibration_init();
			if (timer_on(&htim2) != HAL_OK) {
				MS_ = FAULT;
				break;
			}
			if (timer_on(&htim3) != HAL_OK) {
				MS_ = FAULT;
				break;
			}
			if (timer_on(&htim5) != HAL_OK) {
				MS_ = FAULT;
				break;
			}
			MS_ = RUNNING;
			break;

		case RUNNING:
//			sprintf(data_to_send, "comando = %d\r\n", signal_selected_);
//			UART_Print(data_to_send);
//			HAL_Delay(200);
//			for (int i = 0; i < 4; i++) {
//				app_TLC5916_TurnOnLed(i);
//				HAL_Delay(100);
//			}
//			for (int i = 0; i < 4; i++) {
//				app_TLC5916_TurnOffLed(i);
//				HAL_Delay(100);
//			}

//			app_TLC5916_setAll(255);
//			tlc_status = app_get_tlc_status();
//			sprintf(data_to_send, "tlc_status = %d\r\n", tlc_status);
//			UART_Print(data_to_send);
//			HAL_Delay(2000);
//			app_TLC5916_setAll(0);
//			tlc_status = app_get_tlc_status();
//			sprintf(data_to_send, "tlc_status = %d\r\n", tlc_status);
//			UART_Print(data_to_send);
//			HAL_Delay(2000);

//			for (int i = 0; i < NUM_DSP; i++) {
//				print_DSP(i);
//				HAL_Delay(100);
//			}
//			for (int i = 0; i < NUM_SENS; i++) {
//				adc_values[i] = read_adc(0, i);
//			}
//			sprintf(data_to_send,
//						"adc_values = [ %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu ]\r\n",
//						adc_values[0], adc_values[1],
//						adc_values[2], adc_values[3],
//						adc_values[4], adc_values[5],
//						adc_values[6], adc_values[7],
//						adc_values[8], adc_values[9],
//						adc_values[10], adc_values[11]);
//			UART_Print(data_to_send);
			break;

		case EXTERNAL:

			break;

		case EXPIRED:
			break;

		case FAULT:
//			print_DSP(0);
//			HAL_Delay(100);
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
