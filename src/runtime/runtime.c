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

extern char __kcrt_bss_start[];
extern char __kcrt_bss_end[];

static void zero_bss() {
    __builtin_memset(__kcrt_bss_start, 0, __kcrt_bss_end - __kcrt_bss_start);
}

extern void (*__init_array_start[])();
extern void (*__init_array_end[])();

static void call_cpp_constructors() {
    for (void (**p)(void) = __init_array_start; p < __init_array_end; ++p) {
        (*p)();
    }
}

void kcrt_entrypoint() {
    zero_bss();
    call_cpp_constructors();
    g_std_args = __nullptr;
    int exit_code = KCRT_MAIN_FUNCTION ();
    if (exit_code != 0)
        g_std_args->main_unsucessful();
    else
        while (1) {}
}
