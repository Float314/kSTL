#pragma once

#include "kstl/types.hpp"
#include "kstl/memory.hpp"

namespace kstd {
    /// @brief A heap-allocated string
    /// @note data is not mutable if capacity() == 0
    class string {
    public:
        using iterator = char *;
        using const_iterator = const char *;

        static constexpr size_t npos = size_t(-1);
    public:
        static allocator<char> alloc;
    private:
        char *_data = nullptr;
        size_t _size = 0;
        size_t _capacity = 0;
    private:
        void null_check() noexcept;
    public:
        size_t size() const noexcept;
        size_t capacity() const noexcept;
      
        bool has_value() const noexcept; 
        bool empty() const noexcept;

        char& at(size_t index) noexcept;
        const char& at(size_t index) const noexcept;

        char& front() noexcept;
        const char& front() const noexcept;

        char& back() noexcept;
        const char& back() const noexcept;
    public:
        void append(size_t count, char c) noexcept;
        void append(char c) noexcept;
        void append(char *str) noexcept;
        void append(const string &str) noexcept;

        void push_back(char c) noexcept;
        void pop_back() noexcept;

        void reserve(size_t bytes) noexcept;
        void resize(size_t bytes) noexcept;
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

        bool contains(char c) const noexcept;
        bool contains(const char *substr) const noexcept;
    public:
        iterator begin() noexcept;
        iterator end() noexcept;

        const_iterator begin() const noexcept;
        const_iterator end() const noexcept;

        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;
    public:
        string& operator=(const char*) noexcept;
        string& operator=(const string &s) noexcept;
        string& operator+=(const char*) noexcept;
        string& operator+=(const string &s) noexcept;
        string& operator+=(char c) noexcept;

        string& operator=(string &&other) noexcept;

        string operator+(const string &s) const noexcept;

        char& operator[](size_t index) noexcept;
        const char& operator[](size_t index) const noexcept;
    public:
        string();
        string(const char *data);
        string(const char *data, size_t length);
        string(size_t n, char c);

        string(const string &s);
        string(string &&other);
    public:
        ~string();
    };

    bool operator==(const string &lhs, const string &rhs) noexcept;
    bool operator==(const string &lhs, const char *rhs) noexcept;
    bool operator==(const char *lhs, const string &rhs) noexcept;

    bool operator!=(const string &lhs, const string &rhs) noexcept;
    bool operator!=(const string &lhs, const char *rhs) noexcept;
    bool operator!=(const char *lhs, const string &rhs) noexcept;

    size_t strlen(const char *s);
    char* strcpy(char *__restrict dst, char *__restrict src);
    char* strncpy(char *__restrict dst, const char *__restrict src, size_t dest_size);
    bool streq(const char *s1, const char *s2);

    // TODO: Implement these two
    int stoi(const string &s);
    string to_string(int v);

    void memcpy(void *__restrict dst, const void *__restrict src, size_t bytes);
    void memset(void *s, int c, size_t n);
}
