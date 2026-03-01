#include "flash_store.h"

#include <string.h>


HAL_StatusTypeDef flash_store_write(
    uint32_t address, uint8_t *data, uint32_t size, FLASH_EraseInitTypeDef *erase_init
) {
    HAL_StatusTypeDef status;
    uint32_t sector_error;
    // FLASH_EraseInitTypeDef erase_init;

    // 1. Unlock Flash
    HAL_FLASH_Unlock();

    // 2. Erase Sector
    erase_init->TypeErase = FLASH_TYPEERASE_SECTORS;
    erase_init->NbSectors = 1;

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

    status = HAL_FLASHEx_Erase(erase_init, &sector_error);
    if (status != HAL_OK) {
        HAL_FLASH_Lock();
        return status;
    }

    // 3. Program Data in 128-bit (16-byte) blocks
    for (uint32_t i = 0; i < size; i += FLASH_ALIGNMENT) {
        // H5 uses QUADWORD (128-bit) programming
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, address + i, (uint32_t)(data + i));

        if (status != HAL_OK) {
            break;
        }
    }

    // 4. Lock Flash
    HAL_FLASH_Lock();
    return status;
}

HAL_StatusTypeDef flash_store_read(uint32_t address, uint8_t *data, uint32_t size)
{
    memcpy(data, (void *)address, size);
    return HAL_OK;
}
