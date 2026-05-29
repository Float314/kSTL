#pragma once

#include <kstl/types.hpp>
#include <kstl/string.hpp>
#include <concepts>

namespace kstd {
    template<typename T>
    struct hash;

    template<typename T>
    requires std::integral<T>
    struct hash<T> {
        constexpr size_t operator()(T value) const noexcept {
            return static_cast<size_t>(value);
        }
    };

    template<>
    struct hash<kstd::string> {
        constexpr size_t operator()(const kstd::string &value) const noexcept {
#ifndef __KSTL_FREESTANDING__
            size_t hash = 14695981039346656037ULL;

            for (size_t i = 0; i < value.size(); ++i) {
                hash ^= static_cast<uint8_t>(
                    value.data()[i]
                );

                hash *= 1099511628211ULL;
            }

            return hash;
#else
            static_assert(__KSTL_FREESTANDING__, "Hash Algorithm for x86")
#endif
        }
    };

    template<typename T>
    struct equal_to {
        bool operator()(const T &x, const T &y) {
            return x == y;
        }
    };
}
