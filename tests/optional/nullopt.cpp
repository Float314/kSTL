#include <format>
#include <kstl/memory.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"
#include "kstl/optional.hpp"

int main() {
    kstl::test::init();
    {
        kstd::optional<int> o1 = kstd::nullopt;
    }
    kstl::test::end();
}

