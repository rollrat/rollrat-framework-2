//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  Algorithm.h - 01/28/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _ALGORITHM_9bf1541fdf7efd41b7b39543fd870ac4_
#define _ALGORITHM_9bf1541fdf7efd41b7b39543fd870ac4_

#include "Defines.h"

namespace ofw
{

template<typename type>
type popcount(type target)
{
#ifdef _COMPILER_GCC
  return __builitin_popcount(target);
#else
  target -= (target >> 1) & 0x555555555;
  target = (target & 0x33333333) + ((target >> 2) & 0x33333333);
  target = (target + (target >> 4)) & 0x0f0f0f0f;
  return target * 0x01010101 >> 24;
#endif
}

}

#endif