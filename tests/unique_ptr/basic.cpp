#include <format>
#include <kstl/memory.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"
#include "kstl/string.hpp"

struct some_struct_t {
    uint32_t age = 0;
    char name[512] = {};
};

void fn_ref(kstd::unique_ptr<some_struct_t> &ptr) {
    ptr->age = 42;
    const char name[] = "Michael Clang";
    kstd::strncpy(ptr->name, name, sizeof(name));
}

void fn_cref(const kstd::unique_ptr<some_struct_t> &ptr) {
    kstl::test::log("var(ptr->age)", std::format("{}", ptr->age));
    kstl::test::log("var(ptr->name)", std::format("{}", ptr->name));
}

int main() {
    kstl::test::init();
    {
        kstl::test::log("construct", "kstd::unique_ptr<some_struct_t> ptr = kstd::make_unique<some_struct_t>()");
        kstd::unique_ptr<some_struct_t> ptr = kstd::make_unique<some_struct_t>();
        fn_ref(ptr);
        fn_cref(ptr);
    }
    kstl::test::end();
}
