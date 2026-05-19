#include "kstl/string.hpp"
#include "kstl/algorithm.hpp"
#include "kstl/runtime.hpp"
#include "kstl/stdlib.hpp"

namespace kstl_globals {
    extern kstd::rt_init g_init_data;
    extern void* (*malloc)(size_t);
    extern void (*free)(void*);
}

namespace kstd {
    char null_terminator = '\0';

    string::string() {
    }

    string::string(const char *data) {
        this->_size = strlen(data);
        this->_capacity = this->_size + 1;
        this->_data = (char*) kstl_globals::malloc(this->_capacity);
        strncpy(this->_data, data, this->_capacity);
        this->_data[this->_capacity - 1] = '\0';
    }

    size_t string::size() const noexcept {
        return this->_size;
    }

    size_t string::capacity() const noexcept {
        return this->_capacity;
    }

    void string::append(char c) noexcept {
        if (this->_data == nullptr) {
            this->_capacity = 2;
            this->_size = 1;
            this->_data = (char*) kstl_globals::malloc(2);
            this->_data[0] = c;
            this->_data[1] = '\0';
            return;
        }

        if (this->_size + 2 > this->_capacity) {
            size_t old_capacity = this->_capacity;
            this->_capacity *= 2;
            char *new_data = (char*) kstl_globals::malloc(this->_capacity);
            kstd::memcpy(new_data, this->_data, old_capacity);
            kstl_globals::free(this->_data);
            this->_data = new_data;
        }

        this->_data[this->_size++] = c;
        this->_data[this->_size] = '\0';
    }

    void string::append(char *str) noexcept {
        size_t len = strlen(str);
        for (size_t i = 0; i < len; ++i) {
            char c = str[i];
            if (this->_data == nullptr) {
                this->_capacity = 2;
                this->_size = 1;
                this->_data = (char*) kstl_globals::malloc(2);
                this->_data[0] = c;
                this->_data[1] = '\0';
                return;
            }

            if (this->_size + 2 > this->_capacity) {
                size_t old_capacity = this->_capacity;
                this->_capacity *= 2;
                char *new_data = (char*) kstl_globals::malloc(this->_capacity);
                kstd::memcpy(new_data, this->_data, old_capacity);
                kstl_globals::free(this->_data);
                this->_data = new_data;
            }

            this->_data[this->_size++] = c;
            this->_data[this->_size] = '\0';
        }
    }

    void string::append(const string &str) noexcept {
        size_t len = str._size;
        for (size_t i = 0; i < len; ++i) {
            this->append(str._data[i]);
        }
    }

    void string::push_back(char c) noexcept {
        this->append(c);
    }

    void string::pop_back() noexcept {
        if (this->_size == 0 || this->_data == nullptr) {
            kstl_globals::g_init_data.panic();
        }

        this->_data[this->_size - 1] = '\0';
        this->_size--;
    }

    void string::reserve(size_t bytes) noexcept {
        if (bytes <= this->_capacity) {
            return;
        }

        size_t old_capacity = this->_capacity;
        this->_capacity = kstd::max(bytes, this->_capacity * 2);
        char *new_data = (char*) kstl_globals::malloc(this->_capacity);
        kstd::memcpy(new_data, this->_data, old_capacity);
        kstl_globals::free(this->_data);
        this->_data = new_data;
    }

    void string::shrink_to_fit() noexcept {
        if (this->_capacity > this->_size + 1) {
            this->_capacity = this->_size + 1;
            char *new_data = (char*) kstl_globals::malloc(this->_capacity);
            kstd::memcpy(new_data, this->_data, this->_capacity);
            kstl_globals::free(this->_data);
            this->_data = new_data;
        }
    }

    char* string::data() const noexcept {
        if (this->_capacity == 0) {
            // pwease don't modify it :3
            return &null_terminator;
        }
        return this->_data;
    }

    const char* string::c_str() const noexcept {
        if (this->_capacity == 0) {
            return "";
        }
        return this->_data;
    }

    string::iterator string::begin() noexcept {
        return this->_data;
    }

    string::iterator string::end() noexcept {
        return this->_data + this->_size;
    }

    string::const_iterator string::begin() const noexcept {
        return this->_data;
    }

    string::const_iterator string::end() const noexcept {
        return this->_data + this->_size;
    }

    string& string::operator=(const char* s) noexcept {
        if (this->_data != nullptr) {
            kstl_globals::free(this->_data);
        }

        this->_size = strlen(s);
        this->_capacity = this->_size + 1;
        this->_data = (char*) kstl_globals::malloc(this->_capacity);
        strncpy(this->_data, s, this->_capacity - 1);
        this->_data[this->_capacity - 1] = '\0';
        return *this;
    }

    string& string::operator=(const string &s) noexcept {
        if (this == &s) return *this;

        if (this->_data != nullptr) {
            kstl_globals::free(this->_data);
        }

        this->_size = s._size;
        this->_capacity = s._capacity;
        this->_data = (char*) kstl_globals::malloc(this->_capacity);
        kstd::memcpy(this->_data, s._data, s._capacity);
        return *this;
    }

    string& string::operator=(string &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        kstl_globals::free(this->_data);

        this->_data = other._data;
        this->_size = other._size;
        this->_capacity = other._capacity;

        other._data = nullptr;
        other._size = 0;
        other._capacity = 0;

        return *this;
    }

    string& string::operator+=(const char* s) noexcept {
        this->append(s);
        return *this;
    }

    string& string::operator+=(const string &s) noexcept {
        this->append(s);
        return *this;
    }

    string string::operator+(const string &s) const noexcept {
        string res;
        res.reserve(this->_size + s._size);
        res.append(this->_data);
        res.append(s._data);
        return res;
    }

    string::string(const string &s) {
        this->_size = s._size;
        this->_capacity = s._capacity;
        this->_data = (char*) kstl_globals::malloc(this->_capacity);
        kstd::memcpy(this->_data, s._data, s._capacity);
    }

    string::string(string &&other) {
        this->_data = other._data;
        this->_size = other._size;
        this->_capacity = other._capacity;

        other._data = nullptr;
        other._size = 0;
        other._capacity = 0;
    }

    string::~string() {
        if (this->_data != nullptr) {
            kstl_globals::free(this->_data);
        }
    }

    size_t strlen(const char *s) {
        size_t length = 0;
        while (*s) {
            s++;
            length++;
        }

        return length;
    }

    char* strcpy(char *__restrict dst, char *__restrict src) {
        char *original_dst = dst;

        while (*src) {
            *dst = *src;
            dst++;
            src++;
        }

        *dst = '\0';

        return original_dst;
    }

    char* strncpy(char *__restrict dst, const char *__restrict src, size_t dest_size) {
        size_t chars_copied = 0;
        char *original_dst = dst;
        while (*src && dest_size > chars_copied) {
            *dst = *src;
            dst++;
            src++;
            chars_copied++;
        }

        *dst = '\0';

        return original_dst;
    }
}
