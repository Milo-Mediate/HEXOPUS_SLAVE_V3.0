#include "logging.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "main.h"

#include "usart.h"

#define str(s) #s

static bool _serial_connected = false;
static int _log_level = LL_INFO;

// ToDo
// static void sprintf_header(char *buffer)
// {
//     const char *time = // leggere l'orologio di sistema
//     sprintf(buffer, "[%s]:(%-*s):", time, padding, level_names[_log_level+1]);
// }

// ToDo: Aggiungere un buffer circolare per messaggi a dimensione variabile.
//       I nuovi messaggi vengono accodati e se non c'è spazio si cancellano i messaggi più vecchi.
//       Da capire se aggiungere una logica di priorità sul livello e se fare una struttura dedicata.

static int vlog(int level, const char *format, va_list vlist);

void enable_serial_logging()
{
    _serial_connected = true;
}

void disable_serial_logging()
{
    _serial_connected = false;
}

int log_level()
{
    return _log_level;
}

void set_log_level(int level)
{
    if (level < LL_FATAL || level > LL_DETAIL) {
        dlog(LL_ERROR, "Log level not changed");  // ToDo: definire un buon messaggio d'errore
        return;
    }
    _log_level = level;
}

void dlog(int level, const char *format, ...)
{
    // ToDo: aggiungere un header con l'orario e il livello di log
    if (_serial_connected && (level <= _log_level)) {
        va_list argList;
        va_start(argList, format);
        vlog(level, format, argList);
        va_end(argList);
    }
}

static int vlog(int level, const char *format, va_list vlist)
{
    (void)level;

    char buffer[BUFFER_SIZE + 3];     // + "\r\n\0"
    const int max_len = BUFFER_SIZE;  // max chars excluding CRLF
    int n = vsnprintf(buffer, (size_t)max_len + 1, format, vlist); // +1 for '\0'

    if (n < 0) {
        #define FORMATTING_ERROR "vlog(): formatting error"
        #define ERROR_LEN (sizeof(FORMATTING_ERROR)-1)
        UART_nPrint(FORMATTING_ERROR, ERROR_LEN);
        return ERROR_LEN;
    }

    size_t len = (n > max_len) ? max_len : n;

    // Strip existing line endings so we always end with exactly "\r\n"
    while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
        len--;
    }

    buffer[len++] = '\r';
    buffer[len++] = '\n';
    buffer[len]   = '\0';

    UART_nPrint(buffer, len);  // len includes CRLF now
    return (int)len;
}

void UART_nPrint(const char *str, size_t len)
{
    HAL_UART_Transmit(&huart1, (uint8_t*) str, len, UART_TIMEOUT);
}

void UART_Print(const char *str)
{
	UART_nPrint(str, strlen(str));
}
