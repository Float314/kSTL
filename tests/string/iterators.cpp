#include <format>
#include <kstl/string.hpp>
#include <kstl/runtime.hpp>
#include <kstl/algorithm.hpp>
#include "common.hpp"

int main() {
    kstl::test::init();
    {
        kstl::test::log("construct", std::format("kstd::string v = {}", "\"Hey there.\""));
        kstd::string s = "Hey there.";
        int fill_value = 'C';
        kstl::test::log("function", std::format("kstd::fill(s.begin(), s.end(), {})", fill_value));
        kstd::fill(s.begin(), s.end(), fill_value);
        bool accumulator = false;
        for (auto it = s.begin(); it != s.end(); ++it) {
            accumulator += *it == fill_value;
        }

        kstl::test::expect(accumulator, "v == {/* filled with 'C' */}");

        kstd::string s2 = "EMPTY STR!";
        kstl::test::log("function", std::format("kstd::copy(s.begin(), s.end(), s2.begin())"));
        kstd::copy(s.begin(), s.end(), s2.begin());

        accumulator = false;
        for (size_t i = 0; i < s2.size(); ++i) {
            accumulator += s2.data()[i] == s.data()[i];
        }

        kstl::test::expect(accumulator, "v2 == {/* filled with 'C' */}");

        k_expect(*s.begin() == s[0]);
        k_expect(*(s.end() - 1) == s[s.size() - 1]);
    }
    kstl::test::end();
}
