#include <format>
#include <kstl/utility.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"

struct custom1 {};
struct custom2 {};
struct custom3 {};
struct custom4 {};

#define SmartIsSame(x, y) kstd::is_same_v<kstd::remove_cvref_t<decltype(x)>, y>

int main() {
    kstl::test::init();
    {
        kstd::pair<custom1, custom2> p1;
        static_assert(SmartIsSame(p1.first, custom1));
        static_assert(SmartIsSame(p1.second, custom2));

        static_assert(SmartIsSame(kstd::get<0>(p1), custom1));
        static_assert(SmartIsSame(kstd::get<1>(p1), custom2));

        static_assert(SmartIsSame(kstd::get<custom1>(p1), custom1));
        static_assert(SmartIsSame(kstd::get<custom2>(p1), custom2));

        kstd::tuple<custom1, custom2, custom3, custom4> t1;
        static_assert(SmartIsSame(kstd::get<0>(t1), custom1));
        static_assert(SmartIsSame(kstd::get<1>(t1), custom2));
        static_assert(SmartIsSame(kstd::get<2>(t1), custom3));
        static_assert(SmartIsSame(kstd::get<3>(t1), custom4));
    }
    kstl::test::end();
}
