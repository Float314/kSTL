#pragma once

#include <concepts>
#include <initializer_list>
#include <new>
#include <type_traits>
#include "kstl/types.hpp"
#include "kstl/string.hpp"
#include "kstl/utility.hpp"
#include "kstl/runtime.hpp"
#include "kstl/algorithm.hpp"

namespace kstl_globals {
    extern kstd::rt_init g_init_data;
    extern void* (*malloc)(size_t);
    extern void (*free)(void*);
}

namespace kstd::detail {
    template<typename T>
    void move_array(T *dst, T *src, size_t len) {
        if constexpr (std::is_trivially_copyable_v<T>) {
            kstd::memcpy(dst, src, len * sizeof(T));
        } else {
            for (size_t i = 0; i < len; ++i) {
                new (&dst[i]) T(kstd::move(src[i]));
                src[i].~T();
            }
        }
    }

    template<typename T>
    void copy_array(T *dst, const T *src, size_t len) {
        if constexpr (std::is_trivially_copyable_v<T>) {
            kstd::memcpy(dst, src, len * sizeof(T));
        } else {
            for (size_t i = 0; i < len; ++i) {
                new (&dst[i]) T(src[i]);
            }
        }
    }

    template<typename T>
    void move(T *dst_ptr, T &&src) {
        if constexpr (std::is_trivially_copyable_v<T>) {
            kstd::memcpy(dst_ptr, &src, sizeof(T));
        } else {
            *dst_ptr = kstd::move(src);
        }
    }

    template<typename T>
    void move_construct(T *dst_unconstructed_ptr, T &&src) {
        if constexpr (std::is_trivially_copyable_v<T>) {
            kstd::memcpy(dst_unconstructed_ptr, &src, sizeof(T));
        } else {
            new (dst_unconstructed_ptr) T(kstd::move(src));
        }
    }

    template<typename T>
    void copy(T *dst_ptr, const T &src) {
        if constexpr (std::is_trivially_copyable_v<T>) {
            kstd::memcpy(dst_ptr, &src, sizeof(T));
        } else {
            *dst_ptr = src;
        }
    }

    template<typename T>
    void copy_construct(T *dst_unconstructed_ptr, const T &src) {
        if constexpr (std::is_trivially_copyable_v<T>) {
            kstd::memcpy(dst_unconstructed_ptr, &src, sizeof(T));
        } else {
            new (dst_unconstructed_ptr) T(src);
        }
    }
}

namespace kstd {
    template<typename T>
    class vector {
    public:
        using iterator = T *;
        using const_iterator = const T *;
    private:
        T *_data = nullptr;
        size_t _size = 0;
        size_t _capacity = 0;
    private:
        void grow() noexcept {
            this->_capacity *= 2;

            T* new_data = reinterpret_cast<T*>(
                kstl_globals::malloc(sizeof(T) * this->_capacity)
            );

            detail::move_array(new_data, this->_data, this->_size);

            kstl_globals::free(this->_data);
            this->_data = new_data;
        }
    public:
        size_t size() const noexcept {
            return this->_size;
        }

        size_t capacity() const noexcept {
            return this->_capacity;
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
        void push_back(const T &elm) noexcept {
            if (this->_data == nullptr) {
                this->_capacity = 1;
                this->_size = 0;
                this->_data = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * this->_capacity));
            }

            if (this->_size + 1 > this->_capacity) {
                grow();
            }

            detail::copy_construct(this->_data + this->_size++, elm);
        }

