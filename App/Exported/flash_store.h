
#ifndef FLASH_STORE_H
#define FLASH_STORE_H

#include <stdint.h>
#include "stm32h5xx_hal.h"

#define FLASH_ALIGNMENT 16

HAL_StatusTypeDef flash_store_write(
    uint32_t address, uint8_t *data, uint32_t size, FLASH_EraseInitTypeDef *erase_init
);
HAL_StatusTypeDef flash_store_read(
    uint32_t address, uint8_t *data, uint32_t size
);

#endif // FLASH_STORE_H
