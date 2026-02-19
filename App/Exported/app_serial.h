#ifndef __APP_SERIAL_H
#define __APP_SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

extern void init_serial();
extern void poll_serial();
extern void on_new_serial_data();

#ifdef __cplusplus
}
#endif

#endif /* __APP_SERIAL_H */
