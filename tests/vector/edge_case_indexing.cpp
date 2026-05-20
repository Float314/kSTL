#include <format>
#include <kstl/vector.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"
#include "vector/log_vector.hpp"

bool g_panic_now = true;

int main() {
    kstl::test::init([]() {
        if (!g_panic_now) {
            std::terminate();
        }
        kstl::test::log("NOTICE", "!! Intercepted Panic !!");
        g_panic_now = false;
    });
    {
        kstl::test::log("construct", std::format("kstd::vector<int> v = {}", "{1, 2, 3}"));
        kstd::vector<int> v = {1, 2, 3};
        kstl::test::log_vector_i(v, "v");

        k_expect(v.front() == 1);
        k_expect(v.back() == 3);

        g_panic_now = true;
        {
            auto _ = v.at(-1);
        }

        // shouldn't panic
        {
            auto _ = v[-1];
        }
    }
    kstl::test::end();
}

