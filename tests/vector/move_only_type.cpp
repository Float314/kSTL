#include <format>
#include <kstl/vector.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"
#include "vector/log_vector.hpp"

struct move_only_t {
public:
    int valuable = 0;
public:
    move_only_t() = default;
    move_only_t(int value) : valuable(value) {};

    move_only_t(const move_only_t&) = delete;
    move_only_t& operator=(const move_only_t&) = delete;

    move_only_t(move_only_t&&) = default;
    move_only_t& operator=(move_only_t&&) = default;
public:
    ~move_only_t() = default;
};

int main() {
    kstl::test::init();
    {
        static_assert(!std::is_copy_constructible_v<kstd::vector<move_only_t>>);
        static_assert(std::is_move_constructible_v<kstd::vector<move_only_t>>);
    }
    kstl::test::end();
}


