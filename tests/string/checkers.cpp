#include <format>
#include <kstl/string.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"

int main() {
    kstl::test::init();
    {
        kstd::string s = "abcdefghijklmnopqrstuvwxyz0123456789";

        k_expect(s.contains("pqr"));
        k_expect(!s.contains("pf1"));
        k_expect(s.contains("789"));
        k_expect(s.contains("abc"));

        k_expect(s.starts_with("abc"));
        k_expect(!s.starts_with("bcd"));
        k_expect(s.starts_with('a'));
        k_expect(s.starts_with(kstd::string("abc")));

        k_expect(s.ends_with("789"));
        k_expect(!s.ends_with("678"));
        k_expect(s.ends_with('9'));
        k_expect(s.ends_with(kstd::string("789")));
    }
    kstl::test::end();
}
