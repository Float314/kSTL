#include <array>
#include <chrono>
#include <filesystem>
#include <iostream>
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
    std::string s;
    s.reserve(n);
    for (int i = 0; i < n; ++i) {
        s.push_back(c);
    }
    return s;
}

#define KSTL_PRETTY_TEST_RUNNER

int main() {
    std::cout << "\e[38;5;220m[[ kSTL Tester ]]\e[0m\n";

    int dev_null = open("/dev/null", O_WRONLY);
    int passed = 0;
    int failed = 0;
    std::ostringstream output;
    std::string progress = "";
    int total_tests = 0;


    output << "\e[38;5;220m(Unit Tests)\e[0m\n";

    for (auto &entry : std::filesystem::recursive_directory_iterator("bin/tests")) {
        if (entry.is_regular_file()) {
            ++total_tests;
        }
    }

    auto run_test = [&](const std::filesystem::directory_entry &entry) {
        pid_t child;
        posix_spawn_file_actions_t actions;
        posix_spawn_file_actions_init(&actions);
        posix_spawn_file_actions_adddup2(&actions, dev_null, STDOUT_FILENO);
        posix_spawn_file_actions_adddup2(&actions, dev_null, STDERR_FILENO);

        auto path = entry.path();
        auto relative = path.lexically_relative("bin/tests");

        std::array<char *, 2> args = { (char*) relative.c_str(), nullptr };

        // std::cout << "\e[38;5;220m" << "[TEST]" << "\e[0m" << " " << relative.string() << std::flush;
#ifdef KSTL_PRETTY_TEST_RUNNER
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
#endif

        posix_spawn(&child, path.c_str(), &actions, nullptr, args.data(), environ);

        int status;
        waitpid(child, &status, 0);
        bool success = WIFEXITED(status) && WEXITSTATUS(status) == 0;

        if (!success) {
            output << "\e[2K\e[38;5;196m" << "[FAIL]" << "\e[0m" << " " << relative.string() << std::endl;
            ++failed;
        } else {
            output << "\e[2K\e[38;5;154m" << "[PASS]" << "\e[0m" << " " << relative.string() << std::endl;
            ++passed;
        }

        constexpr int fixed_width = 20;
        int done = passed + failed;
        int filled = done * fixed_width / total_tests;

        progress = "[" + repeat('#', filled) + repeat(' ', fixed_width - filled) + "]";
        std::cout << "\r" << progress << "\e[0m" << std::flush;

        posix_spawn_file_actions_destroy(&actions);
    };

    for (auto &entry : std::filesystem::recursive_directory_iterator("bin/tests")) {
        if (!entry.is_regular_file()) continue;
        if (entry.path().parent_path().filename() == "integration") continue;

        run_test(entry);
    }

    output << "\e[38;5;220m(Integration Tests)\e[0m\n";

    for (auto &entry : std::filesystem::recursive_directory_iterator("bin/tests")) {
        if (!entry.is_regular_file()) continue;
        if (entry.path().parent_path().filename() != "integration") continue;

        run_test(entry);
    }

#ifdef KSTL_PRETTY_TEST_RUNNER
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
#endif

    std::cout << "\r\e[2K";
    std::cout << output.str();

    std::cout << "--> \e[38;5;220mResults\e[0m <--\n";
    std::cout << "\e[38;5;196m" << "Failed: " << "\e[0m" << failed << '\n';
    std::cout << "\e[38;5;154m" << "Passed: " << "\e[0m" << passed << '\n';

    return failed;
}
#else
int main() {
    std::cout << "Run them manually ok\n";
}
#endif
