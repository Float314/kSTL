#include <format>
#include <kstl/memory.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"
#include <kstl/string.hpp>
#include <kstl/vector.hpp>
#include <kstl/unordered_map.hpp>

template<>
struct kstd::hash<kstd::vector<int>> {
    size_t operator()(const kstd::vector<int> &vec) {
        std::size_t seed = vec.size();
        for (int x : vec) {
            seed ^= std::hash<int>{}(x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

using that_map_t = kstd::unordered_map<kstd::vector<int>, kstd::string>;

int main() {
    kstl::test::init();
    {
        that_map_t map;
        map[{4, 2}] = "Hello, world!";
        map[{2, 4}] = "Bye, world!";

        k_expect(map.at({4, 2}) == "Hello, world!");
        k_expect(map.at({2, 4}) == "Bye, world!");
    }
    kstl::test::end();
}

