#include "serial.h"
#include <string.h>

#define T_MSG_MS 1000u

static void reset_active_frame(BufferedSerial *ser)
{
    SerialFrame *active = slq_get_active_frame(&ser->lines);
    active->length = 0;
    ser->overflowed = false;
}

static SerialStatus append_to_frame(BufferedSerial *ser, uint8_t b)
{
    if (ser->overflowed) return SERIAL_ERR_OVERFLOW;

    SerialFrame *active = slq_get_active_frame(&ser->lines);
    if (active->length >= UART_BUFFER_IN) {
        ser->overflowed = true;
        return SERIAL_ERR_OVERFLOW;
    }

    active->data[active->length++] = b;
    return SERIAL_OK;
}

bool serial_init(BufferedSerial *ser, UART_HandleTypeDef *huart, OnNewLine cb)
{
    if (!ser || !huart || !cb) return false;

    memset(ser, 0, sizeof(BufferedSerial)); // Crucial for passing the test
    ser->huart = huart;
    ser->on_new_line = cb;
    ser->last_tick = HAL_GetTick();

    slq_init(&ser->lines);
    reset_active_frame(ser); // Ensures active->length = 0

    return (HAL_UART_Receive_IT(ser->huart, ser->rx_data, 1) == HAL_OK);
}

SerialStatus serial_process_char(BufferedSerial *ser, uint8_t b)
{
    // 1. Line Completion Logic
    if (ser->prev == '\r' && b == '\n') {
        ser->prev = 0;
        SerialFrame *active = slq_get_active_frame(&ser->lines);

        // Strip the trailing '\r' if we aren't overflowed
        if (!ser->overflowed && active->length > 0) {
            active->length--;
        }

        bool pushed = false;
        if (!ser->overflowed) {
            pushed = slq_commit_active_frame(&ser->lines);
        }

        reset_active_frame(ser);
        return pushed ? SERIAL_OK : SERIAL_ERR_QUEUE_FULL;
    }

    // 2. Regular character accumulation
    ser->prev = b;
    return append_to_frame(ser, b);
}

void serial_on_byte_received(BufferedSerial *ser)
{
    uint32_t now = HAL_GetTick();
    if ((now - ser->last_tick) > T_MSG_MS) {
        reset_active_frame(ser);
        ser->prev = 0;
    }
    ser->last_tick = now;

    serial_process_char(ser, ser->rx_data[0]);

    if (HAL_UART_Receive_IT(ser->huart, ser->rx_data, 1) != HAL_OK) {
        ser->rearm_errors++;
    }
}

void serial_poll(BufferedSerial *ser)
{
    SerialFrame *frame;
    while ((frame = slq_pop_ready_frame(&ser->lines)) != NULL) {
        ser->on_new_line(frame->data, frame->length);
        slq_release_frame(&ser->lines);
    }
}
