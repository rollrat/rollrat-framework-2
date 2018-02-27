//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  test-StringTools.cpp - 01/21/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#include <memory>
#include <stdio.h>
#include <string>
#include <iostream>

#include "Test.h"
#include "rollrat-framework/StringTools.h"

#define L1(x) x x x x
#define L2(x) L1(x) L1(x) L1(x) L1(x)
#define L3(x) L2(x) L2(x) L2(x) L2(x)
#define L4(x) L3(x) L3(x) L3(x) L3(x)
#define L5(x) L4(x) L4(x) L4(x) L4(x)
#define L6(x) L5(x) L5(x) L5(x) L5(x)

void test_strlen()
{
  const char testTarget[] = L6("rollrat");
  char* testCopy = new char[sizeof(testTarget)];
  memcpy(testCopy, testTarget, sizeof(testTarget));

  size_t a = ofw::StringTools::strlen(testCopy);
  delete[] testCopy;

  printf("%d %d", a, ::strlen(testTarget));
}

void test_wcslen()
{
  const wchar_t testTarget[] = L6(L"rollrat");
  wchar_t* testCopy = new wchar_t[sizeof(testTarget)/2];
  memcpy(testCopy, testTarget, sizeof(testTarget));

  size_t a = ofw::StringTools::wcslen(testCopy);
  delete[] testCopy;

  printf("%d %d", a, ::wcslen(testTarget));
}

void test_wcsrnchr()
{
  wchar_t test[] = L"rollrat software";
  wchar_t target = L'o';

  std::wcout << ofw::StringTools::wcsrnchr(test, sizeof(test)/2, target);
}

void test_wcsrnstrn()
{
  wchar_t test[] = L"rollrat software rollrat software";
  wchar_t target[] = L"rollrat";

  std::wcout << ofw::StringTools::wcsrnstrn(test, sizeof(test)/2, target, 7);
}

void test_wcsnset()
{
  wchar_t test[] = L"rollrat rollrat";

  ofw::StringTools::wcsnset(test + 1, L'K', 3);
  std::wcout << test;
}