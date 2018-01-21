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
#include "../rollrat-framework/WString.h"

#include "../rollrat-framework/BigInteger/BigInteger.h"

void test_StringTools();

using namespace ofw;
using namespace std;

int main()
{
	/*WString ws(L"<B>rollrat<B> <B>software<B>");
	WString::ArrayType ar = ws.Split(L"<B>");

  ar.Each([] (WString *ws){ wcout << *ws << endl; });

  wcout << ws.Replace(L"<B>", L"->C<-");*/

  BigInteger factorial(1);

  for (int i = 1; i < 100; i++)
  {
    factorial *= i;
    //if (i == 13)
    cout << i << "!=" << factorial << "\n";
  }

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