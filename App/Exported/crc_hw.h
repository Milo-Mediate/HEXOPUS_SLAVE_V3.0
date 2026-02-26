#ifndef __CRC_HW_H
#define __CRC_HW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/**
 * @brief  Calculates the CRC-16-IBM of a data block using the STM32H563 CRC hardware module.
 * @details Configures the CRC module with polynomial 0x8005 (CRC-16-IBM), initial value 0xFFFF,
 *          and computes the CRC on a byte stream. Suitable for both flash data (e.g., 1 kB struct
 *          aligned to 32 bits) and UART packets with variable length. The CRC module is reset
 *          before each calculation to ensure consistency.
 * @param  data Pointer to the data block (struct or array).
 * @param  length Length of the data block in bytes.
 * @retval uint16_t Calculated CRC-16 value.
 * @note   Ensure the CRC module clock is enabled (RCC_AHB4ENR_CRCEN). This function enables it automatically.
 * @note   The input struct must be 32-bit aligned, and any padding bytes must be initialized to 0 to avoid inconsistent results.
 * @note   The CRC module is de-initialized and its clock disabled after calculation to save power.
 */
extern uint16_t calculate_crc16_hw(const uint8_t *data, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* __CRC_HW_H */
