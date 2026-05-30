#include <format>
#include <kstl/unordered_map.hpp>
#include <kstl/runtime.hpp>
#include <kstl/algorithm.hpp>
#include "common.hpp"

int main() {
    kstl::test::init();
    {
        kstd::unordered_map<int, int> hundred_map;
        hundred_map[1] = 100;
        hundred_map[2] = 200;
        hundred_map[3] = 300;
        hundred_map[4] = 400;
        hundred_map[5] = 500;
        hundred_map[6] = 600;
        hundred_map[7] = 700;
        hundred_map[8] = 800;
        hundred_map[9] = 900;
        hundred_map[10] = 1000;

        for (auto &[k, v] : hundred_map) {
            k_expect(v == k * 100);
        }

        k_expect(hundred_map.begin() != hundred_map.end());
        auto start = hundred_map.begin();
        while (++start != hundred_map.end());
        k_expect(start == hundred_map.end());
    }
    kstl::test::end();
}
