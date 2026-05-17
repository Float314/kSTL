#include "kstl/stdlib.hpp"

namespace kstd {
    void memcpy(void *__restrict dst, const void *__restrict src, size_t bytes) {
        for (size_t i = 0; i < bytes; ++i) {
            *(reinterpret_cast<char*>(dst) + i) = *(reinterpret_cast<const char*>(src) + i);
        }
    }
}
