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

        float o1_v = o1.value_or(0);
        k_expect(o1_v == 0);
        kstl::test::log("var(o1.value_or(0))", std::format("{}", o1_v));
        if (o1) {
            k_expect(o1.has_value());
            kstl::test::log("operator*(o1)", std::format("{}", *o1));
        }
        float o2_v = o2.value_or(0);
        k_expect(o2_v != 0);
        kstl::test::log("var(o2)", std::format("{}", o2_v));
    }
    kstl::test::end();
}
