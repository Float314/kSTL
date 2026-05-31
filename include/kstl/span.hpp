#pragma once

#include <concepts>
#include <initializer_list>
#include <new>
#include "kstl/types.hpp"
#include "kstl/stdlib.hpp"
#include "kstl/utility.hpp"
#include "kstl/runtime.hpp"
#include "kstl/algorithm.hpp"

namespace kstl_globals {
    extern kstd::rt_init g_init_data;
}

namespace kstd {
    template<typename T>
    struct span {
    public:
        using iterator = T *;
        using const_iterator = const T *;
    private:
        T *_data = nullptr;
        size_t _size = 0;
    public:
        constexpr size_t size() const noexcept {
            return this->_size;
        }

        constexpr size_t size_bytes() const noexcept {
            return this->_size * sizeof(T);
        }

        constexpr bool empty() const noexcept {
            return this->_size == 0;
        }
    public:
        constexpr T& operator[](size_t index) noexcept {
            return this->_data[index];
        }

        constexpr const T& operator[](size_t index) const noexcept {
            return this->_data[index];
        }

        constexpr span& operator=(const span& other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->_size = other._size;
            this->_data = other._data;

            return *this;
        }

        constexpr bool operator==(const span &other) const noexcept {
            if (&other == this) return true;
            if (other._size != this->_size) return false;

            for (size_t i = 0; i < this->_size; ++i) {
                if (this->_data[i] != other._data[i]) {
                    return false;
                }
            }

            return true;
        }

        constexpr bool operator!=(const span &other) const noexcept {
            return !(*this == other);
        }
    public:
        constexpr T& front() noexcept {
            return this->_data[0];
        }

        constexpr const T& front() const noexcept {
            return this->_data[0];
        }

        constexpr T& back() noexcept {
            return this->_data[this->_size - 1];
        }

        constexpr const T& back() const noexcept {
            return this->_data[this->_size - 1];
        }
    public:
        constexpr iterator begin() noexcept {
            return this->_data;
        }

        constexpr iterator end() noexcept {
            return this->_data + this->_size;
        }

        constexpr const_iterator begin() const noexcept {
            return this->_data;
        }

        constexpr const_iterator end() const noexcept {
            return this->_data + this->_size;
        }
    public:
        constexpr span() = default;
        constexpr span(T *memory, size_t size) : _data(memory), _size(size) {}
        constexpr span(const span &other) = default;
    public:
        // Implicit destructor
    };
}
