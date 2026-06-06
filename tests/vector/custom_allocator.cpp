#include <format>
#include <kstl/vector.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"
#include "vector/log_vector.hpp"

int main() {
    kstl::test::init();
    {
        kstd::vector<int, kstl::test::allocator<int>> vec;
        vec.push_back(42);
    }
    kstl::test::end();
}

