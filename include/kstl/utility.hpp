#pragma once

namespace kstd {
    template<typename T>
    constexpr T&& move(T &value) {
        return static_cast<T&&>(value);
    }
}
