/**
 * @file    definition.h
 * @brief   Global project definitions (STM32H563ZI)
 * @author milo
 * @date 2025-09-26
 *
 * @details
 *  This header collects:
 *   - Build configuration (MASTER/SLAVE, product variants)
 *   - System constants and buffer sizes
 *   - Identifiers and firmware version
 *   - Command map organized by category (System, Hardware, Algorithm, Debug)
 *
 *  Doxygen groups:
 *   - @ref cfg
 *   - @ref sizes
 *   - @ref ids
 *   - @ref cmd_sys, @ref cmd_hw, @ref cmd_alg, @ref cmd_dbg
 */

#ifndef DEFINITION_H
#define DEFINITION_H

#include <stdbool.h>


#define ON    1
#define OFF   0

// Source - https://stackoverflow.com/a/3437484
// Posted by David Titarenco, modified by community. See post 'Timeline' for change history
// Retrieved 2025-12-22, License - CC BY-SA 4.0

#define max(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a > _b ? _a : _b; })

#define min(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a < _b ? _a : _b; })


#define name(x) #x  // stringify
#define sizeof_member(type, member) (sizeof( ((type *)0)->member ))
#define has_tail_padding(type, last_member) (\
	offsetof(type, last_member) + sizeof_member(type, last_member) != sizeof(type)\
)
/* ===========================
 *  Group: Configuration
 * =========================== */
/** @defgroup cfg Build Configuration
 *  @brief Defines device role and product variant.
 *  @{
 */

/* Device role (define only one) */
// #define MASTER                    /**< Enable Master build */
// #define HEXOPAD                /**< Variant: 2 sensors */
#define HEXOPUS                /**< Variant: 8 sensors */

/** @} *//* end of cfg */

/* ===========================
 *  Group: Sizes
 * =========================== */
/** @defgroup sizes Sizes & Limits
 *  @brief Global size constants and runtime limits.
 *  @{
 */

#define MAX_SAMPLES           1000      /**< Maximum number of acquired samples */
#define MAX_WINDOW_SIZE       4096      /**< Maximum processing window size */
#define MAX_CYCLE_THRESHOLD 100000      /**< Maximum Number of while cycle allowed to go in FAULT status */

#define FLASH_ALIGNMENT         16

/** @} *//* end of sizes */

/* ===========================
 *  Group: Machine Status
 * =========================== */
/** @defgroup status Machine Status
 *  @brief System Machine State.
 *  @{
 */
typedef enum {
	IDLE = 0, /** */
	PRE_SETUP, /** */
	SETUP, /** */
	RUNNING, /** */
	STOP, /** */
	MUTING, /** */
	FAULT, /** */
	EXTERNAL, /** */
	EXPIRED,
} MachineStatus_t;

//#define PRE_SETUP         0
//#define SETUP             10
//#define RUN               20
//#define STOP              30
//#define MUTING            240
//#define FAULT             250
//#define EXTERNAL          255

/** @} *//* end of sizes */

/* ===========================
 *  Group: Identifiers
 * =========================== */
/** @defgroup ids Identifiers & Version
 *  @brief CAN IDs and firmware version.
 *  @{
 */

#define CAN_PLOT_ID         ((uint8_t)0xF8)   /**< Reserved ID for signal plotting */
#define VERSION             "HEXOPUS_SLAVE_V3.0"  /**< Firmware version string */

/* Sensor mapping depending on product variant */
#define MAX_SENS          16
#ifdef HEXOPUS
#define NUM_SENS           12
//#define NUM_DSP            1
#define NUM_DAC_CHANNELS   12
#define SAMPLING_FREQ      40000.0f
#define TARGET_FREQ        10000.0f
#endif
#ifdef HEXOPAD
#define NUM_DAC_CHANNELS  2
#define TS                0.00002f // Update Rate Time Reading from ADC (Tim Freq = 50KHz -> 20us)
#endif

#ifdef MASTER
/* Master-specific parameters */
#define NUM_SENS          			8           /**< Master handles 8 sensors per device */
#define SAFETY_OUTPUT_DRY_CONTACT	1
#define SAFETY_OUTPUT_OSSD_CONTACT	2
#define MAX_DEV         			32          /**< Maximum number of addressable devices */
#define BAT_CHANNEL  				ADC_CHANNEL_13
#define MAX_SENS                    8
#define MAX_SAFETY_OUTPUT           4
#define MUTING_IN_TIME 				500         /**< Max Counter to switch in or out the muting state */
#endif

/** @} *//* end of ids */

/* ===========================
 *  Group: System Commands
 * =========================== */
/** @defgroup cmd_sys System Commands
 *  @brief Commands for status, info, datetime, calibration, and parameters.
 *  @{
 */

/* Status & info */
#define CMD_READY                         21
#define CMD_GET_INFO                      22
#define CMD_GET_PARAMETERS                23
#define CMD_END_CAL                       24
#define CMD_SET_SLAVE_STATUS              25
#define CMD_GET_SLAVE_STATUS              26
#define CMD_SET_CAL_STATUS                27
#define CMD_GET_CAL_STATUS                28
#define CMD_SET_SEN_STATUS                29
#define CMD_GET_SEN_STATUS                30

#define CMD_GET_LOG_LEVEL                 83
#define CMD_SET_LOG_LEVEL                 84

#define CMD_HEARTBEAT                     255

/* Datetime (RTC) */
#ifdef MASTER
#define CMD_GET_BATT_VALUE                31
#define CMD_SET_DATETIME                  33
#define CMD_GET_DATETIME                  34
#endif

