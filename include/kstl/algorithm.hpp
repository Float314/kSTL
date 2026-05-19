#pragma once

namespace kstd {
    template<typename T>
    constexpr const T& max(const T &a, const T &b) noexcept {
        return (a > b) ? a : b;
    }

    template<typename T>
    constexpr const T& min(const T &a, const T &b) noexcept {
        return (a < b) ? a : b;
    }

    template<typename T, typename Compare>
    constexpr const T& max(const T &a, const T &b, Compare cmp) noexcept {
        return cmp(a, b) ? b : a;
    }

    template<typename T, typename Compare>
    constexpr const T& min(const T &a, const T &b, Compare cmp) noexcept {
        return cmp(a, b) ? a : b;
    }
}
