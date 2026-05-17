#include "kstl/string.hpp"
#include "kstl/runtime.hpp"
#include "kstl/stdlib.hpp"

namespace kstd {
    char null_terminator = '\0';

    string::string() {
        this->_capacity = 0;
        this->_size = 0;
    }

    string::string(const char *data) {
        this->_size = strlen(data);
        this->_capacity = this->_size + 1;
        this->_data = new char[this->_capacity];
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
            this->_data = new char[2];
            this->_data[0] = c;
            this->_data[1] = '\0';
            return;
        }

        if (this->_size + 2 > this->_capacity) {
            size_t old_capacity = this->_capacity;
            this->_capacity *= 2;
            char *new_data = new char[this->_capacity];
            kstd::memcpy(new_data, this->_data, old_capacity);
            delete[] this->_data;
            this->_data = new_data;
        }

        this->_data[this->_size++] = c;
        this->_data[this->_size] = '\0';
    }

    void string::append(char *str) noexcept {
        size_t len = strlen(str);
        for (size_t i = 0; i < len; ++i) {
            this->append(str[i]);
        }
    }

    void string::append(const string &str) noexcept {
        size_t len = str._size;
        for (size_t i = 0; i < len; ++i) {
            this->append(str._data[i]);
        }
    }

    void string::reserve(size_t bytes) noexcept {
        if (bytes < this->_capacity) {
            return;
        }

        size_t old_capacity = this->_capacity;
        this->_capacity = bytes;
        char *new_data = new char[this->_capacity];
        kstd::memcpy(new_data, this->_data, old_capacity);
        delete[] this->_data;
        this->_data = new_data;
    }

    void string::shrink_to_fit() noexcept {
        if (this->_capacity > this->_size) {
            this->_capacity = this->_size + 1;
            char *new_data = new char[this->_capacity];
            kstd::memcpy(new_data, this->_data, this->_capacity);
            delete[] this->_data;
            this->_data = new_data;
        }
    }

    char* string::data() const noexcept {
        if (this->_capacity == 0) {
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

    string& string::operator=(const char* s) {
        if (this->_data != nullptr) {
            delete[] this->_data;
        }

        this->_size = strlen(s);
        this->_capacity = this->_size + 1;
        this->_data = new char[this->_capacity];
        strncpy(this->_data, s, this->_capacity - 1);
        this->_data[this->_capacity - 1] = '\0';
        return *this;
    }

    string& string::operator=(const string &s) {
        if (this == &s) return *this;

        if (this->_data != nullptr) {
            delete[] this->_data;
        }

        this->_size = s._size;
        this->_capacity = s._capacity;
        this->_data = new char[this->_capacity];
        kstd::memcpy(this->_data, s._data, s._capacity);
        return *this;
    }

    string& string::operator+=(const char* s) {
        this->append(s);
        return *this;
    }

    string& string::operator+=(const string &s) {
        this->append(s);
        return *this;
    }

    string::~string() {
        if (this->_data != nullptr) {
            delete[] this->_data;
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
