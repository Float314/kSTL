#pragma once

#include "types.hpp"

namespace kstd {
    template<typename T>
    constexpr T&& move(T &value) {
        return static_cast<T&&>(value);
    }

    template<typename T>
    struct remove_reference {
        using type = T;
    };

    template<typename T>
    struct remove_reference<T&> {
        using type = T;
    };

    template<typename T>
    struct remove_reference<T&&> {
        using type = T;
    };

    template<typename T>
    using remove_reference_t = typename remove_reference<T>::type;

    template<typename T>
    constexpr T&& forward(remove_reference_t<T> &arg) noexcept {
        return static_cast<T&&>(arg);
    }

    template<typename T>
    constexpr void swap(T &x, T &y) noexcept {
        T temp = kstd::move(x);
        x = kstd::move(y);
        y = kstd::move(temp);
    }

    template<typename T, typename U>
    struct is_same {
        static constexpr bool value = false;
    };

    template<typename T>
    struct is_same<T, T> {
        static constexpr bool value = true;
    };

    template<typename T, typename U>
    inline constexpr bool is_same_v = is_same<T, U>::value;

    template<typename T>
    struct is_array {
        static constexpr bool value = false;
    };

    template<typename T>
    struct is_array<T[]> {
        static constexpr bool value = true;
    };

    
    template<typename T, size_t N>
    struct is_array<T[N]> {
        static constexpr bool value = true;
    };

    template<typename T>
    inline constexpr bool is_array_v = is_array<T>::value;
}
