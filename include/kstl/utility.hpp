#pragma once

#include "types.hpp"
#include <concepts>
#include <compare>

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
    struct remove_const {
        using type = T;
    };

    template<typename T>
    struct remove_const<const T> {
        using type = T;
    };

    template<typename T>
    using remove_const_t = typename remove_const<T>::type;

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

    template<typename T>
    struct type_identity {
        using type = T;
    };

    template<typename T>
    using type_identity_t = typename type_identity<T>::type;

    template<typename Destination>
    constexpr Destination implicit_cast(type_identity_t<Destination> src) {
        return src;
    }

    template<typename Destination, typename Source>
    constexpr Destination pun_cast(const Source &val) {
        return __builtin_bit_cast(Destination, val);
    }

    template<typename T, typename U>
    struct pair {
        T first;
        U second;

        auto operator<=>(const pair &other) const noexcept = default;
        bool operator==(const pair &other) const noexcept = default;
        pair& operator=(const pair &other) noexcept {
            if (this == &other) return *this;

            this->first = other.first;
            this->second = other.second;

            return *this;
        }
        pair& operator=(pair &&other) noexcept
        requires std::move_constructible<T> && 
                 std::move_constructible<U>
        {
            if (this == &other) return *this;

            this->first = kstd::move(other.first);
            this->second = kstd::move(other.second);

            return *this;
        }

        pair() = default;
        pair(const T &first, const T &second) : first(first)
                                              , second(second) 
        {}
        pair(T &&first, T &&second) : first(kstd::move(first))
                                    , second(kstd::move(second)) 
        {}
        ~pair() = default;
    };

    template<typename T, typename U>
    pair<T, U> make_pair(const T &first, const T &second) {
        return pair<T, U> { first, second };
    }

    template<typename T, typename U>
    pair<T, U> make_pair(T &&first, T &&second) {
        return pair<T, U> { first, second };
    }
}
