/**
 * @file serial_manager.h
 * @brief Serial communication manager (Master/Slave)
 *
 * This module handles UART communication, command parsing and interaction
 * with CAN, Flash, RTC, GPIO, and algorithm parameters.
 *
 * It provides functions to:
 * - Print messages over UART
 * - Process received serial commands
 * - Get device/sensor parameters
 * - Send firmware information
 * - Transmit float arrays via UART
 *
 * @author milo
 * @date 2025-03-20
 */

#include "definition.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Print an ACK message over UART.
 *
 * @param command The command connected to the function.
 */
extern void send_ack(uint8_t command);

typedef enum {
    MSG_OK,
    MSG_CRC_ERROR,
    MSG_UNKNOWN_CMD,
} SerialMsgStatus;

/**
 * @brief Process received serial data.
 *
 * Interprets incoming UART data and executes commands such as:
 * - Sensor, signal and device selection
 * - System commands (flash, datetime, calibration, CAN scan)
 * - Hardware control (OSSD, LEDs)
 * - Algorithm configuration (thresholds, gains, cycles)
 * - Debug/test commands
 *
 * @param buffer     Received UART package.
 * @param buffer_len Length of the received UART package.
 */
SerialMsgStatus process_serial_data(const uint8_t *buffer, size_t buffer_len);

/**
 * @brief Send firmware information via UART.
 *
 * Prints the current firmware version and basic system status.
 */
void get_info();

#ifdef MASTER
/**
 * @brief Print parameters of a specific sensor (Slave side).
 *
 * Prints gain, delta, buffer sizes and cycle number for the given sensor.
 *
 * @param sensor Sensor index.
 */
void get_parameter(uint8_t device, uint8_t sensor);
#endif


#ifndef MASTER
void get_parameter(uint8_t sensor);
#endif
