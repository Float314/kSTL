#include <iostream>
#include <kstl/runtime.hpp>
#include <kstl/string.hpp>

extern "C" void* malloc(size_t);
extern "C" void free(void*);

namespace std {
    void terminate() noexcept;
}

int main() {
    kstd::rt_init init_data = {
        .allocator_info = {
            .alloc = malloc,
            .free = free
        },
        .panic = std::terminate
    };
    auto err = kstd::init(init_data);

    if (!err) {
        init_data.panic();
    }

    kstd::string string = "Hello, world!";
    std::cout << string.data() << '\n';
    string.append(" and even more stuff!!!");
    std::cout << string.data() << '\n';
    string.reserve(1);
    std::cout << string.data() << '\n';
    std::cout << string.capacity() << '\n';
    string.shrink_to_fit();
    std::cout << string.capacity() << '\n';
}
