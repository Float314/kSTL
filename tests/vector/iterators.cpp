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
        bool accumulator = false;
        for (size_t i = 0; i < v.size(); ++i) {
            accumulator += v.data()[i] == fill_value;
        }

        kstl::test::expect(accumulator, "v == {42, 42, 42, 42}");

        kstd::vector<int> v2(4);
        kstl::test::log_vector_i(v2, "v2");
        kstl::test::log("function", std::format("kstd::copy(v.begin(), v.end(), v2.begin())"));
        kstd::copy(v.begin(), v.end(), v2.begin());
        kstl::test::log_vector_i(v2, "v2");

        accumulator = false;
        for (size_t i = 0; i < v2.size(); ++i) {
            accumulator += v2.data()[i] == v.data()[i];
        }

        kstl::test::expect(accumulator, "v2 == {42, 42, 42, 42}");

        k_expect(*v.begin() == v[0]);
        k_expect(*(v.end() - 1) == v[v.size() - 1]);
    }
    kstl::test::end();
}

