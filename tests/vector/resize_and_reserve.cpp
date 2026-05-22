#include <format>
#include <kstl/vector.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"
#include "vector/log_vector.hpp"

int main() {
    kstl::test::init();
    {
        kstl::test::log("construct", std::format("kstd::vector<int> v = {}", "{}"));
        kstd::vector<int> v = {};
        k_expect(v.size() == 0);
        v.reserve(20);
        k_expect(v.size() == 0 && v.capacity() == 20);
        v.resize(16);
        k_expect(v.size() == 16 && v.capacity() == 16);
    }
    kstl::test::end();
}

