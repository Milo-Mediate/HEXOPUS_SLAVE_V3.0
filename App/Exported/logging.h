#ifndef __LOGGING_H
#define __LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#define BUFFER_SIZE 256
#define UART_TIMEOUT 100

#define LL_FATAL    -3
#define LL_ERROR    -2
#define LL_WARNING  -1
#define LL_INFO      0
#define LL_DEBUG     1
#define LL_DETAIL    2

extern void enable_serial_logging();
extern void disable_serial_logging();

extern int log_level();
extern void set_log_level(int level);
extern void dlog(int level, const char *format, ...);

// Low level printing functions.
// These ignore the configured logging level!
extern void UART_nPrint(const char *str, size_t len);
extern void UART_Print(const char *str);

#define lf_fatal(format, ...) dlog(LL_FATAL, format, __VA_ARGS__)
#define lf_error(format, ...) dlog(LL_ERROR, format, __VA_ARGS__)
#define lf_warning(format, ...) dlog(LL_WARNING, format, __VA_ARGS__)
#define lf_info(format, ...) dlog(LL_INFO, format, __VA_ARGS__)
#define lf_debug(format, ...) dlog(LL_DEBUG, format, __VA_ARGS__)
#define lf_detail(format, ...) dlog(LL_DETAIL, format, __VA_ARGS__)

#define ln_fatal(format) dlog(LL_FATAL, format)
#define ln_error(format) dlog(LL_ERROR, format)
#define ln_warning(format) dlog(LL_WARNING, format)
#define ln_info(format) dlog(LL_INFO, format)
#define ln_debug(format) dlog(LL_DEBUG, format)
#define ln_detail(format) dlog(LL_DETAIL, format)

#ifdef __cplusplus
}
#endif

#endif /* __LOGGING_H */
