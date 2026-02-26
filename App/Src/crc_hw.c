#include "crc_hw.h"

#include "stm32h5xx_hal.h"
#include "main.h"     /* HAL types (ADC/FDCAN/I2C/SPI/TIM/UART/DMA) */

uint16_t calculate_crc16_hw(const uint8_t *data, size_t length) {
    // Enable CRC clock
    __HAL_RCC_CRC_CLK_ENABLE();

    // CRC handle
    CRC_HandleTypeDef hcrc;
    hcrc.Instance = CRC;

    // Configure CRC module for CRC-16-IBM
    hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE; //DEFAULT_POLYNOMIAL_ENABLE;
    hcrc.Init.GeneratingPolynomial = 0x8005;                   // CRC-16-IBM polynomial
    hcrc.Init.CRCLength = CRC_POLYLENGTH_16B;                 // 16-bit CRC
    hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE; // Initial value 0xFFFF
    hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
    hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
    hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;        // Input as bytes

    // Initialize CRC module
    if (HAL_CRC_Init(&hcrc) != HAL_OK) {
        // Error handling (optional, customize as needed)
        __HAL_RCC_CRC_CLK_DISABLE();
        return 0;
    }

    // Calculate CRC
    uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)data, length);

    // De-initialize CRC module
    HAL_CRC_DeInit(&hcrc);

    // Disable CRC clock to save power
    __HAL_RCC_CRC_CLK_DISABLE();

    // Return 16-bit CRC
    return (uint16_t)(crc & 0xFFFF);
}
