#include <format>
#include <kstl/memory.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"
#include <kstl/string.hpp>
#include <kstl/vector.hpp>

int main() {
    kstl::test::init();
    {
        kstd::vector<kstd::string> strings;

        for (size_t i = 0; i < 10; ++i) {
            strings.push_back(kstd::to_string(i));
        }

        size_t i = 0;
        for (auto &s : strings) {
            k_expect(s == kstd::to_string(i++));
        }
    }
    kstl::test::end();
}

