#include "app_serial.h"
#include "serial.h"
#include "serial_manager.h"


static BufferedSerial serial;

static inline int on_new_line(const uint8_t *buffer, size_t buffer_len)
{
	const SerialMsgStatus status = process_serial_data(buffer, buffer_len);
	// if (status != MSG_OK) {
	// 	//  ToDo
	// }
    return status;
}

void init_serial()
{
	serial_init(&serial, &huart1, on_new_line);
}

void poll_serial()
{
	serial_poll(&serial);
}

void on_new_serial_data()
{
	serial_on_byte_received(&serial);
}
