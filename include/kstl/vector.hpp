#pragma once

#include <initializer_list>
#include <new>
#include "kstl/types.hpp"
#include "kstl/stdlib.hpp"
#include "kstl/utility.hpp"
#include "kstl/runtime.hpp"
#include "kstl/algorithm.hpp"

namespace kstl_globals {
    extern kstd::rt_init g_init_data;
    extern void* (*malloc)(size_t);
    extern void (*free)(void*);
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
                this->_size = 1;
                this->_data = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * this->_capacity));
            }

            if (this->_size + 1 > this->_capacity) {
                size_t old_capacity = this->_capacity;
                this->_capacity *= 2;
                T *new_data = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * this->_capacity));
                kstd::memcpy(new_data, this->_data, sizeof(T) * old_capacity);
                kstl_globals::free(this->_data);
                this->_data = new_data;
            }

            new (this->_data + this->_size++) T(elm);
        }

        void push_back(T &&elm) noexcept {
            if (this->_data == nullptr) {
                this->_capacity = 1;
                this->_size = 1;
                this->_data = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * this->_capacity));
            }

            if (this->_size + 1 > this->_capacity) {
                size_t old_capacity = this->_capacity;
                this->_capacity *= 2;
                T *new_data = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * this->_capacity));
                kstd::memcpy(new_data, this->_data, sizeof(T) * old_capacity);
                kstl_globals::free(this->_data);
                this->_data = new_data;
            }

            new (this->_data + this->_size++) T(elm);
        }

        void pop_back() noexcept {
            if (this->_size == 0 || this->_data == nullptr) {
                kstl_globals::g_init_data.panic();
            }

            this->_data[this->_size - 1].~T();
            this->_size--;
        }

        void reserve(size_t bytes) noexcept {    
            if (bytes < this->_capacity) {
                return;
            }

            size_t old_capacity = this->_capacity;
            this->_capacity *= kstd::max(bytes, _capacity * 2);
            T *new_data = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * this->_capacity));
            kstd::memcpy(new_data, this->_data, sizeof(T) * old_capacity);
            kstl_globals::free(this->_data);
            this->_data = new_data;
        }

        void shrink_to_fit() noexcept {
            if (this->_capacity > this->_size) {
                this->_capacity = this->_size;
                T *new_data = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * this->_capacity));
                kstd::memcpy(new_data, this->_data, sizeof(T) * this->_capacity);
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

            for (size_t i = 0; i < this->_size; ++i) {
                new (&this->_data[i]) T(other._data[i]);
            }

            return *this;
        }

        vector& operator=(vector &&v) noexcept {
            if (this == &v) return;

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
    public:
        vector() = default;

        vector(const vector &v) {
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
}
