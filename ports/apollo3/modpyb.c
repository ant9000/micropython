#include <stdio.h>

#include "py/obj.h"
#include "py/mphal.h"
#include "modpyb.h"

static mp_obj_t pyb_millis(void) {
    return MP_OBJ_NEW_SMALL_INT(mp_hal_ticks_ms());
}
static MP_DEFINE_CONST_FUN_OBJ_0(pyb_millis_obj, pyb_millis);

static const mp_rom_map_elem_t pyb_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_pyb) },
    { MP_ROM_QSTR(MP_QSTR_millis), MP_ROM_PTR(&pyb_millis_obj) },
    { MP_ROM_QSTR(MP_QSTR_LED), MP_ROM_PTR(&pyb_led_type) },
    { MP_ROM_QSTR(MP_QSTR_Switch), MP_ROM_PTR(&pyb_switch_type) },
};

static MP_DEFINE_CONST_DICT(pyb_module_globals, pyb_module_globals_table);

const mp_obj_module_t pyb_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&pyb_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_pyb, pyb_module);
