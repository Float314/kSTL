#include "kstl/stdlib.hpp"
#include "kstl/runtime.hpp"

namespace kstl_globals {
    extern kstd::rt_init g_init_data;
}

namespace kstd {
    void memcpy(void *__restrict dst, const void *__restrict src, size_t bytes) {
        if (bytes == 0) return;
        if (dst == nullptr || src == nullptr) {
            kstl_globals::g_init_data.panic();
        }

        for (size_t i = 0; i < bytes; ++i) {
            *(reinterpret_cast<char*>(dst) + i) = *(reinterpret_cast<const char*>(src) + i);
        }
    }
}
