#pragma once

#include <kstl/cstddef.hpp>
#include <kstl/macros.hpp>
#include <kstl/runtime.hpp>
#include <kstl/stdlib.hpp>
#include <kstl/utility.hpp>

namespace kstl_globals {
    extern kstd::rt_init g_init_data;
    extern void* (*malloc)(size_t);
    extern void (*free)(void*);
}

namespace kstd::detail {
    template<typename T>
    struct manual_destructor_call_default_delete {
        void operator()(T *ptr) const noexcept {
            ptr->~T();
            kstl_globals::free(ptr);
        }
    };

    template<typename T>
    struct operator_delete_default_delete {
        void operator()(T *ptr) const noexcept {
            delete ptr;
        }
    };
}

namespace kstd {
    template<typename T>
    constexpr T* addressof(T &arg) {
        return &arg; // TODO: Actually implement it
    }

    template<typename T>
    constexpr const T* addressof(const T &&arg) = delete;

    template<typename T, typename... Args>
    constexpr T* construct_at(T *location, Args&&... args) {
        return new (location) T(kstd::forward<Args>(args)...);
    }

    template<typename T>
    constexpr void destroy_at(T *location) {
        location->~T();
    }
}

namespace kstd {
    template<typename T>
    class allocator {
    public:
        constexpr T* address(T &x) const noexcept {
            return addressof(x);
        }
        
        constexpr const T* address(const T &x) const noexcept {
            return addressof(x);
        }

        constexpr T* allocate(size_t n) noexcept {
            return reinterpret_cast<T*>(kstl_globals::malloc(sizeof(T) * n));
        }

        constexpr void deallocate(T *p, size_t /* n -> original n passed in by allocate */) noexcept {
            kstl_globals::free(p);
        }

        constexpr size_t max_size() const noexcept {
            return SIZE_MAX; // TODO: Get max size of allocation
        }

        void construct(T *p, const T &value) noexcept {
            new (p) T(value);
        }

        void destroy(T *p) noexcept {
            p->~T();
        }
    public:
        constexpr allocator() noexcept = default;
        constexpr allocator(const allocator &) noexcept = default;
    public:
        ~allocator() = default;
    };

    template<typename T>
    constexpr bool operator==(const allocator<T> &lhs, const allocator<T> &rhs) {
        return true;
    }

    template<typename T>
    constexpr bool operator!=(const allocator<T> &lhs, const allocator<T> &rhs) {
        return !operator==(lhs, rhs);
    }
}

namespace kstd {
    template<typename T>
    using default_delete = detail::manual_destructor_call_default_delete<T>;

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

    template<typename T, typename Deleter>
    bool operator==(const unique_ptr<T, Deleter> &lhs, const unique_ptr<T, Deleter> &rhs) noexcept {
        return lhs.get() == rhs.get();
    }

    template<typename T, typename Deleter>
    bool operator==(const unique_ptr<T, Deleter> &lhs, kstd::nullptr_t) noexcept {
        return lhs.get() == nullptr;
    }

    template<typename T, typename Deleter>
    bool operator==(kstd::nullptr_t, const unique_ptr<T, Deleter> &rhs) noexcept {
        return rhs.get() == nullptr;
    }

    template<typename T, typename Deleter>
    bool operator!=(const unique_ptr<T, Deleter> &lhs, const unique_ptr<T, Deleter> &rhs) noexcept {
        return lhs.get() != rhs.get();
    }

    template<typename T, typename Deleter>
    bool operator!=(const unique_ptr<T, Deleter> &lhs, kstd::nullptr_t) noexcept {
        return lhs.get() != nullptr;
    }

    template<typename T, typename Deleter>
    bool operator!=(kstd::nullptr_t, const unique_ptr<T, Deleter> &rhs) noexcept {
        return rhs.get() != nullptr;
    }

    template<typename T, typename... Args>
    unique_ptr<T> make_unique(Args&&... args) {
        void* mem = kstl_globals::malloc(sizeof(T));

        if (!mem) {
            kstl_globals::g_init_data.panic();
        }

        T* ptr = static_cast<T*>(mem);
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
