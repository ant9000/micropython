#include <stdio.h>

#include "py/obj.h"
#include "py/mphal.h"
#include "modmachine.h"
#include "led.h"
#include "switch.h"
#include "am_mcu_apollo.h"

#define MICROPY_PY_MACHINE_EXTRA_GLOBALS \
    { MP_ROM_QSTR(MP_QSTR_info),      MP_ROM_PTR(&machine_info_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_millis),    MP_ROM_PTR(&machine_millis_obj) }, \
    { MP_ROM_QSTR(MP_QSTR_LED),       MP_ROM_PTR(&machine_led_type) }, \
    { MP_ROM_QSTR(MP_QSTR_Switch),    MP_ROM_PTR(&machine_switch_type) }, \


void machine_init(void) {
}

// machine.info([dump_alloc_table])
// Print out lots of information about the board.
static mp_obj_t machine_info(mp_uint_t n_args, const mp_obj_t *args) {
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
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_info_obj, 0, 1, machine_info);

static mp_obj_t machine_millis(void) {
    return MP_OBJ_NEW_SMALL_INT(mp_hal_ticks_ms());
}
static MP_DEFINE_CONST_FUN_OBJ_0(machine_millis_obj, machine_millis);

static void mp_machine_idle(void) {
    // Do nothing.
}
