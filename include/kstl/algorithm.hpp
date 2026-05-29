#pragma once

#include "kstl/types.hpp"

namespace kstd {
    template<typename T>
    constexpr const T& max(const T &a, const T &b) noexcept {
        return (a > b) ? a : b;
    }

    template<typename T>
    constexpr const T& min(const T &a, const T &b) noexcept {
        return (a < b) ? a : b;
    }

    template<typename T, typename Compare>
    constexpr const T& max(const T &a, const T &b, Compare cmp) noexcept {
        return cmp(a, b) ? b : a;
    }

    template<typename T, typename Compare>
    constexpr const T& min(const T &a, const T &b, Compare cmp) noexcept {
        return cmp(a, b) ? a : b;
    }
    
    template<typename It, typename UnaryPred>
    bool all_of(It first, It last, UnaryPred p) {
        for (; first != last; ++first) {
            if(!p(*first)) {
                return false;
            };
        }
        return true;
    };

    template<class It, class UnaryPred>
    bool any_of(It first, It last, UnaryPred p) {
        for (; first != last; ++first) {
            if(p(*first)) {
                return true;
            };
        }
        return false;
    };

    template<class It, class UnaryPred>
    bool none_of(It first, It last, UnaryPred p) {
        for (; first != last; ++first) {
            if(p(*first)) {
                return false;
            };
        }
        return true;
    };

    template<typename It, typename T>
    It find(It begin, It end, const T &val) {
        for (; begin != end; ++begin) {
            if (*begin == val) {
                return begin;
            }
        }

        return end;
    }
    
    template<typename It, typename T>
    It find_if(It begin, It end, T comp) {
        for (; begin != end; ++begin) {
            if(comp(*begin)) return begin;
        }

        return end;
    }
    
    template<typename It, typename T>
    It find_if_not(It begin, It end, T comp) {
        for (; begin != end; ++begin) {
            if(!comp(*begin)) return begin;
        }

        return end;
    }

    template<typename It, typename T>
    It count(It begin, It end, const T &val) {
        size_t n = 0;
        for (; begin != end; ++begin) {
            if (*begin == val) {
                ++n;
            }
        }
        return n;
    }

    template<typename It, typename T>
    void fill(It begin, It end, const T &val) {
        for (; begin != end; ++begin) {
            *begin = val;
        }
    }

    template<typename InputIt, typename OutputIt>
    OutputIt copy(InputIt begin, InputIt end, OutputIt dest_begin) {
        for (; begin != end; ++begin) {
            *dest_begin = *begin;
            ++dest_begin;
        }

        return dest_begin;
    }
}
