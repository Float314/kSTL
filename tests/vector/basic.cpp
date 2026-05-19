#include <format>
#include <kstl/vector.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"

void log_vector(const kstd::vector<int> &v, std::string variable_name) {
    std::string v_str_repr = "{ ";
    for (const auto &i : v) {
        v_str_repr += std::to_string(i) + ", ";
    }
    v_str_repr = v_str_repr.substr(0, v_str_repr.size() - 2);
    v_str_repr += " }";
    kstl::test::log("var(" + variable_name + ")", v_str_repr);
}

int main() {
    kstl::test::init();
    {
        kstl::test::log("Construct", std::format("kstd::vector<int> v = {}", "{1, 2, 3, 4, 5, 6, 7, 8, 9}"));
        kstd::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        log_vector(v, "v");
        int pb_value = 42;
        kstl::test::log("Function", std::format("v.push_back({})", pb_value));
        v.push_back(pb_value);
        log_vector(v, "v");
    }
    kstl::test::end();
}

