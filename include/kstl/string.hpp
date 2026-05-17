#pragma once

#include "kstl/types.hpp"

namespace kstd {
    class string {
    private:
        char *_data = nullptr;
        size_t _size = 0;
        size_t _capacity = 0;
    public:
        size_t size() const noexcept;
        size_t capacity() const noexcept;
    public:
        void append(char c) noexcept;
        void append(char *str) noexcept;
        void append(const string &str) noexcept;

        void reserve(size_t bytes) noexcept;
        void shrink_to_fit() noexcept;
    public:
        char* data() noexcept;
        const char* c_string() noexcept;
    public:
        string();
        string(const char *data);
    public:
        ~string();
    };

    size_t strlen(const char *s);
    char* strcpy(char *__restrict dst, char *__restrict src);
    char* strncpy(char *__restrict dst, const char *__restrict src, size_t dest_size);
}
