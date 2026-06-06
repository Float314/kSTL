#pragma once

#include <string>
#include <exception>
#include "kstl/memory.hpp"

#define __k_stringify_impl(x) #x
#define __k_stringify(x) __k_stringify_impl(x)
#define k_expect(condition) kstl::test::expect((condition), __k_stringify(condition), __FILE__, __LINE__)
// #define k_expect(lhs, op, rhs) kstl::test::expect(((lhs) op (rhs)), __k_stringify(condition), __FILE__, __LINE__)

namespace kstl::test {
    void init(void (*panic_handler)(void) = std::terminate);
    void expect(bool expectation, const char *condition, const char *file = nullptr, int line_number = -1);
    void log(std::string prefix, std::string data);
    void end();

    template<typename T>
    class null_allocator {
    public:
        constexpr T* address(T &x) const noexcept {
            return addressof(x);
        }
        
        constexpr const T* address(const T &x) const noexcept {
            return addressof(x);
        }

        constexpr T* allocate(size_t n) noexcept {
            return nullptr;
        }

        constexpr void deallocate(T *p, size_t /* n -> original n passed in by allocate */) noexcept {
            (void) p;
        }

        constexpr size_t max_size() const noexcept {
            return 0;
        }

        void construct(T *p, const T &value) noexcept {
            new (p) T(value);
        }

        void destroy(T *p) noexcept {
            p->~T();
        }
    public:
        constexpr null_allocator() noexcept = default;
        constexpr null_allocator(const null_allocator &) noexcept = default;
    public:
        ~null_allocator() = default;
    };

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

}
