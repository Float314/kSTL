#include <array>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>

#if defined(__linux__) || defined(__linux)
#include <unistd.h>
#include <spawn.h>
#include <sys/wait.h>
#include <fcntl.h>
extern char **environ;
int main() {
    std::cout << "\e[38;5;220m[[ kSTL Unit Tests ]]\e[0m\n";

    int dev_null = open("/dev/null", O_WRONLY);
    int passed = 0;
    int failed = 0;
    for (auto &entry : std::filesystem::recursive_directory_iterator("bin/tests")) {
        if (entry.is_regular_file()) {
            pid_t child;
            posix_spawn_file_actions_t actions;
            posix_spawn_file_actions_init(&actions);
            posix_spawn_file_actions_adddup2(&actions, dev_null, STDOUT_FILENO);
            posix_spawn_file_actions_adddup2(&actions, dev_null, STDERR_FILENO);

            std::array<char *, 2> args = { "__tester__", nullptr };

            auto path = entry.path();
            auto relative = path.lexically_relative("bin/tests");

            std::cout << ' ' << relative.string() << '\n' << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(20));

            posix_spawn(&child, path.c_str(), &actions, nullptr, args.data(), environ);

            int status;
            waitpid(child, &status, 0);

            if (status != 0) {
                std::cout << ' ' << "\e[2K\e[38;5;196m" << "Failure: " << "\e[0m" << "(exit code " << status << ")" << '\n' << std::endl;
                ++failed;
            } else {
                std::cout << ' ' << "\e[2K\e[38;5;154mSuccess\e[0m" << '\n' << std::endl;
                ++passed;
            }
        }
    }

    std::cout << "--> \e[38;5;220mResults\e[0m <--\n";
    std::cout << "\e[38;5;196m" << "Failed: " << "\e[0m" << failed << '\n';
    std::cout << "\e[38;5;154m" << "Passed: " << "\e[0m" << passed << '\n';
}
#else
int main() {
    std::cout << "Run them manually ok\n";
}
#endif
