#include "flash_management.h"

#include <string.h>

#include "crc_hw.h"
#include "logging.h"
#include "flash_store.h"
#include "app_algorithm.h"
#include "app_DSP_algorithm.h"
// _Static_assert(sizeof(MasterFlashData_t) == FLASH_SECTOR_SIZE-7840, "Unexpected struct size change!");
// _Static_assert(sizeof(MasterFlashData_t) % FLASH_ALIGNMENT == 0, "Struct size must be a multiple of 16 for H5 Flash ECC");
// _Static_assert(!has_tail_padding(MasterFlashData_t, flash_crc), "Implicit tail padding detected!");

MasterFlashData_t flash_data_read_;
MasterFlashData_t flash_data_write_;


uint16_t flash_calculate_crc(const MasterFlashData_t *mfd)
{
	return calculate_crc16_hw(
		&((uint8_t*)mfd)[MasterFlashData_HeaderLen],
		mfd->length
	);
}

uint16_t flash_update_crc(MasterFlashData_t *mfd)
{
	mfd->crc16 = flash_calculate_crc(mfd);
	return mfd->crc16;
}

/**
 * @brief Initializes MasterFlashData_t with zeros to avoid uninitialized data.
 * @param flash_data Pointer to the MasterFlashData_t struct.
 */
HAL_StatusTypeDef flash_init_mfd(MasterFlashData_t *mfd)
{
	memset(mfd, 0, sizeof(MasterFlashData_t));

    mfd->magic = MAGIC_FOCACCIA;
    mfd->length = MasterFlashData_DataLen;
    mfd->version = 1;

    mfd->serial_board_number_1 = 0x10;
    mfd->serial_board_number_2 = 0x20;
    for (uint8_t i = 0; i < NUM_SENS; i++)

    {
    	mfd->gains[i] = DEFAULT_GAIN;
    	mfd->deltas[i] = DEFAULT_DELTA;
    	mfd->num_cycles[i] = DEFAULT_NUM_CYCLE;
    }

    for (uint8_t i = 0; i < NUM_DSP; i++)
    {
    	mfd->th_dsp_1[i] = 0;
    	mfd->th_dsp_2[i] = 0;
    	mfd->dsp_gains_1[i] = DEFAULT_DSP_GAIN_1;
    	mfd->dsp_gains_2[i] = DEFAULT_DSP_GAIN_2;
    }
	mfd->log_level = log_level();

    mfd->crc16 = flash_calculate_crc(mfd);
    return flash_write_mfd(mfd);
}

HAL_StatusTypeDef flash_write_mfd(MasterFlashData_t *mfd)
{
    FLASH_EraseInitTypeDef erase_init = {
        .TypeErase = FLASH_TYPEERASE_SECTORS,
        .Banks     = SETTINGS_FLASH_BANK,
        .Sector    = SETTINGS_FLASH_SECTOR,
        .NbSectors = 1,
    };
    const uint32_t address = SETTINGS_FLASH_ADDR;
    const uint32_t size = sizeof(MasterFlashData_t);

	mfd->length = MasterFlashData_DataLen;
	flash_update_crc(mfd);

    return flash_store_write(address, (uint8_t *)mfd, size, &erase_init);
}

HAL_StatusTypeDef flash_read_mfd(MasterFlashData_t *mfd)
{
    return flash_store_read(SETTINGS_FLASH_ADDR, (uint8_t *)mfd, sizeof(MasterFlashData_t));
}

HAL_StatusTypeDef flash_init(void)
{
	HAL_StatusTypeDef status = flash_init_mfd(&flash_data_read_);

	if (status == HAL_OK)
		memcpy(&flash_data_write_, &flash_data_read_, sizeof(MasterFlashData_t));

	return status;
}

HAL_StatusTypeDef flash_write(void)
{
	HAL_StatusTypeDef status = flash_write_mfd(&flash_data_write_);

	if (status == HAL_OK)
		memcpy(&flash_data_read_, &flash_data_write_, sizeof(MasterFlashData_t));

	return status;
}

HAL_StatusTypeDef flash_read(void)
{
    HAL_StatusTypeDef status = flash_read_mfd(&flash_data_read_);

	if (status == HAL_OK)
		memcpy(&flash_data_write_, &flash_data_read_, sizeof(MasterFlashData_t));

	return status;
}

bool flash_is_init()
{
	return flash_data_read_.magic == MAGIC_FOCACCIA;
}
