#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct std_args {
    void (*main_unsucessful)();
    void *data;
} std_args_t;

void get_args(struct std_args *out);

#ifdef __cplusplus
}
#endif

int main(void);
