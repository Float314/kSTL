#include <format>
#include <kstl/vector.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"
#include "vector/log_vector.hpp"

int main() {
    kstl::test::init();
    {
        kstl::test::log("construct", std::format("kstd::vector<int> v = {}", "{1, 2, 3, 4, 5, 6, 7, 8, 9}"));
        kstd::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        k_expect(v.size() == 9);
        kstl::test::log_vector_i(v, "v");
        int pb_value = 42;
        kstl::test::log("function", std::format("v.push_back({})", pb_value));
        v.push_back(pb_value);
        k_expect(v.data()[v.size() - 1] == 42);
        kstl::test::log_vector_i(v, "v");
    }
    kstl::test::end();
}

