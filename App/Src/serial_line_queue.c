#include <stdbool.h>

#include "serial_line_queue.h"
#include "stm32h5xx.h"

void slq_init(SerialLineQueue *q)
{
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    q->drops = 0;
    for (int i = 0; i < SLQ_SIZE; i++) {
        q->frames[i].length = 0;
    }
}

SerialFrame* slq_get_active_frame(SerialLineQueue *q)
{
    // The ISR always works on the 'tail' index.
    // In an N+1 buffer, the tail index is guaranteed to be
    // private to the ISR as long as count < SLQ_SIZE.
    return &q->frames[q->tail];
}

bool slq_commit_active_frame(SerialLineQueue *q)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();

    // 1. Check if we have reached the logical capacity
    if (q->count >= PORT_BUFFERS) {
        q->drops++;
        __set_PRIMASK(primask);
        return false;
    }

    // 2. Move the tail using the PHYSICAL size for the modulo
    q->tail = (uint8_t)((q->tail + 1u) % SLQ_SIZE);
    q->count++;

    __set_PRIMASK(primask);
    return true;
}

SerialFrame* slq_pop_ready_frame(SerialLineQueue *q)
{
    // No IRQ disable needed for a simple read check
    if (q->count == 0)
        return NULL;

    return &q->frames[q->head];
}

void slq_release_frame(SerialLineQueue *q)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();

    if (q->count > 0) {
        // 3. Move the head using the PHYSICAL size for the modulo
        q->head = (uint8_t)((q->head + 1u) % SLQ_SIZE);
        q->count--;
    }

    __set_PRIMASK(primask);
}
