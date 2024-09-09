#include "py/obj.h"
#include "shared/timeutils/timeutils.h"
#include "am_mcu_apollo.h"

// Return the localtime as an 8-tuple.
static mp_obj_t mp_time_localtime_get(void) {
    am_hal_rtc_time_t time;
    am_hal_rtc_time_get(&time);
    mp_obj_t tuple[8] = {
        mp_obj_new_int(2000 + time.ui32Year),
        mp_obj_new_int(time.ui32Month),
        mp_obj_new_int(time.ui32DayOfMonth),
        mp_obj_new_int(time.ui32Hour),
        mp_obj_new_int(time.ui32Minute),
        mp_obj_new_int(time.ui32Second),
        mp_obj_new_int(time.ui32Weekday),
        mp_obj_new_int(timeutils_year_day(2000 + time.ui32Year, time.ui32Month, time.ui32DayOfMonth)),
    };
    return mp_obj_new_tuple(8, tuple);
}

// Returns the number of seconds, as an integer, since 1/1/2000.
static mp_obj_t mp_time_time_get(void) {
    am_hal_rtc_time_t time;
    am_hal_rtc_time_get(&time);
    return mp_obj_new_int(timeutils_seconds_since_epoch(2000 + time.ui32Year, time.ui32Month, time.ui32DayOfMonth, time.ui32Hour, time.ui32Minute, time.ui32Second));
}
