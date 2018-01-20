//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  StringTools.h - 01/20/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _STRING_TOOLS_
#define _STRING_TOOLS_

namespace ofw
{

class StringTools
{
public:
  static size_t strlen(const char *str);
  static size_t wcslen(const wchar_t *str);
  static wchar_t *wcsrnchr(wchar_t *ptr, size_t len, wchar_t ch);
  static wchar_t *wcsrnstrn(wchar_t *ptr, size_t ptrlen, 
    const wchar_t *dest, size_t destlen);
  static void wcsnset(wchar_t *ptr, wchar_t ch, size_t len);
  static size_t wcountch(wchar_t *ptr, wchar_t *last, wchar_t ch);
};

}

#endif