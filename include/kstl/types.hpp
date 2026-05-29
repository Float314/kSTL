#pragma once

// C-Compatible Header File

// Fixed-width integer types
typedef signed char        int8_t;
typedef unsigned char      uint8_t;

typedef signed short       int16_t;
typedef unsigned short     uint16_t;

typedef signed int         int32_t;
typedef unsigned int       uint32_t;

typedef __INT64_TYPE__     int64_t;
typedef __UINT64_TYPE__    uint64_t;

typedef __SIZE_TYPE__ size_t;

// Pointer-sized integer types
#if defined(__x86_64__) || defined(_M_X64)
typedef int64_t  intptr_t;
typedef uint64_t uintptr_t;
#else
typedef int32_t  intptr_t;
typedef uint32_t uintptr_t;
#endif

// Limits
#ifndef INT16_MIN
#define INT16_MIN  (-32768)
#endif
#ifndef INT16_MAX
#define INT16_MAX  32767
#endif
#ifndef UINT16_MAX
#define UINT16_MAX 65535
#endif

#ifndef INT32_MIN
#define INT32_MIN  (-2147483647 - 1)
#endif
#ifndef INT32_MAX
#define INT32_MAX  2147483647
#endif
#ifndef UINT32_MAX
#define UINT32_MAX 4294967295U
#endif

#ifndef INT64_MIN
#define INT64_MIN  (-9223372036854775807LL - 1)
#endif
#ifndef INT64_MAX
#define INT64_MAX  9223372036854775807LL
#endif
#ifndef UINT64_MAX
#define UINT64_MAX 18446744073709551615ULL
#endif

#ifdef __cplusplus
namespace kstd {
    using uint8_t   = ::uint8_t;
    using uint16_t  = ::uint16_t;
    using uint32_t  = ::uint32_t;
    using uint64_t  = ::uint64_t;
    using int8_t    = ::int8_t;
    using int16_t   = ::int16_t;
    using int32_t   = ::int32_t;
    using int64_t   = ::int64_t;

    using size_t    = ::size_t;
}
#endif
