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
#include "app_DSP_algorithm.h"
#include "timer_function.h"
#include "machine_state.h"
#include "flash_management.h"

#include "stm32h5xx_hal.h"
#include "tim.h"
#include "fdcan.h"
#include "adc.h"

//static char data_to_send[100];
float f_to_send;
uint16_t flash_check_crc;
uint16_t event_check_crc;
uint8_t debug = 0;
uint32_t current_cycle = 0;
static uint8_t sens = 0;
uint8_t pippo = 0;

void app_init() {
//	 HAL_Delay(5000); // ToDo inserire logica di controllo XOR_RB

//	HAL_Delay(2000);
	machine_state_init();

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

	/** @brief Initializes flash_data_primary and flash_data_backup with zeros and sets initial values. */
	ln_info("Init Flash");

	/** @brief Reads MasterFlashData_t struct from flash memory. */
	flash_read();
	/** @brief Initializes the flash if it has not been initialized yet. */
	if (!flash_is_init()) {
		flash_init();
		ln_info("OK: flash_init");
		HAL_Delay(2000);
		NVIC_SystemReset();
	}
	set_log_level(flash_data_write_.log_level);

	flash_check_crc = flash_calculate_crc(&flash_data_read_);
	if (flash_check_crc != flash_data_read_.crc16) {
		set_machine_state(FAULT);
		ln_info("Flash Fault");
		lf_info("CRC Read %d", flash_data_read_.crc16);
		lf_info("CRC Computation %d", flash_check_crc);
	} else {
		ln_info("Flash Test OK");
		if (flash_data_write_.length < MasterFlashData_DataLen) {
			flash_write();  // Updates length and crc
		}
	}

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
	set_machine_state(SETUP);

// TODO: inserire i TIM HAL_TIM_Base_Start_IT(&htim2);
// TODO: inserire i PWM HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

}

void app_main() {
//	uint32_t current_cycle_counter_ = 0;
//	uint32_t adc_values[NUM_SENS] = { 0 };
//	uint8_t tlc_status = 0;

	while (1) {
		poll_serial();
		switch (get_machine_state()) {
//			case PRE_SETUP:
//				break;

		case SETUP:
			calibration_init();
			if (timer_on(&htim2) != HAL_OK) {
				set_machine_state(FAULT);
				break;
			}
			if (timer_on(&htim3) != HAL_OK) {
				set_machine_state(FAULT);
				break;
			}
//			if (timer_on(&htim5) != HAL_OK) {
//				set_machine_state(FAULT);
//				break;
//			}
			HAL_Delay(1000);
//			for (uint8_t i = 0; i < NUM_DSP; i++)
//			{
//				enable_threshold(2*i);
////				set_outdated_dsp_th(i);
//			}
			set_outdated_dsp_th(1);
//			set_outdated_dsp_th(2);
			set_outdated_dsp_th(3);
			set_outdated_dsp_th(4);
			set_outdated_dsp_th(5);
			HAL_Delay(500);
//			enable_dsp_threshold(1);
//			enable_dsp_threshold(3);
			enable_dsp_threshold(4);
			enable_dsp_threshold(5);
			HAL_Delay(500);
			timer_on(&htim5);
			set_machine_state(RUNNING);

			break;

		case RUNNING:
//			HAL_Delay(1000);
//			lf_info("sensor: 0, value: %.1f", delta_to_check);

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
			if (!get_msg_1_sent())
			{
//				send_stop(SLAVE_ID_1);
				set_msg_1(true); //todo da mettere nella funzione
			}
			if (!get_msg_2_sent())
			{
//				send_stop(SLAVE_ID_2);
				set_msg_2(true);
			}
			HAL_Delay(1000);
			if (!get_stop_1() && (!get_stop_2()))
				set_machine_state(RUNNING);
			break;
		case SEND_PARAMS:
			send_params(SLAVE_ID_1, sens);
			sens += 1;
			HAL_Delay(20);
			if (sens == 6) {
				set_machine_state(RUNNING);
			}
			break;
		case SLAVE_RESET:
			flash_write();
			NVIC_SystemReset();
			break;
		default:
			break;
		}
	}
}
