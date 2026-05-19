#include <kstl/runtime.hpp>
#include <cstddef>
#include <new>

namespace kstl_globals {
    kstd::rt_init g_init_data;
    void* (*malloc)(size_t) = nullptr;
    void (*free)(void*) = nullptr;
    void* (*aligned_alloc)(size_t, size_t) = nullptr;
}

namespace kstd {
    error_code<void> init(const rt_init &init_data) {
        kstl_globals::g_init_data = init_data;
        kstl_globals::malloc = [](size_t sz) -> void* {
            void *mem = kstl_globals::g_init_data.allocator_info.alloc(sz);
            if (mem == nullptr) {
                kstl_globals::g_init_data.panic();
            }

            return mem;
        };
        kstl_globals::free = init_data.allocator_info.free;
        kstl_globals::aligned_alloc = [](size_t align, size_t sz) -> void* {
            return kstl_globals::g_init_data.allocator_info.alloc(sz);
        }; 

        if (
            init_data.allocator_info.alloc == nullptr
            || init_data.allocator_info.free == nullptr
        ) {
            return error_code<void>({}, error_category::rt_init_failure);
        }
        
        if (init_data.panic == nullptr) {
            return error_code<void>({}, error_category::rt_init_failure);
        }

        return error_code<void>({}, error_category::null);
    }
}

// void* operator new(size_t sz) {
//     if (void* mem = kstl_globals::malloc(sz)) {
//         return mem;
//     }
//
//     throw std::bad_alloc{};
// }
//
// void operator delete(void* mem) noexcept {
//     kstl_globals::free(mem);
// }
//
// void* operator new[](size_t sz) {
//     if (void* mem = kstl_globals::malloc(sz)) {
//         return mem;
//     }
//
//     throw std::bad_alloc{};
// }
//
// void operator delete[](void* mem) noexcept {
//     kstl_globals::free(mem);
// }
//
// void operator delete(void* mem, size_t sz) noexcept {
//     (void)sz;
//     kstl_globals::free(mem);
// }
//
// void operator delete[](void* mem, size_t sz) noexcept {
//     (void)sz;
//     kstl_globals::free(mem);
// }
//
// void* operator new(size_t sz, const std::nothrow_t&) noexcept {
//     return kstl_globals::malloc(sz);
// }
//
// void* operator new[](size_t sz, const std::nothrow_t&) noexcept {
//     return kstl_globals::malloc(sz);
// }
//
// void operator delete(void* mem, const std::nothrow_t&) noexcept {
//     kstl_globals::free(mem);
// }
//
// void operator delete[](void* mem, const std::nothrow_t&) noexcept {
//     kstl_globals::free(mem);
// }
//
// void* operator new(size_t sz, std::align_val_t align) {
//     if (void* mem = kstl_globals::aligned_alloc(
//             static_cast<size_t>(align),
//             sz)) {
//         return mem;
//     }
//
//     throw std::bad_alloc{};
// }
//
// void* operator new[](size_t sz, std::align_val_t align) {
//     if (void* mem = kstl_globals::aligned_alloc(
//             static_cast<size_t>(align),
//             sz)) {
//         return mem;
//     }
//
//     throw std::bad_alloc{};
// }
//
// void operator delete(void* mem, std::align_val_t) noexcept {
//     kstl_globals::free(mem);
// }
//
// void operator delete[](void* mem, std::align_val_t) noexcept {
//     kstl_globals::free(mem);
// }
//
// void operator delete(
//     void* mem,
//     size_t,
//     std::align_val_t) noexcept {
//
//     kstl_globals::free(mem);
// }
//
// void operator delete[](
//     void* mem,
//     size_t,
//     std::align_val_t) noexcept {
//
//     kstl_globals::free(mem);
// }
