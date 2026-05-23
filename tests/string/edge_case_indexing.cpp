#include <format>
#include <kstl/string.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"

bool g_panic_now = true;

int main() {
    kstl::test::init([]() {
        if (!g_panic_now) {
            std::terminate();
        }
        kstl::test::log("NOTICE", "!! Intercepted Panic !!");
        g_panic_now = false;
    });
    {
        kstl::test::log("construct", std::format("kstd::string s = {}", "\"Hey there.\""));
        kstd::string s = "Hey there.";

        k_expect(s.front() == 'H');
        k_expect(s.back() == '.');

        g_panic_now = true;
        {
            auto _ = s.at(-1);
        }

        // shouldn't panic
        {
            auto _ = s[-1];
        }
    }
    kstl::test::end();
}


