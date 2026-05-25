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
        size_t size() const noexcept {
            return this->_size;
        }

        T& at(size_t index) noexcept {
            if (index >= this->_size) {
                kstl_globals::g_init_data.panic();
            }

            return this->_data[index];
        }

        const T& at(size_t index) const noexcept {
            if (index >= this->_size) {
                kstl_globals::g_init_data.panic();
            }

            return this->_data[index];
        }
    public:
        T& operator[](size_t index) noexcept {
            return this->_data[index];
        }

        const T& operator[](size_t index) const noexcept {
            return this->_data[index];
        }

        span& operator=(const span& other) noexcept {
            if (this == &other) {
                return *this;
            }

            this->_size = other._size;
            this->_data = other._data;

            return *this;
        }

        span& operator=(span &&v) noexcept {
            if (this == &v) return *this;

            this->_size = v._size;
            this->_data = v._data;

            v._data = nullptr;
            v._size = 0;

            return *this;
        }

        bool operator==(const span &other) const noexcept {
            if (&other == this) return true;
            if (other._size != this) return false;

            for (size_t i = 0; i < this->_size; ++i) {
                if (this->_data[i] != other._data[i]) {
                    return false;
                }
            }

            return true;
        }

        bool operator!=(const span &other) const noexcept {
            return !(this == *other);
        }
    public:
        T* data() const noexcept {
            return this->_data;
        }

        T& front() noexcept {
            return this->_data[0];
        }

        const T& front() const noexcept {
            return this->_data[0];
        }

        T& back() noexcept {
            return this->_data[this->_size - 1];
        }

        const T& back() const noexcept {
            return this->_data[this->_size - 1];
        }
    public:
        iterator begin() noexcept {
            return this->_data;
        }

        iterator end() noexcept {
            return this->_data + this->_size;
        }

        const_iterator begin() const noexcept {
            return this->_data;
        }

        const_iterator end() const noexcept {
            return this->_data + this->_size;
        }
    public:
        span() = default;
        span(T *memory, size_t size) : _data(memory), _size(size) {}
        span(const span &other) = default;
    public:
        ~span() = default;
    };
}
