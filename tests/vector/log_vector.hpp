#pragma once

#include <format>
#include <kstl/vector.hpp>
#include <string>
#include "common.hpp"

namespace kstl::test {
    void log_vector_i(const kstd::vector<int> &v, std::string variable_name);

    template<typename T, typename Fn>
    void log_vector(const kstd::vector<T> &v, std::string variable_name, Fn to_string, std::string type_name) {
        std::string v_str_repr = std::format("(kstd::vector<{}>) {}", type_name, "{ ");
        for (const auto &i : v) {
            v_str_repr += to_string(i) + ", ";
        }
        v_str_repr = v_str_repr.substr(0, v_str_repr.size() - 2);
        v_str_repr += " }";
        kstl::test::log("var(" + variable_name + ")", v_str_repr);
    }
}