/* Master parameters & errors */
#ifdef MASTER
#define CMD_ERROR_ARRAY                   36
//#define CMD_GET_MASTER_PARAMETERS_SAVED   37

#endif

/* Master/Slave flash commands */

#define CMD_FLASH_WRITE                   200
#define CMD_FLASH_READ                    201
#define CMD_FLASH_ERASE                   202
#define CMD_SET_LED_DIAG_1                75
#define CMD_GET_LED_DIAG_1                76
#define CMD_SET_LED_DIAG_2                77
#define CMD_GET_LED_DIAG_2                78
#define CMD_SET_LED_MUTING                79
#define CMD_GET_LED_MUTING                80
#define CMD_SET_LED_SENSOR                81
#define CMD_GET_LED_SENSOR                82

/* Scan and distributed calibration (Master) */
#define CMD_CAN_SCAN                      146
#define CMD_CALIBRATE_SLAVE               147

/** @} *//* end of cmd_sys */

/* ===========================
 *  Group: Hardware Commands
 * =========================== */
/** @defgroup cmd_hw Hardware Commands
 *  @brief GPIO, LEDs, OSSD, DAC/potentiometers, and start/stop sequences.
 *  @{
 */
/* Safety sequences */
#define CMD_STOP                          36
#define CMD_PRE_STOP                      37
#define CMD_START                         38
#define CMD_PRE_START                     39


/* LEDs/OSSD (Master side) */
#ifdef MASTER
#define CMD_OSSD_STATUS           		  67
#endif

/* Actuators depending on product variant */
#define CMD_AUTOSET_V_REF_1               40
#define CMD_AUTOSET_V_REF_2               41
#define CMD_SET_V_REF_1                   52
#define CMD_GET_V_REF_1                   53
#define CMD_SET_V_REF_2                   54
#define CMD_GET_V_REF_2                   55

/** @} *//* end of cmd_hw */

/* ===========================
 *  Group: Algorithm Commands
 * =========================== */
/** @defgroup cmd_alg Algorithm Commands
 *  @brief Dynamic thresholds, gains, buffers, cycles, and streaming.
 *  @{
 */

#define CMD_SET_TH1                       122 // Fixed threshold used to manage 10Khz signal
#define CMD_GET_TH1                       123 // Fixed threshold used to manage 10Khz signal
#define CMD_SET_TH2                       124 // Fixed threshold used to manage 10Khz signal
#define CMD_GET_TH2                       125 // Fixed threshold used to manage 10Khz signal

#define CMD_SET_GAIN                      126
#define CMD_GET_GAIN                      127
#define CMD_SET_DELTA_V                   128
#define CMD_GET_DELTA_V                   129
#define CMD_SET_TAU_1                     130 // Works on ADC raw value in order to filter
#define CMD_GET_TAU_1                     131
#define CMD_SET_TAU_2                     132 // Works on ADC raw value in order to filter
#define CMD_GET_TAU_2                     133
#define CMD_SET_TAU_TH                    134 // Used to manage the refresh the 300Khz threshold value
#define CMD_GET_TAU_TH                    135
#define CMD_SET_NUM_CYCLE                 136
#define CMD_GET_NUM_CYCLE                 137

#define CMD_ALG_ON                        138
#define CMD_ALG_OFF                       139
#define CMD_DSP_ALG_ON                    140
#define CMD_DSP_ALG_OFF                   141

/* DSP / streaming */
#define CMD_PLOT_ON                       149
#define CMD_PLOT_OFF                      150
#define CMD_PLOT_SENSOR_1                 151
#define CMD_PLOT_SENSOR_2                 152
#define CMD_PLOT_SENSOR_3                 153
#define CMD_PLOT_SENSOR_4                 154
#define CMD_PLOT_SENSOR_5                 155
#define CMD_PLOT_SENSOR_6                 156

/** @} *//* end of cmd_alg */

/* ===========================
 *  Group: Debug Commands
 * =========================== */
/** @defgroup cmd_dbg Debug Commands
 *  @brief Utilities for testing over Serial and CAN.
 *  @{
 */

#define CMD_SERIAL_PLOT                   143
#define CMD_FDCAN_TX_DEBUG                144
#define CMD_RESET_SLAVE                   145
#define CMD_CAN_MSG_DEBUG                 254

/** @} *//* end of cmd_dbg */

/* ===========================
 *  Group: Selection Commands
 * =========================== */
/** @defgroup cmd_sel Selection Commands
 *  @brief Sensor, signal, and device selection via UART/CAN.
 *  @{
 */
#define CMD_SELECT_SENSOR               9
#define CMD_SELECT_SIGNAL               10
#ifdef MASTER
#define CMD_SELECT_DEVICE               11
#endif
/** @} *//* end of cmd_sel */

#define CMD_OPEN_CONNECTION_USER         1
#define CMD_OPEN_CONNECTION_PASSWORD     2
#define CMD_SERIAL_NUMBER                3
#define CMD_SAFETY_OUTPUT_TYPE           4
#define CMD_SAFETY_UPULSE_DELAY          5
#define CMD_SAFETY_UPULSE_ACT            6
#define CMD_SAFETY_UPULSE_DURATION       7
#define CMD_DEV_NUMB_INSTALLED           8
#define CMD_SLAVE_SETUP                 12
#define CMD_TIME_SETUP                  13
#define CMD_EXPIRATION_TIME             14
#define CMD_BATT_SETUP                  15
#define CMD_USER_SETUP                  16
#define CMD_USERNAME_SETUP              17
#define CMD_PASSWORD_SETUP              18
#endif /* DEFINITION_H */
