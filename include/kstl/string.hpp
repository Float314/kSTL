#pragma once

#include "kstl/types.hpp"

namespace kstd {
    class string {
    public:
        using iterator = char *;
        using const_iterator = const char *;

        static constexpr size_t npos = size_t(-1);
    private:
        char *_data = nullptr;
        size_t _size = 0;
        size_t _capacity = 0;
    public:
        size_t size() const noexcept;
        size_t capacity() const noexcept;
      
        bool has_value() const noexcept; 
        bool empty() const noexcept;

        char& at(size_t index) noexcept;
        const char& at(size_t index) const noexcept;
    public:
        void append(char c) noexcept;
        void append(char *str) noexcept;
        void append(const string &str) noexcept;

        void push_back(char c) noexcept;
        void pop_back() noexcept;

        void reserve(size_t bytes) noexcept;
        void shrink_to_fit() noexcept;
    public:
        char* data() const noexcept;
        const char* c_str() const noexcept;
    public:
        string substr(size_t pos, size_t count = npos) const noexcept;

        bool starts_with(char prefix) const noexcept;
        bool starts_with(const char *prefix) const noexcept;
        bool starts_with(const string &prefix) const noexcept;

        bool ends_with(char suffix) const noexcept;
        bool ends_with(const char *suffix) const noexcept;
        bool ends_with(const string &suffix) const noexcept;
    public:
        iterator begin() noexcept;
        iterator end() noexcept;

        const_iterator begin() const noexcept;
        const_iterator end() const noexcept;
    public:
        string& operator=(const char*) noexcept;
        string& operator=(const string &s) noexcept;
        string& operator+=(const char*) noexcept;
        string& operator+=(const string &s) noexcept;

        string& operator=(string &&other) noexcept;

        string operator+(const string &s) const noexcept;

        char& operator[](size_t index) noexcept;
        const char& operator[](size_t index) const noexcept;

        bool operator==(const string &other) const noexcept;
        bool operator==(const char *s) const noexcept;

        bool operator!=(const string &other) const noexcept;
        bool operator!=(const char *s) const noexcept;
    public:
        string();
        string(const char *data);

        string(const string &s);
        string(string &&other);
    public:
        ~string();
    };

    size_t strlen(const char *s);
    char* strcpy(char *__restrict dst, char *__restrict src);
    char* strncpy(char *__restrict dst, const char *__restrict src, size_t dest_size);
    bool streq(const char *s1, const char *s2);

    // TODO: Implement these two
    int stoi(const string &s);
    string to_string(int v);
}
