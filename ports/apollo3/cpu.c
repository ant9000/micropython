#include "mpconfigport.h"
#include "am_mcu_apollo.h"
#include "am_bsp.h"

mp_uint_t systick_millis = 0;

void am_uart_isr(void) {
    am_bsp_buffered_uart_service();
}

void SysTick_Handler(void) {
    systick_millis++;
    am_devices_button_array_tick(am_bsp_psButtons, AM_BSP_NUM_BUTTONS);
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

    // Start Systick.
    am_hal_systick_int_enable();
    am_hal_systick_load(AM_HAL_CLKGEN_FREQ_MAX_HZ / 1000);
    am_hal_systick_start();

    // Enable the RTC.
    am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_XTAL_START, 0);
    am_hal_rtc_osc_select(AM_HAL_RTC_OSC_XT);
    am_hal_rtc_osc_enable();

    // Enable buttons
    am_devices_button_array_init(am_bsp_psButtons, AM_BSP_NUM_BUTTONS);
}
