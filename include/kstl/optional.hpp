#pragma once

#include <kstl/runtime.hpp>
#include <kstl/stdlib.hpp>
#include <kstl/utility.hpp>

namespace kstl_globals {
    extern kstd::rt_init g_init_data;
    extern void* (*malloc)(size_t);
    extern void (*free)(void*);
}

namespace kstd {
    template<typename T>
    class optional {
    private:
        alignas(T) uint8_t storage[sizeof(T)] = {};
        bool _has_value = false;
    private:
        const T* ptr() const noexcept {
            if (!this->_has_value) {
                kstl_globals::g_init_data.panic();
            }

            return reinterpret_cast<const T*>(this->storage);
        }

        T* ptr() noexcept {
            if (!this->_has_value) {
                kstl_globals::g_init_data.panic();
            }

            return reinterpret_cast<T*>(this->storage);
        }
    public:
        bool has_value() const noexcept {
            return this->_has_value;
        }

        T value() const noexcept {
            if (this->_has_value) {
                return *this->ptr();
            }

            kstl_globals::g_init_data.panic();
        }

        T value_or(const T &default_value) const noexcept {
            if (this->_has_value) {
                return *this->ptr();
            } else {
                return default_value;
            }
        }

        T value_or(T &&default_value) const noexcept {
            if (this->_has_value) {
                return *this->ptr();
            } else {
                return kstd::move(default_value);
            }
        }
    public:
        explicit operator bool() const noexcept {
            return this->_has_value;
        }

        optional& operator=(const optional &other) noexcept {
            if (this == &other) return;

            if (this->_has_value) {
                this->ptr()->~T();
            }

            this->_has_value = false;

            if (other._has_value) {
                new (storage) T(*other.ptr());
                this->_has_value = true;
            }

            return *this;
        }

        optional& operator=(optional &&other) noexcept {
            if (this == other) return;

            if (this->_has_value) {
                this->ptr()->~T();
            }

            this->_has_value = false;

            if (other._has_value) {
                new (storage) T(kstd::move(*other.ptr()));
                this->_has_value = true;
            }

            return *this;
        }

        T& operator*() noexcept {
            if (!this->_has_value) {
                kstl_globals::g_init_data.panic();
            }

            return *this->ptr();
        }

        const T& operator*() const noexcept {
            if (!_has_value) {
                kstl_globals::g_init_data.panic();
            }

            return *this->ptr();
        }

        T& operator->() noexcept {
            if (!_has_value) {
                kstl_globals::g_init_data.panic();
            }

            return this->ptr();
        }

        const T& operator->() const noexcept {
            if (!_has_value) {
                kstl_globals::g_init_data.panic();
            }

            return this->ptr();
        }
    public:
        optional() {}

        template<typename... Args>
        optional(Args... args) {
            new (this->storage) T(kstd::forward<Args>(args)...);
            this->_has_value = true;
        }

        optional(const T &value) {
            new (this->storage) T(value);
            this->_has_value = true;
        }

        optional(const optional &other) {
            if (other._has_value) {
                new (storage) T(*other.ptr());
                this->_has_value = true;
            }
        }

        optional(optional &&other) {
            if (other._has_value) {
                new (this->storage) T(kstd::move(*other.ptr()));
                this->_has_value = true;
                other._has_value = false;
            }
        }
    public:
        ~optional() {
            if (this->_has_value) {
                this->ptr()->~T();
            }
        }
    };
}
