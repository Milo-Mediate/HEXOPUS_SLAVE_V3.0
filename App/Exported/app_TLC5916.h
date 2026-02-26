/*
 * app_TLC5916.h
 *
 *  Created on: 19 feb 2026
 *      Author: emili
 */

#ifndef EXPORTED_APP_TLC5916_H_
#define EXPORTED_APP_TLC5916_H_
#include "TLC5916.h"
#include "app_hw_definition.h"
#include "spi.h"

void app_TLC5916_init();
uint8_t app_get_tlc_status();
TLC5916_error_t app_TLC5916_TurnOnLed(uint8_t sens);
TLC5916_error_t app_TLC5916_TurnOffLed(uint8_t sens);
TLC5916_error_t app_TLC5916_setAll(uint8_t status);


#endif /* EXPORTED_APP_TLC5916_H_ */
