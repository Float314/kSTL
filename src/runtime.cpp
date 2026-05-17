#include <kstl/runtime.hpp>

namespace kstl_globals {
    kstd::rt_init g_init_data;
    void* (*malloc)(size_t) = nullptr;
    void (*free)(void*) = nullptr;
}

namespace kstd {
    error_code<void> init(const rt_init &init_data) {
        kstl_globals::g_init_data = init_data;

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
//     return kstl_globals::malloc(sz);
// }
//
// void operator delete(void *mem) noexcept {
//     kstl_globals::free(mem);
// }
