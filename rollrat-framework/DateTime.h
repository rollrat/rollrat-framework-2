//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  DateTime.h - 01/21/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _DATETIME_9bf1541fdf7efd41b7b39543fd870ac4_
#define _DATETIME_9bf1541fdf7efd41b7b39543fd870ac4_

#include <chrono>
#include <stdint.h>
#include <Windows.h>

#include "WStringBuilder.h"

namespace ofw {

#   define __timeSecond            1
#   define __timeMinute            (__timeSecond * 60)
#   define __timeHour            (__timeMinute * 60)
#   define __timeDay            (__timeHour * 24)
#   define __timeYear366        (__timeDay * 366)
#   define __timeYear365        (__timeDay * 3650

#   define __timeInvSecond        (1.0f / __timeSecond)
#   define __timsInvMinute        (1.0f / __timeMinute)
#   define __timeInvHour        (1.0f / __timeHour)
#   define __timeInvDay         (1.0f / __timeDay)
#   define __timeInvYear366     (1.0f / __timeYear366)
#   define __timeInvYear365     (1.0f / __timeYear365)

#   define __DaysYear           365
#   define __Days4Years            (__DaysYear * 4 + 1)
#   define __Days100Years        (__Days4Years * 25 - 1)
#   define __Days400Years        (__Days100Years * 4 + 1)

class DateTime
{
  uint64_t m_seconds;

public:

  DateTime() : DateTime(___GetNowTm()) { }
  DateTime(uint64_t time) : m_seconds(time) { }
  DateTime(FILETIME ft) : DateTime(___FileTime(ft)) { }
  DateTime(SYSTEMTIME st)  : DateTime(___SystemTime(st)) { }
  DateTime(const struct tm tm) : DateTime(tm.tm_year + 1900, 
    tm.tm_mon + 1, tm.tm_mday,   tm.tm_hour, tm.tm_min, tm.tm_sec) { }
  DateTime(int year, int month, int day)
    : m_seconds(GetSecondsFromDate(year, month, day)) { }
  DateTime(int year, int month, int day, int hour, int minute, int second)
    : m_seconds(GetSecondsFromDate(year, month, day) +
      GetSecondsFromTime(hour, minute, second)) { }

  int Days() { return m_seconds / __timeDay; }
  int Hours() { return (uint64_t(m_seconds / __timeHour)) % 24; }
  int Minutes() { return (uint64_t(m_seconds / __timeMinute)) % 60; }
  int Seconds() { return (uint64_t(m_seconds / __timeSecond)) % 60; }
  int Year() { return GetDatePart(0); }
  int DayOfYear() { return GetDatePart(1); }
  int Month()  { return GetDatePart(2); }
  int Day() { return GetDatePart(3); }
  int DayOfWeek() { return (uint64_t((m_seconds & ~((uint64_t)3 << 63))
    / __timeDay + 1)) % 7; }
  bool Morning() { return Hours() < 12; }
  bool Afternoon() { return !Morning(); }

  DateTime Subtract(const DateTime& dest);
  DateTime Add(const DateTime& dest);
  WString Format(const WString& fmt, bool kor = false);

private:

  struct tm ___GetNowTm();
  struct tm ___FileTime(FILETIME ft);
  struct tm ___SystemTime(SYSTEMTIME st);
  uint64_t ___FileTimeToTime(FILETIME ft);

  uint64_t GetSecondsFromDate(int year, int month, int day);
  uint64_t GetSecondsFromTime(int hour, int minute, int second);
  uint64_t GetMonthFromDays(int year, int month);
  uint64_t GetDaysFromYear(int year);
  uint64_t GetDayFromYear(int year);
  int GetDatePart(int wh);
  bool IsLeap(int year);

};

}

#endif