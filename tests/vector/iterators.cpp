#include <format>
#include <kstl/vector.hpp>
#include <kstl/runtime.hpp>
#include <kstl/algorithm.hpp>
#include "common.hpp"
#include "vector/log_vector.hpp"

int main() {
    kstl::test::init();
    {
        kstl::test::log("construct", std::format("kstd::vector<int> v = {}", "{1, 2, 3, 4}"));
        kstd::vector<int> v = {1, 2, 3, 4};
        kstl::test::log_vector_i(v, "v");
        int fill_value = 42;
        kstl::test::log("function", std::format("kstd::fill(v.begin(), v.end(), {})", fill_value));
        kstd::fill(v.begin(), v.end(), fill_value);

        kstd::vector<int> v2(4);
        kstl::test::log_vector_i(v2, "v2");
        kstl::test::log("function", std::format("kstd::copy(v.begin(), v.end(), v2.begin())"));
        kstd::copy(v.begin(), v.end(), v2.begin());
        kstl::test::log_vector_i(v2, "v2");
    }
    kstl::test::end();
}

