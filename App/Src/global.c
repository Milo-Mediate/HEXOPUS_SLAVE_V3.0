/**
 * @file global.c
 * @brief Global functions, handles and shared flags for MASTER/SLAVE builds.
 * @details
 * Aggregates HAL handles, global flags/buffers, helpers, and the high-level
 * `system_init()` bring-up sequence (peripherals, AD9833, DSP defaults).
 *
 * ## Build-time roles
 * - `MASTER` → coordinator features (fleet, OSSD, RTC printout).
 * - `!MASTER` → sensor/DSP node with ADC, DAC and Goertzel path.
 * - `HEXOPAD` / `HEXOPUS` enable specific peripherals/features.
 *
 * ## Concurrency
 * - Several globals are accessed in ISR and thread contexts; protect multi-byte
 *   variables and shared flags where needed (IRQ masking or a small critical section).
 *
 * ## MISRA Notes
 * - Function prototypes use `void` when no parameters (e.g. `system_init(void)`).
 * - Added basic guards: NULL/range checks in helpers (`bytes_to_*`, `time_to_string`).
 * - Use of `memcpy` avoids aliasing issues (permitted, safer than casts).
 * - Integer literals annotated with `U` where appropriate; float with `f`.
 * - `sprintf` used only with controlled buffers; caller ensures sizes; consider `snprintf`.
 * - Magic numbers kept for compatibility but marked as defaults; consider moving to config.
 *
 * @author milo
 * @date   2025-03-20
 */

#include "global.h"
#include "can_manager.h"
#include "serial_manager.h"
#include <string.h>   /* memcpy */
#include <stdio.h>    /* sprintf */
#include <stdbool.h>

#ifdef HEXOPUS
#include "LTC1660.h"
#endif

#ifndef MASTER
#include "buffer_manager.h"
#endif

/* DMA channels */
/** @brief DMA node for GPDMA1 channel 7 */
DMA_NodeTypeDef Node_GPDMA1_Channel7;
/** @brief DMA list for GPDMA1 channel 7 */
DMA_QListTypeDef List_GPDMA1_Channel7;
/** @brief DMA handler for GPDMA1 channel 7 */
DMA_HandleTypeDef handle_GPDMA1_Channel7;
/** @brief DMA node for GPDMA2 channel 7 */
DMA_NodeTypeDef Node_GPDMA2_Channel7;
/** @brief DMA list for GPDMA2 channel 7 */
DMA_QListTypeDef List_GPDMA2_Channel7;
/** @brief DMA handler for GPDMA2 channel 7 */
DMA_HandleTypeDef handle_GPDMA2_Channel7;

/* =======================
 *  Flash memory
 * ======================= */

/** @brief Data structure to be written in flash memory */
FlashData_t flash_data_write_ = { .counter = 0, .status_flags = 0, .device_id =
		{ 0 }, .config_value = 0, .checksum = 0, .reserved = { 0 } };
/** @brief Data structure read from flash memory */
FlashData_t flash_data_read_ = { .counter = 0, .status_flags = 0, .device_id = {
		0 }, .config_value = 0, .checksum = 0, .reserved = { 0 } };
HAL_StatusTypeDef status_;
uint8_t flash_operations_;

/* =======================
 *  CAN Communication
 * ======================= */

/** @brief CAN transmission header */
FDCAN_TxHeaderTypeDef TxHeader_;
/** @brief CAN reception header */
FDCAN_RxHeaderTypeDef RxHeader_;
/** @brief CAN transmission buffer */
uint8_t TxData_[64] = { 0 };
/** @brief CAN reception buffer */
uint8_t RxData_[64] = { 0 };

/* =======================
 *  Serial Communication
 * ======================= */

/** @brief Temporary buffer for UART RX */
uint8_t rx_data_[2];
/** @brief Counter for received string length */
int rx_counter_ = 0;
/** @brief UART input buffer */
uint8_t rx_buffer_in_[UART_BUFFER_IN];
/** @brief Auxiliary UART input buffer */
uint8_t global_data_buffer_in_[UART_BUFFER_IN];

/* =======================
 *  Debug / System flags
 * ======================= */

/** @brief Global tick counter */
uint32_t tick_count_ = 0;
/** @brief Flag for serial plot activation */
uint8_t flag_serial_plot_ = 0;
/** @brief Flag for CAN plot activation */
uint8_t flag_can_plot_ = 0;

/** @brief Selected sensor for debugging */
uint8_t sensor_selected_ = 0;
/** @brief Selected signal for debugging */
uint8_t signal_selected_ = 0;

/* =======================
 *  Finite-State Machine
 * ======================= */

/** @brief Machine status */
uint8_t MS_ = PRE_SETUP;

/* =======================
 *  Master-specific variables
 * ======================= */

#ifdef HEXOPAD
extern DAC_HandleTypeDef hdac1;
#endif

#ifdef HEXOPUS
uint16_t dac_values_[NUM_DAC_CHANNELS];
uint8_t status_DSP_pre_stop_[NUM_SENS] = { 0U };
uint8_t status_DSP_stop_[NUM_SENS] = { 0U };
uint8_t led_status_[NUM_SENS] = { 0U };
#endif

