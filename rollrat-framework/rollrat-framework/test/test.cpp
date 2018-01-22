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

#include "../rollrat-framework/BigInteger/BigFraction.h"

void test_StringTools();

using namespace ofw;
using namespace std;

int main()
{
	/*WString ws(L"<B>rollrat<B> <B>software<B>");
	WString::ArrayType ar = ws.Split(L"<B>");

  ar.Each([] (WString *ws){ wcout << *ws << endl; });

  wcout << ws.Replace(L"<B>", L"->C<-");*/

 /* BigInteger factorial(1);

  for (int i = 1; i < 100; i++)
  {
    factorial *= i;
    if (i == 15)
    cout << i << "!=" << factorial << "\n";
  }*/

  BigFraction bf(0);

  for (int i = 1; i < 1000; i++)
    bf += BigFraction(1, i);

  cout << bf.fraction_point(10) << "\n";

  //cout << "x3F4C54109C045923"_i << "\n";

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