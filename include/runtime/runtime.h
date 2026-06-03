#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct std_args {
    void (*main_unsucessful)();
    void *data;
} std_args_t;

void get_args(struct std_args *out);

/// @brief Trigger 'exit' and run atexit() callbacks
/// @note ONLY call if not using KCRT_MAIN_FUNCTION
void trigger_exit_callbacks(void);

/// @brief Register atexit function with a maximum of 33 functions
/// @note Called in reverse order at invocation of trigger_exit_callbacks()
int atexit(void (*cb)(void));

int KCRT_MAIN_FUNCTION (void);

#ifdef __cplusplus
}
#endif
