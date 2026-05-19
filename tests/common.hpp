#pragma once

#include <string>
namespace kstl::test {
    void init();
    void log(std::string prefix, std::string data);
    void end();
}
