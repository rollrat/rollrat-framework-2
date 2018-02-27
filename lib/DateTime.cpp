//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  DateTime.cpp - 01/21/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#include "rollrat-framework/DateTime.h"

using namespace ofw;

static const int _date_366[] = {0,   31,  60,  91,  121, 152, 182,
                                213, 244, 274, 305, 335, 366};
static const int _date_365[] = {0,   31,  59,  90,  120, 151, 181,
                                212, 243, 273, 304, 334, 365};

static const wchar_t *_month_eng[] = {L"January", L"February", L"March",
                                      L"April",   L"May",      L"June",
                                      L"July",    L"August",   L"September",
                                      L"October", L"November", L"December"};

static const wchar_t *_week_eng[] = {L"Sunday",    L"Monday",   L"Tuesday",
                                     L"Wednesday", L"Thursday", L"Friday",
                                     L"Saturday"};
static const wchar_t *_week_kor[] = {L"일", L"월", L"화", L"수",
                                     L"목", L"금", L"토"};
static const wchar_t *_week_kor_post[] = {L"일요일", L"월요일", L"화요일",
                                          L"수요일", L"목요일", L"금요일",
                                          L"토요일"};

const int *days366 = _date_366;
const int *days365 = _date_365;

const wchar_t **monthsEng = _month_eng;

const wchar_t **weeksEng = _week_eng;
const wchar_t **weeksKorFull = _week_kor_post;
const wchar_t **weeksKorShort = _week_kor;

const wchar_t *meridiemEngFirst = L"A.M.";
const wchar_t *meridiemEngLast = L"P.M.";
const wchar_t *meridiemEngFirstShort = L"AM";
const wchar_t *meridiemEngLastShort = L"PM";

const wchar_t *meridiemKorFirst = L"오전";
const wchar_t *meridiemKorLast = L"오후";

DateTime DateTime::Subtract(const DateTime &dest) {
  if (dest.m_seconds > this->m_seconds)
    return DateTime(dest.m_seconds - this->m_seconds - 1);
  else
    return DateTime(this->m_seconds - dest.m_seconds - 1);
}
DateTime DateTime::Add(const DateTime &dest) {
  return DateTime(this->m_seconds + dest.m_seconds);
}

WString DateTime::Format(const WString &fmt, bool kor) {
  WStringBuilder wsb(fmt.Length() << 2);
  WString ref;
  const wchar_t *ptr = fmt.Reference();

  int wh;

  auto countRepeat = [](const wchar_t *t, wchar_t ch) {
    const wchar_t *ptr = t;
    for (; *ptr == ch; ptr++)
      ;
    return ptr - t;
  };

  // stftime이랑 거의 연동된다고 보면됨
  for (; *ptr;) {
    wh = countRepeat(ptr, *ptr);
    switch (*ptr) {
      case L'y':  // year(대문자는 없음)
        if (wh <= 2)
          wsb.Append(WString(Year()).Substring(2));
        else if (wh >= 4)
          wsb.Append(Year());
        break;
      case L'M':  // month
        if (wh == 1)
          wsb.Append(Month() % 10);
        else if (wh == 2)
          wsb.Append(WString(Month()).PadLeft(2, L'0'));
        else if (wh == 3)
          wsb.Append(kor ? WString::Concat(Month(), L"월")
                         : WString(monthsEng[Month() - 1]).Substring(0, 3));
        else if (wh >= 4)
          wsb.Append(kor ? WString::Concat(Month(), L"월")
                         : monthsEng[Month() - 1]);
        break;
      case L'd':  // day
        if (wh == 1)
          wsb.Append(Day() % 10);
        else if (wh == 2)
          wsb.Append(WString(Day()).PadLeft(2, L'0'));
        else if (wh == 3)
          wsb.Append(kor ? weeksKorShort[DayOfWeek()]
                         : WString(weeksEng[DayOfWeek()]).Substring(0, 3));
        else if (wh >= 4)
          wsb.Append(kor ? weeksKorFull[DayOfWeek()] : weeksEng[DayOfWeek()]);
        break;
      case L'T':  // AM/PM
        if (wh == 1)
          wsb.Append(Morning() ? L'A' : L'P');
        else if (wh >= 2)
          wsb.Append(Morning() ? (kor ? meridiemKorFirst : meridiemEngFirst)
                               : (kor ? meridiemKorLast : meridiemEngLast));
        break;
      case L't':  // A.M./P.M.
        if (wh == 1)
          wsb.Append(Morning() ? L'A' : L'P');
        else if (wh >= 2)
          wsb.Append(Morning()
                         ? (kor ? meridiemKorFirst : meridiemEngFirstShort)
                         : (kor ? meridiemKorLast : meridiemEngLastShort));
        break;
      case L'h':  // 12-hour
        if (wh == 1)
          wsb.Append((Hours() % 12) % 10);
        else if (wh >= 2)
          wsb.Append(WString(Hours() % 12).PadLeft(2, L'0'));
        break;
      case L'H':  // 24-hour
        if (wh == 1)
          wsb.Append((Hours()) % 10);
        else if (wh >= 2)
          wsb.Append(WString(Hours()).PadLeft(2, L'0'));
        break;
      case L'm':  // minute
        if (wh == 1)
          wsb.Append(Minutes() % 10);
        else if (wh >= 2)
          wsb.Append(WString(Minutes()).PadLeft(2, L'0'));
        break;
      case L's':  // second
        if (wh == 1)
          wsb.Append(Seconds() % 10);
        else if (wh >= 2)
          wsb.Append(WString(Seconds()).PadLeft(2, L'0'));
        break;
      default:
        wsb.Append(*ptr++);
        continue;
    }
    ptr += wh;
  }

  return wsb.ToString();
}

