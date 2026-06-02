#pragma once

#include "kstl/types.hpp"

namespace kstd {
    /// @brief Deprecated; Use kstd::memcpy from <kstl/string.hpp> instead
    void memcpy(void *__restrict dst, const void *__restrict src, size_t bytes);
}
