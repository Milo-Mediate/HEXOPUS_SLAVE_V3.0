/*
 * TLC5916.h
 *
 *  Created on: 17 feb 2026
 *      Author: emili
 */

#ifndef EXPORTED_TLC5916_H_
#define EXPORTED_TLC5916_H_

void TLC5916_Init();
HAL_StatusTypeDef TLC5916_Write_Status(SPI_HandleTypeDef *hspi, const uint8_t new_status);
uint8_t TLC5916_Read_Status();
HAL_StatusTypeDef Turn_On_Led(SPI_HandleTypeDef *hspi, const uint8_t idx);
HAL_StatusTypeDef Turn_Off_Led(SPI_HandleTypeDef *hspi, const uint8_t idx);

#endif /* EXPORTED_TLC5916_H_ */
