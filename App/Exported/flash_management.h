#ifndef FLASH_MANAGEMENT_H
#define FLASH_MANAGEMENT_H

#include <assert.h>
#include <stddef.h>

#include "stm32h5xx_hal.h"
#include "definition.h"
#include "app_hw_definition.h"
#include "global.h"  // Include la struttura definita esternamente
#include "arm_math.h"

//#define FLASH_SECTOR_SIZE       (8192U)  // 8 KB
#define FLASH_START_SECTOR_ADDRESS 117
#define FLASH_NB_SECTORS 10
//#define BANK2_BASE_ADDR 0x08080000U
#define BASE_ADDR 0x08000000U
#define FLASH_VECTORS_SIZE 32

#define SETTINGS_FLASH_SECTOR  FLASH_START_SECTOR_ADDRESS
#define SETTINGS_FLASH_ADDR    (BASE_ADDR + (SETTINGS_FLASH_SECTOR * FLASH_SECTOR_SIZE))
#define SETTINGS_FLASH_BANK    FLASH_BANK_1


/**
 * @brief Struct for flash data, 128-bit aligned for STM32H563.
 * @note Fields are ordered to minimize padding. Size: 478 bytes (excluding flash_crc), 480 bytes total.
 * @note Padding bytes are initialized to 0 to ensure consistent CRC calculation.
 */
typedef struct {
	uint32_t magic;
	uint16_t length;
	uint16_t crc16;
	uint16_t version;

	// BOARD SETTINGS
	uint16_t serial_board_number_1; /**< Board serial number */
	uint16_t serial_board_number_2; /**< Board serial number */
	uint8_t num_sens;
	float32_t gains[NUM_SENS];
	float32_t deltas[NUM_SENS];
	uint16_t num_cycles[NUM_SENS];

	uint8_t num_dsp;
	float32_t th_dsp_1[NUM_DSP];
	float32_t th_dsp_2[NUM_DSP];
	float32_t dsp_gains_1[NUM_DSP];
	float32_t dsp_gains_2[NUM_DSP];

	uint8_t log_level;
} __attribute__((packed, aligned(FLASH_ALIGNMENT))) MasterFlashData_t;

#define MasterFlashData_HeaderLen (             \
    sizeof_member(MasterFlashData_t, magic)     \
    + sizeof_member(MasterFlashData_t, length)  \
    + sizeof_member(MasterFlashData_t, crc16)   \
)

#define MasterFlashData_DataLen (sizeof(MasterFlashData_t) - MasterFlashData_HeaderLen)

/**
 * @brief Global flash data instance for primary configuration, Used to store values read from flash.
 */
extern MasterFlashData_t flash_data_read_;

/**
 * @brief Global flash data instance for backup or temporary configuration. Used to change values which will be written in the flash.
 */
extern MasterFlashData_t flash_data_write_;

/**
 * @brief Initializes MasterFlashData_t with zeros to avoid uninitialized data.
 * @param flash_data Pointer to the MasterFlashData_t struct.
 */

bool flash_is_init();
uint16_t flash_calculate_crc(const MasterFlashData_t *mfd);
uint16_t flash_update_crc(MasterFlashData_t *mfd);

HAL_StatusTypeDef flash_init();
HAL_StatusTypeDef flash_write();
HAL_StatusTypeDef flash_read();

HAL_StatusTypeDef flash_init_mfd(MasterFlashData_t *mfd);
HAL_StatusTypeDef flash_write_mfd(MasterFlashData_t *mfd);
HAL_StatusTypeDef flash_read_mfd(MasterFlashData_t *mfd);

#endif // FLASH_MANAGEMENT_H
