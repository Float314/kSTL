#pragma once

#include <string>
#include <exception>

#define __k_stringify_impl(x) #x
#define __k_stringify(x) __k_stringify_impl(x)
#define k_expect(condition) kstl::test::expect((condition), __k_stringify(condition))

namespace kstl::test {
    void init(void (*panic_handler)(void) = std::terminate);
    void expect(bool expectation, const char *condition);
    void log(std::string prefix, std::string data);
    void end();
}
