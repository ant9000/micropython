#include "py/mpconfig.h"
#include "am_bsp.h"
#include "am_util_delay.h"

// Receive single character
int mp_hal_stdin_rx_chr(void) {
    uint8_t ch;
    const am_hal_uart_transfer_t sUartRead = {
        .ui32Direction = AM_HAL_UART_READ,
        .pui8Data = &ch,
        .ui32NumBytes = 1,
        .ui32TimeoutMs = AM_HAL_UART_WAIT_FOREVER,
        .pui32BytesTransferred = 0,
    };
    am_bsp_com_uart_transfer(&sUartRead);
    return (int)ch;
}

// Send string of given length
mp_uint_t mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    uint32_t ui32BytesWritten = 0;
    const am_hal_uart_transfer_t sUartWrite = {
        .ui32Direction = AM_HAL_UART_WRITE,
        .pui8Data = (uint8_t *)str,
        .ui32NumBytes = (uint32_t)len,
        .ui32TimeoutMs = 0,
        .pui32BytesTransferred = &ui32BytesWritten,
    };
    am_bsp_com_uart_transfer(&sUartWrite);
    am_util_delay_ms(1);
    return ui32BytesWritten;
}
