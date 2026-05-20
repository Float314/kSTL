#include <format>
#include <kstl/string.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"

int main() {
    kstl::test::init();
    {
        kstl::test::log("construct", std::format("kstd::string s = {}", "\"Hello\""));
        kstd::string s = "Hello";
        kstl::test::log("var(s)", s.data());
        kstl::test::log("operator", std::format("s += {}", "\", world!\""));
        s += ", world!";
        kstl::test::log("var(s)", s.data());
        kstl::test::log("function", std::format("s.is_empty() -> {}", s.is_empty()));
    }
    kstl::test::end();
}
