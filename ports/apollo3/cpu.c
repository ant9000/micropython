#include "am_mcu_apollo.h"
#include "am_bsp.h"

void am_uart_isr(void) {
    am_bsp_buffered_uart_service();
}

void cpu_init(void) {
    // Set the clock frequency.
    am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0);
    // Set the default cache configuration
    am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
    am_hal_cachectrl_enable();
    // Configure the board for low power operation.
    am_bsp_low_power_init();
    // Initialize the printf interface for UART output.
    am_bsp_buffered_uart_printf_enable();
}
