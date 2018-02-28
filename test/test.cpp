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

#include <future>
#include <iostream>
#include <stack>

#include "Test.h"
#include "rollrat-framework/BigInteger/BigFraction.h"
#include "rollrat-framework/ChronoTimer.h"
#include "rollrat-framework/Crypto/Hash.h"
#include "rollrat-framework/Encoding/Base64Encoding.h"
#include "rollrat-framework/Encoding/UrlEncoding.h"
#include "rollrat-framework/File.h"
#include "rollrat-framework/Sorting.h"
#include "rollrat-framework/WString.h"

void test_WStringBuilder();
void test_UrlEncoding();
void test_Hash();
void test_Base64();
void test_Enumerator();
void test_WString();
void test_StringTools();
void test_BigXXX();

using namespace ofw;
using namespace std;

BigBase test_Paraller(long beg, long end) {
  long len = end - beg;
  if (len < 5000) {
    BigBase bi = "1";
    for (int i = beg; i < end; i++) bi *= i;
    return bi;
  }

  long mid = beg + len / 2;
  auto handle = async(launch::async, test_Paraller, mid, end);
  BigBase get = test_Paraller(beg, mid);
  return get * handle.get();
}

int main() {
  std::locale::global(std::locale("kor"));
  std::wcout.imbue(std::locale("kor"));
  std::wcin.imbue(std::locale("kor"));

  ChronoTimer ct;

  ct.start();
  BigBase bi = test_Paraller(1, 200000);
  ct.finish();
  cout << *ct << endl;

  return 0;
}

void test_WStringBuilder() {
  WStringBuilder wsb(5);
  wsb.Append("abcde");
  wsb.Append("fghij");
  wsb.Append("klnmo");
  wsb.Append("pqrst");
  wsb.Append("uvwxy");
  wsb.Append("z0123");
  wsb.Append("45678");
  wsb.Append("9");
  wsb.Replace("b", "c");
  wcout << wsb.ToString() << endl;
}

void test_UrlEncoding() {
  wcout << UrlEncoding::UrlEncode("rollrat") << endl;
  wcout << UrlEncoding::UrlDecode(L"%72%6F%6C%6C%72%61%74") << endl;
}

void test_Hash() {
  // e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
  wcout << Sha256::FromString(L"") << endl;
  wcout << Sha256::FromString(L"rollrat") << endl;
}

void test_Base64() {
  wcout << Base64Encoding::Base64Encode("rollrat") << endl;
  wcout << WString((const char *)Base64Encoding::Base64Decode(
                       Base64Encoding::Base64Encode("rollrat"))
                       .Array(),
                   7);
}

void test_Enumerator() {
  // enumerate specific folder
  FolderEnumerator fe("C:\\");
  do
    wcout << fe.GetFullName() << endl;
  while (fe.NextFolder());

  // enumerate all sub-folder
  stack<FolderEnumerator *> stc;
  stc.push(nullptr);
  FolderEnumerator *ptr = new FolderEnumerator("C:\\");
  while (ptr) {
    if (ptr->IsValid() && ptr->NextFolder()) {
      wcout << ptr->GetFullName() << endl;
      stc.push(ptr);
      ptr = new FolderEnumerator(ptr->GetFullName());

    } else {
      ptr = stc.top();
      stc.pop();
    }
  }
}

void test_WString() {
  // test split and replace
  WString ws(L"<B>rollrat<B> <B>software<B>");
  WString::ArrayType ar = ws.Split(L"<B>");
  ar.Each([](WString *ws) { wcout << *ws << endl; });
  wcout << ws.Replace(L"<B>", L"->C<-");
}

void test_BigXXX() {
  // test bigbase to_string function
  BigInteger factorial(1);
  for (int i = 1; i < 100; i++) {
    factorial *= i;
    if (i == 15) cout << i << "!=" << factorial << "\n";
  }

  // test bigfraction fraction_point
  BigFraction bf(0);
  for (int i = 1; i < 100; i++) bf += BigFraction(1, i);
  cout << bf.fraction_point(10) << "\n";

  // test get phi approach using bif fraction
  bf = BigFraction(0);
  for (int i = 1; i < 500; i++) {
    bf += BigFraction(1, i * 4 - 3);
    bf -= BigFraction(1, i * 4 - 1);
  }
  cout << (bf * 4).fraction_point(100) << "\n";

  // test init_hex string
  cout << "x3F4C54109C045923"_i
       << "\n";
}

void test_StringTools() {
  test_strlen();
  std::cout << '\n';
  test_wcslen();
  std::cout << '\n';
  test_wcsrnchr();
  std::cout << '\n';
  test_wcsrnstrn();
  std::cout << '\n';
  test_wcsnset();
  std::cout << '\n';
}