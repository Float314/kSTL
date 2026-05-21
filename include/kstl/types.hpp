#pragma once

// Fixed-width integer types
typedef signed char        int8_t;
typedef unsigned char      uint8_t;

typedef signed short       int16_t;
typedef unsigned short     uint16_t;

typedef signed int         int32_t;
typedef unsigned int       uint32_t;

typedef signed long int64_t;
typedef unsigned long uint64_t;

typedef unsigned long size_t;

// Pointer-sized integer types
#if defined(__x86_64__) || defined(_M_X64)
typedef int64_t  intptr_t;
typedef uint64_t uintptr_t;
#else
typedef int32_t  intptr_t;
typedef uint32_t uintptr_t;
#endif

// Limits (optional but useful)
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
