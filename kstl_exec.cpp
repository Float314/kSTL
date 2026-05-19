#include "kstl/vector.hpp"
#include <iostream>
#include <kstl/runtime.hpp>
#include <kstl/string.hpp>

extern "C" void* malloc(size_t);
extern "C" void free(void*);

namespace std {
    void terminate() noexcept;
}

kstd::string str() {
    return {};
}

int main() {
    kstd::rt_init init_data = {
        .allocator_info = {
            .alloc = [](size_t size) -> void* {
                void *mem = malloc(size);
                printf("[A]: %p of %zu\n", mem, size);
                return mem;
            },
            .free = [](void *mem) -> void {
                printf("[F]: %p\n", mem);
                free(mem);
            }
        },
        .panic = std::terminate
    };
    auto ec = kstd::init(init_data);

    if (!ec) {
        init_data.panic();
    }
}
