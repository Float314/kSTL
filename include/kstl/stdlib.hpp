#pragma once

#include "kstl/types.hpp"

namespace kstd {
    /// @brief Register atexit function with a maximum of 33 functions
    /// @note Called in reverse order at invocation of trigger_exit_callbacks()
    /// @note Requires kstl_crt
    extern "C" int atexit(void(*cb)(void));
}
