#include <format>
#include <kstl/vector.hpp>
#include <kstl/runtime.hpp>
#include "common.hpp"
#include "vector/log_vector.hpp"

struct log_int {
    int inner;
    log_int& operator++() {
        ++inner;
        kstl::test::log("log_int", "increment");
        return *this;
    }

    log_int& operator--() {
        --inner;
        kstl::test::log("log_int", "decrement");
        return *this;
    }

    operator int() {
        return inner;
    }

    log_int() = default;
    log_int(int n) : inner(n) {}
};

struct logger_t {
    static log_int g_alive;

    logger_t() {
        ++g_alive;
    }

    logger_t(logger_t &&logger) {
        ++g_alive;
    }

    logger_t& operator=(const logger_t &other) {
        return *this;
    }

    logger_t& operator=(logger_t &&other) {
        return *this;
    }

    ~logger_t() {
        --g_alive;
    }
};

log_int logger_t::g_alive = 0;

int main() {
    kstl::test::init();
    {
        k_expect(logger_t::g_alive == 0);

        {
            kstd::vector<logger_t> lvec;
            for (size_t i = 0; i < 10; ++i) {
                lvec.emplace_back();
            }
        }

        k_expect(logger_t::g_alive == 0);
    }
    kstl::test::end();
}

