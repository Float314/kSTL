#include <format>
#include <kstl/unordered_map.hpp>
#include <kstl/runtime.hpp>
#include <string>
#include "common.hpp"

int main() {
    kstl::test::init();
    {
        kstd::unordered_map<int, int> hundred_map;
    }
    kstl::test::end();
}

