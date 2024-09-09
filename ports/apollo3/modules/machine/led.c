#include "py/runtime.h"
#include "am_bsp.h"
#include "led.h"

typedef struct _machine_led_obj_t {
    mp_obj_base_t base;
} machine_led_obj_t;

static const machine_led_obj_t machine_led_obj[] = {
    {{&machine_led_type}},
    {{&machine_led_type}},
    {{&machine_led_type}},
    {{&machine_led_type}},
    {{&machine_led_type}},
};

#define NUM_LED MP_ARRAY_SIZE(machine_led_obj)
#define LED_ID(obj) ((obj) - &machine_led_obj[0] + 1)

void machine_led_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    machine_led_obj_t *self = self_in;
    mp_printf(print, "LED(%u)", LED_ID(self));
}

static mp_obj_t machine_led_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 1, 1, false);
    mp_int_t led_id = mp_obj_get_int(args[0]);
    if (!(1 <= led_id && led_id <= NUM_LED)) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("LED %d does not exist"), led_id);
    }
    return (mp_obj_t)&machine_led_obj[led_id - 1];
}

mp_obj_t machine_led_on(mp_obj_t self_in) {
    machine_led_obj_t *self = self_in;
    am_devices_led_on(am_bsp_psLEDs, LED_ID(self) - 1);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(machine_led_on_obj, machine_led_on);

mp_obj_t machine_led_off(mp_obj_t self_in) {
    machine_led_obj_t *self = self_in;
    am_devices_led_off(am_bsp_psLEDs, LED_ID(self) - 1);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(machine_led_off_obj, machine_led_off);

mp_obj_t machine_led_toggle(mp_obj_t self_in) {
    machine_led_obj_t *self = self_in;
    am_devices_led_toggle(am_bsp_psLEDs, LED_ID(self) - 1);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(machine_led_toggle_obj, machine_led_toggle);

static const mp_rom_map_elem_t machine_led_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_on), MP_ROM_PTR(&machine_led_on_obj) },
    { MP_ROM_QSTR(MP_QSTR_off), MP_ROM_PTR(&machine_led_off_obj) },
    { MP_ROM_QSTR(MP_QSTR_toggle), MP_ROM_PTR(&machine_led_toggle_obj) },
};

static MP_DEFINE_CONST_DICT(machine_led_locals_dict, machine_led_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    machine_led_type,
    MP_QSTR_LED,
    MP_TYPE_FLAG_NONE,
    make_new, machine_led_make_new,
    print, machine_led_print,
    locals_dict, &machine_led_locals_dict
    );
