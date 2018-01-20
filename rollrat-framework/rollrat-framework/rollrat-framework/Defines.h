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

#ifndef _DEFINES_
#define _DEFINES_

#include <stdint.h>

// Get a pointer type can be sure.
#if defined(__x86_64__) || defined(__ia64__) || defined(_M_AMD64) \
  || defined(_M_IA64) || defined(_WIN64) || defined(__alpha__) \
  || defined(__s390__)
#define _X64_MODE
typedef uint64_t	ptr_type;
#else
typedef uint32_t	ptr_type;
#endif


#endif