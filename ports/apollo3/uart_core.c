#include "py/mpconfig.h"
#include "am_bsp.h"

extern void uart_print(char *pcStr);
extern int uart_getch();

/*
 * Core UART functions to implement for a port
 */

// Receive single character
int mp_hal_stdin_rx_chr(void) {
    return uart_getch();
}

// Send string of given length
mp_uint_t mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    uart_print((char *)str);
    return 0;
}
