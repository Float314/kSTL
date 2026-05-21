#include "common.hpp"
#include "kstl/runtime.hpp"
#include <cstdlib>
#include <cstdio>
#include <format>

namespace std {
    void terminate(void) noexcept;
}

size_t g_memory_allocated = 0;
size_t g_memory_allocations = 0;

size_t g_total_allocations = 0;
size_t g_total_mem_allocated = 0;

namespace kstl_globals {
    extern kstd::rt_init g_init_data;
}

namespace kstl::test {
    void init(void (*panic_handler)(void)) {
        struct header {
            size_t sz;
        };
        kstd::rt_init init_data = {
            .allocator_info = {
                .alloc = [](size_t size) -> void* {
                    void *mem = malloc(size + sizeof(header));
                    printf("[A]: %zu bytes -> %p\n", size, reinterpret_cast<void*>((char*) mem + sizeof(header)));
                    g_memory_allocated += size;
                    g_memory_allocations += 1;
                    ++g_total_allocations;
                    g_total_mem_allocated += size;
                    ((header*) mem)->sz = size;
                    return reinterpret_cast<void*>((char*) mem + sizeof(header));
                },
                .free = [](void *mem) -> void {
                    header *h = (header*) ((char*) mem - sizeof(header));
                    g_memory_allocated -= h->sz;
                    g_memory_allocations -= 1;
                    printf("[F]: %zu bytes -> %p\n", h->sz, mem);

                    free(h);
                }
            },
            .panic = panic_handler
        };

        auto ec = kstd::init(init_data);
        if (!ec) init_data.panic();
    }

    void log(std::string prefix, std::string data) {
        printf("%s: %s\n", prefix.c_str(), data.c_str());
    }

    void end() {
        printf("--- Memory Status ---\n");
        printf("Unfreed Memory: %zu bytes\n", g_memory_allocated);
        printf("Unfreed Allocations: %zu\n", g_memory_allocations);
        printf("\n");
        printf("Total Memory Allocated: %zu bytes\n", g_total_mem_allocated);
        printf("Total Memory Allocations: %zu\n", g_total_allocations);
        printf("--- ------------- ---\n");
    }

    void expect(bool expectation, const char *condition) {
        if (!expectation) {
            log("EXPECTATION FAILURE", std::format("{}", condition));
            end();
            fflush(stdout);
            fflush(stderr);
            kstl_globals::g_init_data.panic();
        }
    }
}
