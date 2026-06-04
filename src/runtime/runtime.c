#include <kstl/types.hpp>
#include <runtime/runtime.h>

#define __nullptr (0)

static void default_main_unsuccessful() {
    __builtin_trap();
}

static std_args_t *g_std_args;

void get_args(struct std_args *out) {
    if (out->main_unsucessful == __nullptr) {
        out->main_unsucessful = default_main_unsuccessful;
    }

    g_std_args = out;
}

typedef void(*atexit_callback_t)(void);

static void null_cb(void) {}

static atexit_callback_t atexit_callbacks[33];
static size_t atexit_callbacks_ptr = 0;

void trigger_exit_callbacks(void) {
    for (size_t i = atexit_callbacks_ptr + 1; i-- > 0;) {
        atexit_callbacks[i]();
    }
}

int atexit(void (*cb)(void)) {
    if (atexit_callbacks_ptr >= 33) {
        return -1;
    }

    atexit_callbacks[atexit_callbacks_ptr++] = cb;
    return 0;
}

extern char __kcrt_bss_start[];
extern char __kcrt_bss_end[];

static void zero_bss(void) {
    __builtin_memset(__kcrt_bss_start, 0, __kcrt_bss_end - __kcrt_bss_start);
}

extern void (*__init_array_start[])();
extern void (*__init_array_end[])();

static void call_cpp_constructors(void) {
    for (void (**p)(void) = __init_array_start; p < __init_array_end; ++p) {
        (*p)();
    }
}

void kcrt_entrypoint(void) {
    zero_bss();
    call_cpp_constructors();
    atexit_callbacks[0] = null_cb;
    g_std_args = __nullptr;
    int exit_code = KCRT_MAIN_FUNCTION ();
    trigger_exit_callbacks();
    if (exit_code != 0)
        g_std_args->main_unsucessful();
    else
        while (1) {}
}
