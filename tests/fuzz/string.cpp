#include <format>
#include <kstl/string.hpp>
#include <kstl/runtime.hpp>
#include <string>
#include "common.hpp"

extern "C" int LLVMFuzzerInitialize(int *, char ***) {
    kstl::test::init();
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    kstd::string kstl((char*) data, size);
    std::string std((char*) data, size);

    k_expect(kstl.size() == std.size());
    k_expect(kstl.empty() == std.empty());
    return 0;
}

static struct cleanup_t {
    ~cleanup_t() {
        kstl::test::end();
    }
} cleanup;
