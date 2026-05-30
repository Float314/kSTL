#include <runtime/runtime.h>

#define __nullptr (0)

void default_main_unsuccessful() {
    __builtin_trap();
}

static std_args_t *g_std_args;

void get_args(struct std_args *out) {
    if (out->main_unsucessful == __nullptr) {
        out->main_unsucessful = default_main_unsuccessful;
    }

    g_std_args = out;
}

void kcrt_entrypoint() {
    g_std_args = __nullptr;
    int exit_code = main();
    if (exit_code != 0)
        g_std_args->main_unsucessful();
    else
        while (1) {}
}
