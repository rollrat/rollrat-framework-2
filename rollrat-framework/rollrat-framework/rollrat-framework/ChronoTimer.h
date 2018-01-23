//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  ChronoTimer.h - 01/23/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _CHRONOTIMER_
#define _CHRONOTIMER_

#include <chrono>

namespace ofw
{

class ChronoTimer
{
  // OS dependent type.
  using timer_type = std::chrono::high_resolution_clock;

  timer_type::time_point begin;
  timer_type::time_point end;
  std::chrono::duration<long double> gap;

public:

  void start() { begin = timer_type::now(); }
  void finish() { end = timer_type::now(); gap = end - begin; }
  long double operator*() { return gap.count(); }
};

}

#endif