        void push_back(T &&elm) noexcept {
            if (this->_data == nullptr) {
                this->_capacity = 1;
                this->_size = 0;
                this->_data = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * this->_capacity));
            }

            if (this->_size + 1 > this->_capacity) {
                grow();
            }

            detail::move_construct(this->_data + this->_size++, kstd::move(elm));
        }

        template<typename... Args>
        void emplace_back(Args&&... args) noexcept {
            if (this->_data == nullptr) {
                this->_capacity = 1;
                this->_size = 0;
                this->_data = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * this->_capacity));
            }

            if (this->_size + 1 > this->_capacity) {
                grow();
            }

            new (this->_data + this->_size++) T(kstd::forward<Args>(args)...);
        }

        void pop_back() noexcept {
            if (this->_size == 0 || this->_data == nullptr) {
                kstl_globals::g_init_data.panic();
            }

            this->_data[this->_size - 1].~T();
            this->_size--;
        }

        void reserve(size_t elements) noexcept {
            if (this->_data == nullptr) {
                this->_capacity = 1;
                this->_size = 0;
                this->_data = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * this->_capacity));
            }

            if (elements < this->_capacity) {
                return;
            }

            // if (this->_data == nullptr) { asm volatile ("ud2"); }

            size_t old_capacity = this->_capacity;
            this->_capacity = kstd::max(elements, _capacity * 2);
            T *new_data = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * this->_capacity));
            detail::move_array(new_data, this->_data, old_capacity);
            kstl_globals::free(this->_data);
            this->_data = new_data;
        }

        void resize(size_t elements) {
            if (this->_data == nullptr) {
                this->_capacity = 1;
                this->_size = 0;
                this->_data = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * this->_capacity));
            }

            T* new_data = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * elements));

            size_t copy_count = (elements < _size) ? elements : _size;

            detail::move_array(new_data, this->_data, copy_count);

            for (size_t i = copy_count; i < elements; ++i) {
                new (&new_data[i]) T();
            }

            kstl_globals::free(_data);

            _data = new_data;
            _size = elements;
            _capacity = elements;
        }

        void shrink_to_fit() noexcept {
            if (this->_capacity > this->_size) {
                size_t old_capacity = this->_capacity;
                this->_capacity = this->_size;
                T *new_data = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * this->_capacity));
                detail::move_array(new_data, this->_data, old_capacity);
                kstl_globals::free(this->_data);
                this->_data = new_data;
            }
        }
    public:
        T& operator[](size_t index) noexcept {
            return this->_data[index];
        }

        const T& operator[](size_t index) const noexcept {
            return this->_data[index];
        }

        vector& operator=(const vector& other) noexcept {
            if (this == &other) {
                return *this;
            }

            for (size_t i = 0; i < this->_size; ++i) {
                this->_data[i].~T();
            }

            kstl_globals::free(this->_data);

            this->_size = other._size;
            this->_capacity = other._capacity;

            this->_data = reinterpret_cast<T*>(
                kstl_globals::malloc(sizeof(T) * this->_capacity)
            );

            detail::copy_array(this->_data, other._data, this->_size);

            return *this;
        }

        vector& operator=(vector &&v) noexcept {
            if (this == &v) return *this;

            for (size_t i = 0; i < this->_size; ++i) {
                this->_data[i].~T();
            }

            kstl_globals::free(this->_data);

            this->_size = v._size;
            this->_capacity = v._capacity;
            this->_data = v._data;

            v._data = nullptr;
            v._capacity = 0;
            v._size = 0;

            return *this;
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

        const_iterator cbegin() const noexcept {
            return const_iterator(this->_data);
        }

        const_iterator cend() const noexcept {
            return const_iterator(this->_data + this->_size);
        }
    public:
        vector() = default;

        vector(const vector &v) requires std::copy_constructible<T> {
            this->_size = v._size;
            this->_capacity = v._capacity;
            this->_data = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * this->_capacity));
            for (size_t i = 0; i < this->_size; i++) {
                new (this->_data + i) T(v._data[i]);
            }
        }

        vector(size_t sz) {
            this->_size = sz;
            this->_capacity = sz;
            this->_data = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * this->_capacity));
            for (size_t i = 0; i < this->_size; ++i) {
                new (this->_data + i) T();
            }
        }

        vector(std::initializer_list<T> list) {
            this->_size = 0;
            this->_capacity = list.size();
            this->_data = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * this->_capacity));

            for (const T &item : list) {
                this->push_back(item);
            }
        }

        vector(vector &&v) {
            this->_size = v._size;
            this->_capacity = v._capacity;
            this->_data = v._data;

            v._data = nullptr;
            v._capacity = 0;
            v._size = 0;
        }
    public:
        ~vector() {
            if (this->_data != nullptr) {
                for (size_t i = 0; i < this->_size; ++i) {
                    this->_data[i].~T();
                }

                kstl_globals::free(this->_data);
            }
        }
    };

    template<typename T>
    bool operator==(const vector<T> &lhs, const vector<T> &rhs) noexcept {
        if (&lhs == &rhs) return true;
        if (lhs.size() != rhs.size()) return false;

        for (size_t i = 0; i < lhs.size(); ++i) {
            if (lhs[i] != rhs[i]) {
                return false;
            }
        }

        return true;
    }

    template<typename T>
    bool operator!=(const vector<T> &lhs, const vector<T> &rhs) noexcept {
        return !operator==(lhs, rhs);
    }
}
