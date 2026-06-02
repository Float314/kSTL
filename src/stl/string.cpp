#include "kstl/string.hpp"
#include "kstl/algorithm.hpp"
#include "kstl/runtime.hpp"

namespace kstl_globals {
    extern kstd::rt_init g_init_data;
    extern void* (*malloc)(size_t);
    extern void (*free)(void*);
}

namespace kstd {
    char null_terminator = '\0';

    bool streq_2n(const char *s1, const char *s2, size_t s1_len, size_t s2_len);

    string::string() {}

    string::string(const char *data) {
        this->_size = strlen(data);
        this->_capacity = this->_size + 1;
        this->_data = (char*) kstl_globals::malloc(this->_capacity);
        strncpy(this->_data, data, this->_capacity);
        this->_data[this->_capacity - 1] = '\0';
    }

    string::string(size_t n, char c) {
        this->append(n, c);
    }

    size_t string::size() const noexcept {
        return this->_size;
    }

    size_t string::capacity() const noexcept {
        return this->_capacity;
    }

    void string::null_check() noexcept {
        if (this->_data == nullptr) {
            this->_capacity = 1;
            this->_size = 0;
            this->_data = reinterpret_cast<char*>(kstl_globals::malloc(this->_capacity));
        }
    } 

    void string::append(size_t n, char c) noexcept {
        if (n == 0) return;
        if (this->_capacity < this->_size + n) {
            this->reserve(this->_size + n);
        }
        while (n-- != 0) this->append(c);
    }

