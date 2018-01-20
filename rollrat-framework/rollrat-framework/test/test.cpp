//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  test.cpp - 01/21/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#include <iostream>

#include "Test.h"

void test_StringTools();

int main()
{
  test_StringTools();

  return 0;
}

void test_StringTools()
{
  test_strlen();    std::cout << '\n';
  test_wcslen();    std::cout << '\n';
  test_wcsrnchr();  std::cout << '\n';
  test_wcsrnstrn(); std::cout << '\n';
  test_wcsnset();   std::cout << '\n';
}