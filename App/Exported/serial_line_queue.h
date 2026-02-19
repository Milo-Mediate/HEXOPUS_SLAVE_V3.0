#ifndef SERIAL_LINE_QUEUE_H
#define SERIAL_LINE_QUEUE_H

#include <stddef.h>
#include <stdint.h>

#include "global.h" // PORT_BUFFERS and UART_BUFFER_IN defined here

#define PORT_BUFFERS 3
#define SLQ_SIZE (PORT_BUFFERS + 1)
// sizeof(((SerialLineQueue *)0)->frames)/sizeof(((SerialLineQueue *)0)->frames[0])

typedef struct {
    uint8_t  data[UART_BUFFER_IN];
    uint16_t length;
} SerialFrame;

typedef struct {
    volatile uint8_t head;
    volatile uint8_t tail;
    volatile uint8_t count;
    uint32_t drops;
    SerialFrame frames[SLQ_SIZE];
} SerialLineQueue;

void         slq_init(SerialLineQueue *q);
SerialFrame* slq_get_active_frame(SerialLineQueue *q);
bool         slq_commit_active_frame(SerialLineQueue *q);
SerialFrame* slq_pop_ready_frame(SerialLineQueue *q);
void         slq_release_frame(SerialLineQueue *q);

#endif
