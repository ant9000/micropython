#include "py/runtime.h"
#include "am_bsp.h"
#include "switch.h"

typedef struct _machine_switch_obj_t {
    mp_obj_base_t base;
} machine_switch_obj_t;

static const machine_switch_obj_t machine_switch_obj[] = {
    {{&machine_switch_type}},
    {{&machine_switch_type}},
    {{&machine_switch_type}},
};

#define NUM_SWITCH MP_ARRAY_SIZE(machine_switch_obj)
#define SWITCH_ID(obj) ((obj) - &machine_switch_obj[0] + 1)

void machine_switch_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    machine_switch_obj_t *self = self_in;
    mp_printf(print, "Switch(%u) %s", SWITCH_ID(self));
}

static mp_obj_t machine_switch_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 1, 1, false);
    mp_int_t sw_id = mp_obj_get_int(args[0]);
    if (!(1 <= sw_id && sw_id <= NUM_SWITCH)) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("Switch %d does not exist"), sw_id);
    }
    return (mp_obj_t)&machine_switch_obj[sw_id - 1];
}

mp_obj_t machine_switch_value(mp_obj_t self_in) {
    machine_switch_obj_t *self = self_in;
    return am_devices_button_is_down(am_bsp_psButtons[SWITCH_ID(self) - 1]) ? mp_const_true : mp_const_false;
}
static MP_DEFINE_CONST_FUN_OBJ_1(machine_switch_value_obj, machine_switch_value);

mp_obj_t machine_switch_call(mp_obj_t self_in, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 0, 0, false);
    return machine_switch_value(self_in);
}

static const mp_rom_map_elem_t machine_switch_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_value), MP_ROM_PTR(&machine_switch_value_obj) },
};

static MP_DEFINE_CONST_DICT(machine_switch_locals_dict, machine_switch_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    machine_switch_type,
    MP_QSTR_Switch,
    MP_TYPE_FLAG_NONE,
    make_new, machine_switch_make_new,
    print, machine_switch_print,
    call, machine_switch_call,
    locals_dict, &machine_switch_locals_dict
    );
