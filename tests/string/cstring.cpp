#include <format>
#include <kstl/string.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"

namespace kstl_globals {
    extern kstd::rt_init g_init_data;
    extern void* (*malloc)(size_t);
    extern void (*free)(void*);
}

int main() {
    kstl::test::init();
    {
        {
            const char *string = "This is 26 characters long";
            k_expect(kstd::strlen(string) == 26);
            kstl::test::log("expectation", "kstd::strlen(string) -> 26");
        }

        {
            const char *s1 = "ABC";
            const char *s2 = "ABC";
            k_expect(kstd::streq(s1, s2));
            kstl::test::log("expectation", "kstd::streq(s1, s2) -> true");
        }

        {
            const char *s1 = "abc";
            const char *s2 = "abcd";
            k_expect(!kstd::streq(s1, s2));
            kstl::test::log("expectation", "kstd::streq(s1, s2) -> false");
        }

        {
            const char *s1 = "abc";
            const char *s2 = "def";
            k_expect(!kstd::streq(s1, s2));
            kstl::test::log("expectation", "kstd::streq(s1, s2) -> false");
        }
    }
    kstl::test::end();
}
