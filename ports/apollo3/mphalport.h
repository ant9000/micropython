extern mp_uint_t systick_millis;
static inline mp_uint_t mp_hal_ticks_ms(void) {
    return systick_millis;
}
static inline void mp_hal_set_interrupt_char(char c) {
}
