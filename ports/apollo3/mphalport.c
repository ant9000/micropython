#include "py/mpconfig.h"
#include "py/mphal.h"
#include "py/runtime.h"
#include "shared/timeutils/timeutils.h"
#include "mphalport.h"
#include "am_bsp.h"
#include "am_util_delay.h"
#include "irq.h"

extern volatile uint32_t systick_ms;
mp_uint_t mp_hal_ticks_ms(void) {
    return systick_ms;
}

mp_uint_t mp_hal_ticks_us(void) {
    mp_uint_t irq_state = disable_irq();
    uint32_t counter = SysTick->VAL;
    uint32_t milliseconds = mp_hal_ticks_ms();
    uint32_t status = SysTick->CTRL;
    enable_irq(irq_state);

    // It's still possible for the countflag bit to get set if the counter was
    // reloaded between reading VAL and reading CTRL. With interrupts  disabled
    // it definitely takes less than 50 HCLK cycles between reading VAL and
    // reading CTRL, so the test (counter > 50) is to cover the case where VAL
    // is +ve and very close to zero, and the COUNTFLAG bit is also set.
    if ((status & SysTick_CTRL_COUNTFLAG_Msk) && counter > 50) {
        // This means that the HW reloaded VAL between the time we read VAL and the
        // time we read CTRL, which implies that there is an interrupt pending
        // to increment the tick counter.
        milliseconds++;
    }
    uint32_t load = SysTick->LOAD;
    counter = load - counter; // Convert from decrementing to incrementing

    // ((load + 1) / 1000) is the number of counts per microsecond.
    //
    // counter / ((load + 1) / 1000) scales from the systick clock to microseconds
    // and is the same thing as (counter * 1000) / (load + 1)
    return milliseconds * 1000 + (counter * 1000) / (load + 1);
}

mp_uint_t mp_hal_ticks_cpu(void) {
    return am_hal_systick_count();
}

void mp_hal_delay_us(mp_uint_t us) {
    am_hal_systick_delay_us(us);
}

void mp_hal_delay_ms(mp_uint_t ms) {
    uint32_t t0 = systick_ms;
    while (systick_ms - t0 < ms) {
        mp_event_handle_nowait();
    }
}

uint64_t mp_hal_time_ns(void) {
    uint64_t ns;
    am_hal_rtc_time_t time;
    am_hal_rtc_time_get(&time);
    ns = timeutils_seconds_since_epoch(2000 + time.ui32Year, time.ui32Month, time.ui32DayOfMonth, time.ui32Hour, time.ui32Minute, time.ui32Second);
    ns *= 1000000000ULL;
    return ns;
}

void mp_hal_set_interrupt_char(int c) {
}

uintptr_t mp_hal_stdio_poll(uintptr_t poll_flags) {
    uintptr_t ret = 0;
    #if MICROPY_PY_OS_DUPTERM
    ret |= mp_os_dupterm_poll(poll_flags);
    #endif
    return ret;
}


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
