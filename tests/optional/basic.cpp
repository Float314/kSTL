#include <format>
#include <kstl/memory.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"
#include "kstl/optional.hpp"

kstd::optional<float> divide(float x, float y) {
    if (y == 0) {
        return {};
    }

    return { x / y };
}

int main() {
    kstl::test::init();
    {
        auto o1 = divide(4, 0);
        auto o2 = divide(4, 2);

        kstl::test::log("var(o1)", std::format("{}", o1.value_or(0)));
        if (o1) {
            kstl::test::log("operator*(o1)", std::format("{}", *o1));
        }
        kstl::test::log("var(o2)", std::format("{}", o2.value_or(0)));
    }
    kstl::test::end();
}
