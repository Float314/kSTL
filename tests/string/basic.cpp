#include <format>
#include <kstl/string.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"

int main() {
    kstl::test::init();
    {
        kstl::test::log("Construct", std::format("kstd::string s = {}", "\"Hello\""));
        kstd::string s = "Hello";
        kstl::test::log("var(s)", s.data());
        kstl::test::log("Operator", std::format("s += {}", "\", world!\""));
        s += ", world!";
        kstl::test::log("var(s)", s.data());
        kstl::test::log("s.is_empty()", std::format("{}", s.is_empty()));
    }
    kstl::test::end();
}
