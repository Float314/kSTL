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
    template<typename T, size_t Size>
    class array {
    public:
        using iterator = T *;
        using const_iterator = const T *;
    public:
        T _data[Size];
    public:
        void __bounds_check(size_t index) const noexcept {
            if (index >= Size || index < 0) {
                kstl_globals::g_init_data.panic();
            }
        }
    public:
        size_t size() const noexcept {
            return Size;
        }

        T& at(size_t index) noexcept {
            __bounds_check(index);
            return this->_data[index];
        }

        const T& at(size_t index) const noexcept {
            __bounds_check(index);
            return this->_data[index];
        }
    public:
        T& operator[](size_t index) noexcept {
            return this->_data[index];
        }

        const T& operator[](size_t index) const noexcept {
            return this->_data[index];
        }

        array& operator=(const array &other) noexcept {
            if (&other == this) return *this;
            for (size_t i = 0; i < other.size(); ++i) {
                this->_data[i] = other._data[i];
            }
        }

        array& operator=(array &&v) noexcept {
            for (size_t i = 0; i < v.size(); ++i) {
                this->_data[i] = kstd::move(v._data[i]);
            }
        }

        bool operator==(const array &other) const noexcept {
            return !(this != other);
        }

        bool operator!=(const array &other) const noexcept {
            for (size_t i = 0; i < other.size(); ++i) {
                if (other[i] == this->_data[i]) {
                    return false;
                }
            }
            return true;
        }
    public:
        T* data() const noexcept {
            return this->_data;
        }

        T& front() noexcept {
            __bounds_check(0);
            return this->_data[0];
        }

        const T& front() const noexcept {
            __bounds_check(0);
            return this->_data[0];
        }

        T& back() noexcept {
            __bounds_check(Size - 1);
            return this->_data[Size - 1];
        }

        const T& back() const noexcept {
            __bounds_check(Size - 1);
            return this->_data[Size - 1];
        }
    public:
        iterator begin() noexcept {
            return this->_data;
        }

        iterator end() noexcept {
            return this->_data + Size;
        }

        const_iterator begin() const noexcept {
            return this->_data;
        }

        const_iterator end() const noexcept {
            return this->_data + Size;
        }
    public:
        ~array() = default;
    };
}
