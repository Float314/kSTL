#include <format>
#include <kstl/vector.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"
#include "vector/log_vector.hpp"

bool g_panic_now = false;

int main() {
    kstl::test::init([]() {
        if (!g_panic_now) {
            std::terminate();
        }
        kstl::test::log("NOTICE", "!! Intercepted Panic !!");
        g_panic_now = false;
    });
    {
        kstd::vector<int, kstl::test::null_allocator<int>> vec;
        g_panic_now = true;
        vec.push_back(42);
    }
    kstl::test::end();
}

