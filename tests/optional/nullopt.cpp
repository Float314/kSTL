#include <format>
#include <kstl/memory.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"
#include "kstl/optional.hpp"

int main() {
    kstl::test::init();
    {
        kstd::optional<int> o1 = kstd::nullopt;
        kstd::optional<int> o2;

        k_expect(o1.has_value() == o2.has_value());
        k_expect(o1.value_or(-1) == o2.value_or(-1));
    }
    kstl::test::end();
}

