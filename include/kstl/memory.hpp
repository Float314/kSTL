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
    struct __calldestructor_default_delete {
        void operator()(T *ptr) const {
            ptr->~T();
            kstl_globals::free(ptr);
        }
    };

    template<typename T>
    using default_delete = __calldestructor_default_delete<T>;

    template<typename T, typename Deleter = default_delete<T>>
    class unique_ptr {
        static_assert(!kstd::is_array_v<T>, "unique_ptr<T[]> is not supported");
    private:
        T *ptr = nullptr;
        [[no_unique_address]] Deleter deleter;
    public:
        T* get() const noexcept {
            return this->ptr;
        }

        Deleter get_deleter() const noexcept {
            return this->deleter;
        }
    public:
        T* release() noexcept {
            T* ret_ptr = this->ptr;
            this->ptr = nullptr;
            return ret_ptr;
        }

        void reset(T *ptr = nullptr) noexcept {
            if (this->ptr != nullptr) {
                deleter(this->ptr);
            }

            this->ptr = ptr;
        }
    public:
        void swap(unique_ptr<T> &other) noexcept {
            kstd::swap(this->ptr, other.ptr);
        }
    public:
        explicit operator bool() const noexcept {
            return this->ptr != nullptr;
        }

        T* operator->() const noexcept {
            return this->ptr;
        }

        T& operator*() const noexcept {
            return *this->ptr;
        }

        unique_ptr& operator=(const unique_ptr &) noexcept = delete;

        unique_ptr& operator=(unique_ptr &&other) noexcept {
            this->ptr = other.ptr;
            this->deleter = other.deleter;

            other.ptr = nullptr;
            
            return *this;
        }
    public:
        unique_ptr(T *ptr) : ptr(ptr) {}
        unique_ptr() : ptr(nullptr) {}

        unique_ptr(const unique_ptr &other) = delete;
        unique_ptr(unique_ptr &&other) {
            this->ptr = other.ptr;
            this->deleter = other.deleter;

            other.ptr = nullptr;
        }
    public:
        ~unique_ptr() {
            if (ptr != nullptr) {
                deleter(ptr);
            }
        }
    };

    template<typename T, typename... Args>
    unique_ptr<T> make_unique(Args&&... args) {
        T *ptr = reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T)));
        if (ptr == nullptr) {
            kstl_globals::g_init_data.panic();
        }
        new (ptr) T(kstd::forward<Args>(args)...);
        return unique_ptr<T>(ptr);
    }

    // TODO: Implement shared_ptr

//     template<typename T, typename Deleter = default_delete<T>>
//     class shared_ptr {
//         static_assert(!kstd::is_array_v<T>, "shared_ptr<T[]> is not supported");
//     private:
//         struct control_block {
//
//         };
//
//         T *ptr = nullptr;
// #ifdef KSTL_SHARED_PTR_ATOMIC_REFCOUNT
//         static_assert(false, "shared_ptr with atomic reference count is not supported in an embedded environment");
// #else
//
// #endif
//         [[no_unique_address]] Deleter deleter;
//     public:
//         T* get() const noexcept {
//             return this->ptr;
//         }
//
//         Deleter get_deleter() const noexcept {
//             return this->deleter;
//         }
//     public:
//         T* release() noexcept {
//             T* ret_ptr = this->ptr;
//             this->ptr = nullptr;
//             return ret_ptr;
//         }
//
//         void reset(T *ptr = nullptr) noexcept {
//             if (this->ptr != nullptr) {
//                 deleter(this->ptr);
//             }
//
//             this->ptr = ptr;
//         }
//     public:
//         void swap(shared_ptr<T> &other) noexcept {
//             kstd::swap(this->ptr, other.ptr);
//         }
//     public:
//         explicit operator bool() const noexcept {
//             return this->ptr != nullptr;
//         }
//
//         T* operator->() const noexcept {
//             return this->ptr;
//         }
//
//         T& operator*() const noexcept {
//             return *this->ptr;
//         }
//
//         shared_ptr& operator=(const shared_ptr &) noexcept = delete;
//
//         shared_ptr& operator=(shared_ptr &&other) noexcept {
//             this->ptr = other.ptr;
//             this->deleter = other.deleter;
//
//             other.ptr = nullptr;
//
//             return *this;
//         }
//     public:
//         shared_ptr(T *ptr) : ptr(ptr) {}
//         shared_ptr() : ptr(nullptr) {}
//
//         shared_ptr(const shared_ptr &other) = delete;
//         shared_ptr(shared_ptr &&other) {
//             this->ptr = other.ptr;
//             this->deleter = other.deleter;
//
//             other.ptr = nullptr;
//         }
//     public:
//         ~shared_ptr() {
//             if (ptr != nullptr) {
//                 deleter(ptr);
//             }
//         }
//     };
}