#ifndef MASTER
/* ---------------- CAN (SLAVE) ---------------- */
uint8_t flag_slave_connected_ = 0U;
uint16_t SLAVE_ID_ = 0x010U; /* Loaded from FLASH at runtime */

/* ---------------- ADC raw buffers ---------------- */
uint32_t adc1_data_[NUM_SENS];
uint32_t adc2_data_[NUM_DSP];
bool flag_adc_new_sample_;

/* ---------------- DAC control ---------------- */
uint8_t flag_auto_set_dac_ = 0U;
uint16_t flag_manual_set_dac_ = 0U;

/* ---------------- DSP calibration ---------------- */
uint8_t flag_calculate_th_dsp_ = 0U;

uint8_t flag_th_DSP_[NUM_SENS] = { 0 };
uint8_t flag_msg_sensor_sent_ = 0U;
uint8_t flag_msg_DSP_pre_stop_sent_ = 0U;
uint8_t flag_msg_DSP_stop_sent_ = 0U;

/** @brief Bitmask: each element represents one sensor stop status. */
uint8_t status_sensor_stop_[NUM_SENS] = { 0 };
#endif /* !MASTER */

#ifndef MASTER
/* ------------------------------------------------------------------------- */
/**
 * @brief System initialization sequence (SLAVE build).
 * @details
 * - Computes Goertzel coefficient: `coeff_ = 2*cos(omega_)`.
 * - Initializes AD9833 (square wave @ 298 kHz, 0°).
 * - Assigns default DSP parameters per sensor and resets TH1/TH2 buffers.
 * - Leaves delays for power-up stability.
 *
 * @warning If called while IRQs are active, protect any code that shares
 *          globals with ISRs.
 */
void system_init(void) {
	char debug[50];
	float32_t tau_1_ = 0.001;
	float32_t tau_2_ = 0.05;
	float32_t tau_dsp_ = 0.002;

	/* Goertzel coefficient */
	coeff_ = arm_cos_f32(omega_) * 2.0f;

	HAL_Delay(1000U);
	AD9833_Init(1U, 298000.0f, 0.0f);
	(void) snprintf(debug, sizeof(debug), "AD9833 SET FREQ: 298000\r\n");
	UART_Print(debug);

	HAL_Delay(1000U);

	for (uint8_t i = 0U; i < NUM_SENS; i++) {
		gains_[i] = 4.0f;
		deltas_[i] = 3.0f;
		num_cycles_sizes_[i] = 100U;

		reset_TH1(i, 1024U);
		reset_TH2(i, 256U);

		ist_k_[i] = EMA_GetAlpha(tau_1_, TS);
		sec_k_[i] = EMA_GetAlpha(tau_2_, TS); //0.00019998000133325533f; // TAU = 100ms
		th_k_[i] = EMA_GetAlpha(0.25f, 0.02048f);

		dsp_k_[i] = EMA_GetAlpha(tau_dsp_, TS_DSP);

		thresholds_[i] = 4095;
	}

	flag_read_new_val_ = 0U;
	HAL_Delay(1000U);

	HAL_GPIO_WritePin(LTC1660_CLR_GPIO_Port, LTC1660_CLR_Pin, GPIO_PIN_SET);

	for (uint8_t i = 0U; i < NUM_SENS; i++) {
		auto_set_dac_value(i + 1U);
		HAL_Delay(10U);
	}

	HAL_Delay(1000U);
	(void) snprintf(debug, sizeof(debug), "CALIBRATION END\r\n");
	UART_Print(debug);

	flag_read_new_val_ = 1U;
}
#endif
/* ------------------------------------------------------------------------- */
/**
 * @brief Convert 4 bytes into an IEEE-754 float.
 * @param[in] buf Pointer to input buffer (size >= 4). Must not be NULL.
 * @return Reconstructed float; 0.0f if `buf` is NULL.
 * @note Uses `memcpy` to avoid aliasing/alignment issues.
 */
float bytes_to_float(uint8_t *buf) {
	if (buf == NULL) {
		return 0.0f;
	} /* MISRA guard */
	float f_aux = 0.0f;
	(void) memcpy(&f_aux, buf, sizeof(float));
	return f_aux;
}

/* ------------------------------------------------------------------------- */
/**
 * @brief Convert 2 bytes into a little-endian uint16_t.
 * @param[in] buf Pointer to input buffer (size >= 2). Must not be NULL.
 * @return Reconstructed value; 0U if `buf` is NULL.
 * @note Uses `memcpy` to avoid aliasing/alignment issues.
 */
uint16_t bytes_to_int(uint8_t *buf) {
	if (buf == NULL) {
		return 0U;
	} /* MISRA guard */
	uint16_t i_aux = 0U;
	(void) memcpy(&i_aux, buf, sizeof(uint16_t));
	return i_aux;
}

uint8_t count_ones(uint64_t val) {
	uint8_t count = 0;
	while (val) {
		count += val & 1;
		val >>= 1;
	}
	return count;
}
