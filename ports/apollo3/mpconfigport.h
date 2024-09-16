#include <stdint.h>

// options to control how MicroPython is built

#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_FULL_FEATURES)

// You can disable the built-in MicroPython compiler by setting the following
// config option to 0.  If you do this then you won't get a REPL prompt, but you
// will still be able to execute pre-compiled scripts, compiled with mpy-cross.
#define MICROPY_ENABLE_COMPILER     (1)

#define MICROPY_ENABLE_GC                 (1)
#define MICROPY_HELPER_REPL               (1)
#define MICROPY_ENABLE_EXTERNAL_IMPORT    (1)
#define MICROPY_PY_BUILTINS_HELP          (1)
#define MICROPY_PY_BUILTINS_HELP_TEXT apollo3_help_text
#define MICROPY_PY_BUILTINS_HELP_MODULES  (1)

#define MICROPY_PY_TIME                   (1)
#define MICROPY_PY_TIME_TIME_TIME_NS      (1)
#define MICROPY_PY_TIME_GMTIME_LOCALTIME_MKTIME (1)
#define MICROPY_PY_TIME_INCLUDEFILE       "ports/apollo3/modules/time/modtime.c"
#define MICROPY_PY_MACHINE                (1)
#define MICROPY_PY_MACHINE_INCLUDEFILE    "ports/apollo3/modules/machine/modmachine.c"

#define MICROPY_ALLOC_PATH_MAX            (256)

// Use the minimum headroom in the chunk allocator for parse nodes.
#define MICROPY_ALLOC_PARSE_CHUNK_INIT    (16)

// type definitions for the specific machine

typedef intptr_t mp_int_t; // must be pointer size
typedef uintptr_t mp_uint_t; // must be pointer size
typedef long mp_off_t;

#define MP_SSIZE_MAX (0x7fffffff)

#define MICROPY_LONGINT_IMPL        (MICROPY_LONGINT_IMPL_MPZ)
#define MICROPY_FLOAT_IMPL          (MICROPY_FLOAT_IMPL_FLOAT)
#define MP_NEED_LOG2                (1)

// extra builtin names to add to the global namespace
#define MICROPY_PORT_BUILTINS \
    { MP_ROM_QSTR(MP_QSTR_open), MP_ROM_PTR(&mp_builtin_open_obj) },

// We need to provide a declaration/definition of alloca()
#include <alloca.h>

#define MICROPY_HW_BOARD_NAME "apollo3_evb"
#define MICROPY_HW_MCU_NAME "apollo3"

#define MICROPY_MIN_USE_CORTEX_CPU (1)
#define MICROPY_STACK_SIZE     (32 * 1024) // stack size 32 kilobytes
#define MICROPY_HEAP_SIZE      (16 * 1024) // heap size 16 kilobytes

#define MP_STATE_PORT MP_STATE_VM