struct tm DateTime::___GetNowTm() {
  struct tm now;
  time_t _time;
  time(&_time);
  localtime_s(&now, &_time);
  return now;
}

struct tm DateTime::___FileTime(FILETIME ft) {
  SYSTEMTIME st;
  FileTimeToSystemTime(&ft, &st);
  return ___SystemTime(st);
}

struct tm DateTime::___SystemTime(SYSTEMTIME st) {
  struct tm ftm;
  memset(&ftm, 0, sizeof(struct tm));
  ftm.tm_mday = st.wDay;
  ftm.tm_mon = st.wMonth - 1;
  ftm.tm_year = st.wYear - 1900;
  ftm.tm_min = st.wMinute;
  ftm.tm_hour = st.wHour;
  ftm.tm_sec = st.wSecond;
  return ftm;
}

// http://six-oval.googlecode.com/svn/tags/six-oval-1.2.0_src-tag/test/resources
// /OvalTestContent/5.10/linux/oval-def_time_difference_function.xml
uint64_t DateTime::___FileTimeToTime(FILETIME ft) {
  return (uint64_t)((*(uint64_t *)&ft - 0x19DB1DED53E8000) / 10000000);
}

uint64_t DateTime::GetSecondsFromDate(int year, int month, int day) {
  const int *days = IsLeap(year) ? days366 : days365;

  uint64_t sum = 0;

  if (year > 0) sum += GetDaysFromYear(year);
  if (month > 0) sum += days[month - 1];
  if (day > 0) sum += day - 1;

  return sum * uint64_t(__timeDay);
}

uint64_t DateTime::GetSecondsFromTime(int hour, int minute, int second) {
  return (uint64_t(hour) * 3600 + uint64_t(minute) * 60 + uint64_t(second)) *
         uint64_t(__timeSecond);
}

uint64_t DateTime::GetMonthFromDays(int year, int month) {
  const int *days = IsLeap(year) ? days366 : days365;
  return days[month] - days[month - 1];
}

uint64_t DateTime::GetDaysFromYear(int year) {
  uint64_t y = year - 1;
  return (y * 365) + (y / 4 - y / 100 + y / 400 - 1) + 1;
}

uint64_t DateTime::GetDayFromYear(int year) {
  return (GetDaysFromYear(year) + 1) % 7;
}

// 0: year
// 1: day of year
// 2: month
// 3: day
int DateTime::GetDatePart(int wh) {
  int n = (int)(m_seconds / __timeDay);
  int y400 = n / __Days400Years;
  n -= y400 * __Days400Years;
  int y100 = n / __Days100Years;
  if (y100 == 4) y100 = 3;
  n -= y100 * __Days100Years;
  int y4 = n / __Days4Years;
  n -= y4 * __Days4Years;
  int y1 = n / __DaysYear;
  if (y1 == 4) y1 = 3;
  if (wh == 0) return y400 * 400 + y100 * 100 + y4 * 4 + y1 + 1;
  n -= y1 * __DaysYear;
  if (wh == 1) return n + 1;
  bool leap = y1 == 3 && (y4 != 24 || y100 == 3);
  const int *days = leap ? days366 : days365;
  int m = (n >> 5) + 1;
  while (n >= days[m]) m++;
  if (wh == 2) return m;
  return n - days[m - 1] + 1;
}

bool DateTime::IsLeap(int year) {
  return ((year % 400 == 0) || (year % 100 != 0)) && (year % 4 == 0);
}