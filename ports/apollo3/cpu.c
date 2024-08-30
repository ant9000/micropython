#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"

void *phUART;
uint8_t g_pui8TxBuffer[256];
uint8_t g_pui8RxBuffer[2];

const am_hal_uart_config_t g_sUartConfig = {
    //
    // Standard UART settings: 115200-8-N-1
    //
    .ui32BaudRate = 115200,
    .ui32DataBits = AM_HAL_UART_DATA_BITS_8,
    .ui32Parity = AM_HAL_UART_PARITY_NONE,
    .ui32StopBits = AM_HAL_UART_ONE_STOP_BIT,
    .ui32FlowControl = AM_HAL_UART_FLOW_CTRL_NONE,
    //
    // Set TX and RX FIFOs to interrupt at half-full.
    //
    .ui32FifoLevels = (AM_HAL_UART_TX_FIFO_1_2 |
                       AM_HAL_UART_RX_FIFO_1_2),
    //
    // Buffers
    //
    .pui8TxBuffer = g_pui8TxBuffer,
    .ui32TxBufferSize = sizeof(g_pui8TxBuffer),
    .pui8RxBuffer = g_pui8RxBuffer,
    .ui32RxBufferSize = sizeof(g_pui8RxBuffer),
};

void am_uart_isr(void) {
    //
    // Service the FIFOs as necessary, and clear the interrupts.
    //
    uint32_t ui32Status, ui32Idle;
    am_hal_uart_interrupt_status_get(phUART, &ui32Status, true);
    am_hal_uart_interrupt_clear(phUART, ui32Status);
    am_hal_uart_interrupt_service(phUART, ui32Status, &ui32Idle);
}

void uart_print(char *pcStr) {
    uint32_t ui32StrLen = 0;
    uint32_t ui32BytesWritten = 0;
    //
    // Measure the length of the string.
    //
    while (pcStr[ui32StrLen] != 0)
    {
        ui32StrLen++;
    }
    //
    // Print the string via the UART.
    //
    const am_hal_uart_transfer_t sUartWrite =
    {
        .ui32Direction = AM_HAL_UART_WRITE,
        .pui8Data = (uint8_t *) pcStr,
        .ui32NumBytes = ui32StrLen,
        .ui32TimeoutMs = 0,
        .pui32BytesTransferred = &ui32BytesWritten,
    };
    am_hal_uart_transfer(phUART, &sUartWrite);
    if (ui32BytesWritten != ui32StrLen)
    {
        //
        // Couldn't send the whole string!!
        //
        while(1);
    }
}

int uart_getch(void) {
    // TODO: am_hal_queue_item_get
    return 0;
}

void cpu_init(void) {
    //
    // Set the clock frequency.
    //
    am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0);

    //
    // Set the default cache configuration
    //
    am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
    am_hal_cachectrl_enable();

    //
    // Configure the board for low power operation.
    //
    am_bsp_low_power_init();

    //
    // Initialize the printf interface for UART output.
    //
    am_hal_uart_initialize(0, &phUART);
    am_hal_uart_power_control(phUART, AM_HAL_SYSCTRL_WAKE, false);
    am_hal_uart_clock_speed_e eUartClockSpeed = eUART_CLK_SPEED_DEFAULT ;
    am_hal_uart_control(phUART, AM_HAL_UART_CONTROL_CLKSEL, &eUartClockSpeed);
    am_hal_uart_configure(phUART, &g_sUartConfig);

    //
    // Enable the UART pins.
    //
    am_hal_gpio_pinconfig(AM_BSP_GPIO_COM_UART_TX, g_AM_BSP_GPIO_COM_UART_TX);
    am_hal_gpio_pinconfig(AM_BSP_GPIO_COM_UART_RX, g_AM_BSP_GPIO_COM_UART_RX);

    //
    // Enable interrupts.
    //
    NVIC_EnableIRQ((IRQn_Type)(UART0_IRQn));
    am_hal_interrupt_master_enable();

    //
    // Set the main print interface to use the UART print function we defined.
    //
    am_util_stdio_printf_init(uart_print);
}
