#ifndef SERIAL_H
#define SERIAL_H

#include <stdbool.h>
#include "usart.h"
#include "serial_line_queue.h"

typedef enum {
    SERIAL_OK,
    SERIAL_ERR_OVERFLOW,
    SERIAL_ERR_QUEUE_FULL
} SerialStatus;

typedef int (*OnNewLine)(const uint8_t *buffer, size_t len);

typedef struct {
    UART_HandleTypeDef *huart;
    OnNewLine on_new_line;

    uint8_t  rx_data[1];
    uint8_t  prev;
    bool     overflowed;
    uint32_t last_tick;

    volatile uint32_t rearm_errors;
    SerialLineQueue lines;
} BufferedSerial;

bool serial_init(BufferedSerial *ser, UART_HandleTypeDef *huart, OnNewLine cb);
void serial_on_byte_received(BufferedSerial *ser);
void serial_poll(BufferedSerial *ser);

// Exposed for Unit Testing on PC
SerialStatus serial_process_char(BufferedSerial *ser, uint8_t b);

#endif
