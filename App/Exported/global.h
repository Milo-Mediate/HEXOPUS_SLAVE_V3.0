
/**
 * @file global.h
 * @brief Global handles, flags, buffers and init routines for MASTER/SLAVE builds.
 * @details
 * This header exposes peripherals handles, shared buffers/flags, and
 * helper APIs declared/defined in global.c. It supports both MASTER and SLAVE
 * build variants through preprocessor switches.
 *
 * ## Build-time roles
 * - `MASTER` → coordinator features (fleet, OSSD, RTC printout).
 * - `!MASTER` → sensor/DSP node with ADC, DAC and Goertzel path.
 *
 * @note Replace or ensure inclusion of the proper HAL headers through @ref main.h.
 * @author milo
 * @date   2025-03-20
 */
#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"     /* HAL types (ADC/FDCAN/I2C/SPI/TIM/UART/DMA) */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "definition.h"

#ifndef MASTER
//#include "arm_math.h"
#endif

/* -------------------------------------------------------------------------- */
/*                           Peripheral handlers                              */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                                   Flash                                    */
/* -------------------------------------------------------------------------- */
extern HAL_StatusTypeDef   status_;
extern uint8_t             flash_operations_;

/* -------------------------------------------------------------------------- */
/*                               CAN buffers                                  */
/* -------------------------------------------------------------------------- */
extern FDCAN_TxHeaderTypeDef TxHeader_;
extern FDCAN_RxHeaderTypeDef RxHeader_;
extern uint8_t               TxData_[64];
extern uint8_t               RxData_[64];

/* -------------------------------------------------------------------------- */
/*                           Serial (UART) buffers                             */
/* -------------------------------------------------------------------------- */
// SERIAL
#define UART_BUFFER_IN 100
#define UART_BUFFER_OUT 1000

extern uint8_t  rx_interrupt_flag_;
extern uint8_t  requesting_data_;
extern uint8_t  rx_data_[2];
extern int      rx_counter_;
extern uint8_t  rx_buffer_in_[UART_BUFFER_IN];
extern uint8_t  rx_buffer_out_[UART_BUFFER_OUT];
extern uint8_t  global_data_buffer_in_[UART_BUFFER_IN];
extern uint8_t  global_data_buffer_out_[UART_BUFFER_OUT];

/* -------------------------------------------------------------------------- */
/*                           Debug / system flags                              */
/* -------------------------------------------------------------------------- */
extern uint32_t tick_count_;
extern uint8_t  flag_serial_plot_;
extern uint8_t  flag_can_plot_;

extern uint8_t  sensor_selected_;
extern uint8_t  signal_selected_;


/* -------------------------------------------------------------------------- */
/*                           Machine-Status                                    */
/* -------------------------------------------------------------------------- */
extern uint8_t  MS_;


/* -------------------------------------------------------------------------- */
/*                           MASTER-specific section                           */
/* -------------------------------------------------------------------------- */
#ifdef MASTER
extern uint8_t   flag_all_ready_;
extern uint8_t   flag_end_cal_;

extern uint8_t   device_configuration_;
extern uint8_t   OSSD_1_STATUS_;
extern uint8_t   OSSD_2_STATUS_;
extern uint8_t   check_ossd_1_;
extern uint8_t   check_ossd_2_;
extern uint8_t   count_check_;
extern uint8_t   count_recive_;
extern uint16_t  dev_installed_;
extern uint16_t  dev_scanned_;
extern uint8_t   count_dev_connected_;
extern uint16_t  master_cal_status_;
extern uint8_t   count_dev_ready_;
extern uint8_t   status_1_;
extern uint8_t   status_2_;
extern uint8_t   flag_stop_ossd_1_;
extern uint8_t   flag_stop_ossd_2_;

/* Master parameters */
extern float     master_k_e_th_[MAX_DEV * NUM_SENS];
extern float     master_gains_[MAX_DEV * NUM_SENS];
extern uint16_t  master_numCycle_[MAX_DEV * NUM_SENS];
extern float     master_deltas_[MAX_DEV * NUM_SENS];

/* Plot helpers & debug values */
extern float     signal_to_plot_[8];
extern float     master_moving_average_;
extern float     master_threshold_;
extern float     master_rms_;
extern float     master_threshold_DSP_pre_stop_;
extern float     master_threshold_DSP_stop_;

extern uint8_t   device_selected_;

/* DS1302 buffers */
extern char      time_buffer_out_[100];
extern uint8_t   TxTime_[7];
extern uint8_t   RxTime_[7];

/**
 * @brief Convert DS1302 raw time (7 bytes) to formatted ASCII (time_buffer_out_).
 * @param[in] buf Pointer to {hh,mm,ss,wd,dd,mm,yy}.
 */
void time_to_string(uint8_t *buf);
#endif /* MASTER */

/* Optional HAL handle exposed elsewhere */
#ifdef HEXOPAD
extern DAC_HandleTypeDef   hdac1;
#endif

/* -------------------------------------------------------------------------- */
/*                         HEXOPUS / DSP status arrays                         */
/* -------------------------------------------------------------------------- */
#ifdef HEXOPUS
extern uint16_t            dac_values_[NUM_DAC_CHANNELS];
extern uint8_t             status_DSP_pre_stop_[NUM_SENS];
extern uint8_t             status_DSP_stop_[NUM_SENS];
extern uint8_t             led_status_[NUM_SENS];
#endif

/* -------------------------------------------------------------------------- */
/*                            SLAVE-specific section                           */
/* -------------------------------------------------------------------------- */
#ifndef MASTER
extern uint8_t   flag_slave_connected_;
extern uint16_t  SLAVE_ID_;

extern uint32_t  adc1_data_[NUM_SENS];
extern uint32_t  adc2_data_[NUM_SENS];

extern uint8_t   flag_auto_set_dac_;
extern uint16_t  flag_manual_set_dac_;

extern uint8_t   flag_calculate_th_dsp_;
extern uint8_t   flag_th_DSP_[NUM_SENS];

extern uint8_t   flag_msg_sensor_sent_;
extern uint8_t   flag_msg_DSP_pre_stop_sent_;
extern uint8_t   flag_msg_DSP_stop_sent_;

/** @brief One byte per sensor: each element represents the stop status. */
extern uint8_t   status_sensor_stop_[NUM_SENS];
#endif /* !MASTER */

/* -------------------------------------------------------------------------- */
/*                                   APIs                                     */
/* -------------------------------------------------------------------------- */
/**
 * @brief System initialization sequence.
 * @details
 * - MASTER: high-level state machine (Flash → scan → verify → calibrate).
 * - SLAVE : compute Goertzel coeff, init AD9833, set DSP defaults.
 */
void     system_init(void);

/**
 * @brief Reconstruct a float from 4 bytes (little-endian).
 * @param[in] buf Pointer to input buffer (size >= 4). Must not be NULL.
 * @return Float value; 0.0f if buf is NULL.
 */
float    bytes_to_float(uint8_t *buf);

/**
 * @brief Reconstruct a uint16_t from 2 bytes (little-endian).
 * @param[in] buf Pointer to input buffer (size >= 2). Must not be NULL.
 * @return 16-bit value; 0U if buf is NULL.
 */
uint16_t bytes_to_int(uint8_t *buf);

/**
 * @brief Count number of set bits (population count) in a 64-bit value.
 * @param[in] val Input value.
 * @return Number of 1-bits (0..64).
 */
uint8_t  count_ones(uint64_t val);

#ifdef __cplusplus
}
#endif

#endif /* GLOBAL_H */
