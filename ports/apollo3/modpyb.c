#include <stdio.h>

#include "py/obj.h"
#include "py/mphal.h"
#include "modpyb.h"
#include "am_mcu_apollo.h"

static mp_obj_t pyb_millis(void) {
    return MP_OBJ_NEW_SMALL_INT(mp_hal_ticks_ms());
}
static MP_DEFINE_CONST_FUN_OBJ_0(pyb_millis_obj, pyb_millis);

static mp_obj_t pyb_info(void) {
    uint32_t status;
    am_hal_mcuctrl_device_t device;
    status = am_hal_mcuctrl_info_get(AM_HAL_MCUCTRL_INFO_DEVICEID, &device);
    if (status == AM_HAL_STATUS_SUCCESS) {
        printf("Device part number    : %08lx\n", device.ui32ChipPN);
        printf("Chip ID 0             : %08lx\n", device.ui32ChipID0);
        printf("Chip ID 1             : %08lx\n", device.ui32ChipID1);
        printf("Revision              : %08lx\n", device.ui32ChipRev);
        printf("Vendor ID             : %08lx\n", device.ui32VendorID);
        printf("SKU                   : %08lx\n", device.ui32SKU);
        printf("Qualified             : %08lx\n", device.ui32Qualified);
        printf("Flash size            : %lu kB\n", device.ui32FlashSize / 1024);
        printf("SRAM size             : %lu kB\n", device.ui32SRAMSize / 1024);
        printf("JEDEC part number     : %08lx\n", device.ui32JedecPN);
        printf("JEDEC manufacturer ID : %08lx\n", device.ui32JedecJEPID);
        printf("JEDEC chip revision   : %08lx\n", device.ui32JedecCHIPREV);
        printf("JEDEC CoreSight ID    : %08lx\n", device.ui32JedecCID);
    } else {
        printf("ERROR: cannot read device ID\n");
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(pyb_info_obj, pyb_info);

static const mp_rom_map_elem_t pyb_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_pyb) },
    { MP_ROM_QSTR(MP_QSTR_millis), MP_ROM_PTR(&pyb_millis_obj) },
    { MP_ROM_QSTR(MP_QSTR_info), MP_ROM_PTR(&pyb_info_obj) },
    { MP_ROM_QSTR(MP_QSTR_LED), MP_ROM_PTR(&pyb_led_type) },
    { MP_ROM_QSTR(MP_QSTR_Switch), MP_ROM_PTR(&pyb_switch_type) },
};

static MP_DEFINE_CONST_DICT(pyb_module_globals, pyb_module_globals_table);

const mp_obj_module_t pyb_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&pyb_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_pyb, pyb_module);
