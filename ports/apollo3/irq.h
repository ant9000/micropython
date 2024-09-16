#ifndef MICROPY_INCLUDED_APOLLO3_IRQ_H
#define MICROPY_INCLUDED_APOLLO3_IRQ_H

static inline uint32_t query_irq(void) {
    return __get_PRIMASK();
}

static inline void enable_irq(mp_uint_t state) {
    __set_PRIMASK(state);
}

static inline mp_uint_t disable_irq(void) {
    mp_uint_t state = __get_PRIMASK();
    __disable_irq();
    return state;
}

#endif // MICROPY_INCLUDED_APOLLO3_IRQ_H
