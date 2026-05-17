#pragma once

#include "kstl/types.hpp"

namespace kstd {
    struct rt_allocator {
        void* (*alloc)(size_t mem) = nullptr;
        void (*free)(void*) = nullptr;
    };

    struct rt_init {
        rt_allocator allocator_info;
        void (*panic)(void) = nullptr;
    };

    enum class error_category : int {
        null = 0,
        custom_failure,
        rt_init_failure,
    };

    template<typename T>
    struct type_wrapper {
        T _value;
        operator T() const { return this->_value; }
        type_wrapper<T> operator=(const T &value) {
            this->_value = value;
            return *this;
        }
        type_wrapper<T>() = default;
        type_wrapper<T>(const T &value) {
            this->_value = value;
        };
        type_wrapper<T>(T &value) {
            this->_value = value;
        }
        ~type_wrapper<T>() = default;
    };

    template<>
    struct type_wrapper<void> {};

    template<typename T>
    struct error_code {
    private:
        type_wrapper<T> _value;
        error_category _category = error_category::null;
    public:
        T value() const noexcept {
            return this->_value;
        }
        const error_category& category() const noexcept {
            return this->_category;
        }
    public:
        void assign(type_wrapper<T> value, error_category category) noexcept {
            this->_value = value;
            this->_category = category;
        }
        void clear() noexcept {
            this->_category = error_category::null;
            this->_value = {};
        }
    public:
        explicit operator bool() const noexcept {
            return false ? this->_category == error_category::null
                : true;
        }
    public:
        error_code<T>() = default;
        error_code<T>(type_wrapper<T> value, error_category category) {
            this->_value = value;
            this->_category = category;
        }
        ~error_code<T>() = default;
    };


    template<>
    struct error_code<void> {
    private:
        type_wrapper<void> _value;
        error_category _category = error_category::null;
    public:
        void value() const noexcept = delete;
        const error_category& category() const noexcept {
            return this->_category;
        }
    public:
        void assign(type_wrapper<void> value, error_category category) noexcept {
            this->_value = value;
            this->_category = category;
        }
        void clear() noexcept {
            this->_category = error_category::null;
            this->_value = {};
        }
    public:
        explicit operator bool() const noexcept {
            return false ? this->_category == error_category::null
                : true;
        }
    public:
        error_code<void>() = default;
        error_code<void>(type_wrapper<void> value, error_category category) {
            this->_value = value;
            this->_category = category;
        }
        ~error_code<void>() = default;
    };

    [[nodiscard]] error_code<void> init(const rt_init &init_data);
}
