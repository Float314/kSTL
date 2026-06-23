#include <array>
#include <chrono>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

#if defined(__linux__) || defined(__linux)
#include <unistd.h>
#include <spawn.h>
#include <sys/wait.h>
#include <fcntl.h>
extern char **environ;

std::string repeat(char c, int n) {
    if (n == 0) return "";
    std::string s;
    s.reserve(n);
    for (int i = 0; i < n; ++i) {
        s.push_back(c);
    }
    return s;
}

#define KSTL_PRETTY_TEST_RUNNER

int main(int argc, char **argv) {
    bool fuzz_tests = true;

    for (size_t i = 0; i < argc; ++i) {
        if (std::strcmp(argv[i], "--no-fuzz") == 0) {
            fuzz_tests = false;
        }
    }

    std::cout << "\e[38;5;220m[[ kSTL Tester ]]\e[0m\n";

    int dev_null = open("/dev/null", O_WRONLY);
    std::atomic<int> passed = 0;
    std::atomic<int> failed = 0;
    std::atomic<int> skips = 0;

    std::mutex output_mutex;
    std::ostringstream output;

    std::mutex cout_mutex;

    std::string progress = "";
    int total_tests = 0;

    std::atomic<int> queued = 0;
    std::atomic<bool> start = false;

    output << "\e[38;5;220m(All Tests)\e[0m\n";

    for (auto &entry : std::filesystem::recursive_directory_iterator("bin/tests")) {
        if (entry.is_regular_file()) {
            ++total_tests;
        }
    }

    auto run_test = [&](const std::filesystem::directory_entry &entry) {
        while (!start) {}
        pid_t child;
        posix_spawn_file_actions_t actions;
        posix_spawn_file_actions_init(&actions);
        posix_spawn_file_actions_adddup2(&actions, dev_null, STDOUT_FILENO);
        posix_spawn_file_actions_adddup2(&actions, dev_null, STDERR_FILENO);

        auto path = entry.path();
        auto relative = path.lexically_relative("bin/tests");
        std::string relstr = relative.string();

        if (relstr.contains("fuzz") && !fuzz_tests) {
            ++skips;
            --queued;
            return;
        }

        std::array<char *, 3> args = { (char*) relative.c_str(), (char*) "-runs=1000", nullptr };

        // std::cout << "\e[38;5;220m" << "[TEST]" << "\e[0m" << " " << relative.string() << std::flush;

        posix_spawn(&child, path.c_str(), &actions, nullptr, args.data(), environ);

        int status;
        waitpid(child, &status, 0);
        bool success = WIFEXITED(status) && WEXITSTATUS(status) == 0;
        bool intentional_failure = WEXITSTATUS(status) == 234;

        if (success) {
            std::lock_guard<std::mutex> _(output_mutex); 
            output << "\e[2K\e[38;5;154m" << "[PASS]" << "\e[0m" << " " << relstr << std::endl;
            ++passed;
        } else if (intentional_failure) {
            std::lock_guard<std::mutex> _(output_mutex); 
            output << "\e[2K\e[38;5;220m" << "[XFAIL]" << "\e[0m" << " " << relstr << std::endl;
            ++passed;
        } else {
            std::lock_guard<std::mutex> _(output_mutex); 
            output << "\e[2K\e[38;5;196m" << "[FAIL]" << "\e[0m" << " " << relstr << std::endl;
            ++failed;
        }

        posix_spawn_file_actions_destroy(&actions);

        --queued;
    };

    for (auto &entry : std::filesystem::recursive_directory_iterator("bin/tests")) {
        if (!entry.is_regular_file()) continue;
        if (entry.path().parent_path().filename() == "integration") continue;

        ++queued;
        std::thread(run_test, entry).detach();
    }

    for (auto &entry : std::filesystem::recursive_directory_iterator("bin/tests")) {
        if (!entry.is_regular_file()) continue;
        if (entry.path().parent_path().filename() != "integration") continue;

        ++queued;
        std::thread(run_test, entry).detach();
    }

    start = true;

    while (queued != 0) {
        constexpr int fixed_width = 20;
        int filled = queued * fixed_width / total_tests;

        progress = "[" + repeat('#', filled) + repeat(' ', fixed_width - filled) + "]";
        std::cout << "\r" << progress << "\e[0m" << std::flush;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "\r\e[2K";
    std::cout << output.str();

    std::cout << "--> \e[38;5;220mResults\e[0m <--\n";
    std::cout << "\e[38;5;196m" << "Failed: " << "\e[0m" << failed << '\n';
    std::cout << "\e[38;5;154m" << "Passed: " << "\e[0m" << passed << '\n';
    if (skips > 0 || fuzz_tests) {
        std::cout << "\e[38;5;220m" << "Skipped: " << "\e[0m" << skips << '\n';
    }

    return failed;
}
#else
int main() {
    std::cout << "Run them manually ok\n";
}
#endif
