#include "kstl/stdlib.hpp"
#include "kstl/runtime.hpp"

namespace kstl_globals {
    extern kstd::rt_init g_init_data;
}

namespace kstd {
    void memcpy(void *__restrict dst, const void *__restrict src, size_t bytes) {
        __builtin_memcpy(dst, src, bytes); // ok
    }
}
