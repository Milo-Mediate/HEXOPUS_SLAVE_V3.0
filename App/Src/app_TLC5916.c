/*
 * app_TLC5916.c
 *
 *  Created on: 19 feb 2026
 *      Author: emili
 */
#include "app_TLC5916.h"
#include "stm32h5xx_hal.h"

TLC5916_t tlc;

void app_TLC5916_init()
{
	TLC5916_init(&hspi3, &tlc, TLC5916_OE_GPIO_Port, TLC5916_OE_Pin, TLC5916_LE_GPIO_Port, TLC5916_LE_Pin);
}

uint8_t app_get_tlc_status()
{
	return tlc.led_status;
}

TLC5916_error_t app_TLC5916_TurnOnLed(uint8_t sens)
{
	return TLC5916_TurnOnLed(&tlc, sens);
}

TLC5916_error_t app_TLC5916_TurnOffLed(uint8_t sens)
{
	return TLC5916_TurnOffLed(&tlc, sens);
}

TLC5916_error_t app_TLC5916_setAll(uint8_t status)
{
	return TLC5916_SetAll(&tlc, status);
}
