//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  Defines.h - 01/20/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _DEFINES_9bf1541fdf7efd41b7b39543fd870ac4_
#define _DEFINES_9bf1541fdf7efd41b7b39543fd870ac4_

#include <stdint.h>

// Get a pointer type can be sure.
#if defined(__x86_64__) || defined(__ia64__) || defined(_M_AMD64) || \
    defined(_M_IA64) || defined(_WIN64) || defined(__alpha__) ||     \
    defined(__s390__)
#define _X64_MODE
typedef uint64_t ptr_type;
#else
typedef uint32_t ptr_type;
#endif

#ifdef _MSC_VER
#define _COMPILER_MS
#elif __clang__
#define _COMPILER_LLVM
#elif __GNUC__
#define _COMPILER_GCC
#elif defined(__MINGW32__) || defined(__MINGW64__)
#define _COMPILER_MINGW
#endif

#if defined(_WIN32) || defined(_WIN64)
#define _OS_WINDOWS
#elif __linux__
#define _OS_LINUX
#endif

#endif