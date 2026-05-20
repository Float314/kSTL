#include <format>
#include <kstl/string.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"

int main() {
    kstl::test::init();
    {
        kstl::test::log("construct", std::format("kstd::string s = {}", "\"Hello\""));
        kstd::string s = "Hello";
        k_expect(kstd::streq(s.data(), "Hello"));
        kstl::test::log("var(s)", s.data());
        kstl::test::log("operator", std::format("s += {}", "\", world!\""));
        s += ", world!";
        k_expect(kstd::streq(s.data(), "Hello, world!"));
        kstl::test::log("var(s)", s.data());
        k_expect(!s.empty());
        kstl::test::log("function", std::format("s.empty() -> {}", s.empty()));
    }
    kstl::test::end();
}
