#include <format>
#include <kstl/vector.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"
#include "vector/log_vector.hpp"

int main() {
    kstl::test::init();
    {
        kstd::vector<int> vector;
        kstl::test::log("Task", "Push-Back");
        for (size_t i = 0; i < 100000; ++i) {
            vector.push_back(i);
        }

        kstl::test::log("Task", "VectorCopy");
        kstd::vector<int> vector_copy = vector;
        kstl::test::log("Task", "VectorMove");
        kstd::vector<int> vector_move = kstd::move(vector);

        kstl::test::log("Task", "Done");
    }
    kstl::test::end();
}