    void string::append(char c) noexcept {
        this->null_check();

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
            this->null_check();

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

    void string::reserve(size_t elements) noexcept {
        this->null_check();

        if (elements == this->_size) return;
        if (elements < this->_size) {
            this->_capacity = elements;

            return;
        }

        size_t old_capacity = this->_capacity;
        this->_capacity = elements;

        char *new_data = reinterpret_cast<char*>(kstl_globals::malloc(this->_capacity));

        kstd::memcpy(new_data, this->_data, old_capacity);
        kstd::memset(new_data + this->_size, 0, elements - old_capacity);
        kstl_globals::free(this->_data);

        this->_data = new_data;
    }

    void string::resize(size_t elements) noexcept {
        this->null_check();

        if (elements == this->_size) return;
        if (elements < this->_size) {
            this->_data[elements] = '\0';
            this->_size = elements;

            return;
        }

        size_t old_size = this->_size;
        this->_capacity = elements + 1;
        this->_size = elements;

        char *new_data = reinterpret_cast<char*>(kstl_globals::malloc(this->_capacity));

        kstd::memcpy(new_data, this->_data, old_size);
        kstd::memset(new_data + this->_size, 0, elements - old_size);
        kstl_globals::free(this->_data);

        this->_data = new_data;
    }

    void string::shrink_to_fit() noexcept {
        this->null_check();

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
    
    bool string::has_value() const noexcept {
        return this->_data != nullptr && this->_size > 0;
    }

    bool string::empty() const noexcept {
        return this->_size == 0;
    }

    char& string::at(size_t index) noexcept {
        if (index >= this->_size) {
            kstl_globals::g_init_data.panic();
        }

        return this->_data[index];
    }

    const char& string::at(size_t index) const noexcept {
        if (index >= this->_size) {
            kstl_globals::g_init_data.panic();
        }

        return this->_data[index];
    }

    char& string::front() noexcept {
        this->null_check();
        if (0 >= this->_size) {
            kstl_globals::g_init_data.panic();
        }

        return this->_data[0];
    }

    const char& string::front() const noexcept {
        if (0 >= this->_size) {
            kstl_globals::g_init_data.panic();
        }

        return this->_data[0];
    }

    char& string::back() noexcept {
        this->null_check();
        if (this->_size - 1 < 0) {
            kstl_globals::g_init_data.panic();
        }

        return this->_data[this->_size - 1];
    }

    const char& string::back() const noexcept {
        if (this->_size - 1 < 0) {
            kstl_globals::g_init_data.panic();
        }

        return this->_data[this->_size - 1];
    }

    bool string::starts_with(char prefix) const noexcept {
        if (this->_data == nullptr || this->_size == 0) {
            return false;
        }

        return this->_data[0] == prefix;
    }
    
    bool string::starts_with(const char *prefix) const noexcept {
        size_t len = strlen(prefix);
        if (this->_data == nullptr || this->_size < len) {
            return false;
        }

        for (size_t i = 0; i < len; ++i) {
            if (prefix[i] != this->_data[i]) {
                return false;
            }
        }

        return true;
    }

    bool string::starts_with(const string &prefix) const noexcept {
        size_t len = prefix._size;
        if (this->_data == nullptr || this->_size < len) {
            return false;
        }

        for (size_t i = 0; i < len; ++i) {
            if (prefix[i] != this->_data[i]) {
                return false;
            }
        }

        return true;
    }

    bool string::ends_with(char suffix) const noexcept {
        if (this->_data == nullptr || this->_size == 0) {
            return false;
        }

        return this->_data[this->_size - 1] == suffix;
    }

    bool string::ends_with(const char *suffix) const noexcept {
        size_t len = strlen(suffix);
        if (this->_data == nullptr || this->_size < len) {
            return false;
        }

        for (size_t _i = len; _i != 0; --_i) {
            size_t i = _i - 1;

            if (suffix[i] != this->_data[i]) {
                return false;
            }
        }

        return true;
    }

    bool string::ends_with(const string &suffix) const noexcept {
        size_t len = suffix._size;
        if (this->_data == nullptr || this->_size < len) {
            return false;
        }

        for (size_t _i = len; _i != 0; --_i) {
            size_t i = _i - 1;

            if (suffix[i] != this->_data[i]) {
                return false;
            }
        }

        return true;
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

    string::const_iterator string::cbegin() const noexcept {
        return const_iterator(this->_data);
    }

    string::const_iterator string::cend() const noexcept {
        return const_iterator(this->_data + this->_size);
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

    string& string::operator+=(char c) noexcept {
        this->append(c);
        return *this;
    }

    string string::operator+(const string &s) const noexcept {
        string res;
        res.reserve(this->_size + s._size);
        res.append(this->_data);
        res.append(s._data);
        return res;
    }

    char& string::operator[](size_t index) noexcept {
        return this->_data[index];
    }

    const char& string::operator[](size_t index) const noexcept {
        return this->_data[index];
    }

    bool string::operator==(const string &other) const noexcept {
        if (this == &other) return true;
        return streq_2n(this->_data, other._data, this->_size, other._size);
    }

    bool string::operator==(const char *s) const noexcept {
        return streq_2n(this->_data, s, this->_size, strlen(s));
    }

    bool string::operator!=(const string &other) const noexcept {
        return !(*this == other);
    }

    bool string::operator!=(const char *s) const noexcept {
        return !(*this == s);
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

    bool operator==(const char *lhs, const string &rhs) {
        return rhs == lhs;
    }

    bool operator!=(const char *lhs, const string &rhs) {
        return rhs != lhs;
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

    bool streq(const char *s1, const char *s2) {
        size_t s1_len = strlen(s1);
        size_t s2_len = strlen(s2);

        if (s1_len != s2_len) {
            return false;
        }

        // s1 && s2 are same length
        for (size_t i = 0; i < s1_len; ++i) {
            if (s1[i] != s2[i]) {
                return false;
            }
        }

        return true;
    }

    bool streq_2n(const char *s1, const char *s2, size_t s1_len, size_t s2_len) {
        if (s1_len != s2_len) {
            return false;
        }

        // s1 && s2 are same length
        for (size_t i = 0; i < s1_len; ++i) {
            if (s1[i] != s2[i]) {
                return false;
            }
        }

        return true;
    }

    void memcpy(void *__restrict dst, const void *__restrict src, size_t bytes) {
        __builtin_memcpy(dst, src, bytes);
    }

    void memset(void *s, int c, size_t n) {
        __builtin_memset(s, c, n);
    }

    kstd::string to_string(int v) {
        kstd::string result;

        while (v > 9) {
            result += static_cast<char>((v % 10) + '0');
            v /= 10;
        }

        result += static_cast<char>(v + '0');
        kstd::reverse(result.begin(), result.end());

        return result;
    }
}
