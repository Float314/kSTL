#include "vector/log_vector.hpp"
#include "common.hpp"

namespace kstl::test {
    void log_vector_i(const kstd::vector<int> &v, std::string variable_name) {
        std::string v_str_repr = "(kstd::vector<int>) " "{ ";
        for (const auto &i : v) {
            v_str_repr += std::to_string(i) + ", ";
        }
        v_str_repr = v_str_repr.substr(0, v_str_repr.size() - 2);
        v_str_repr += " }";
        kstl::test::log("var(" + variable_name + ")", v_str_repr);
    }
}
