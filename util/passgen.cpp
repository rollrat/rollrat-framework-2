//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  passgen.cpp - 01/26/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#include <rollrat-framework/Crypto/Hash.h>
#include <rollrat-framework/Random.h>
#include <rollrat-framework/WString.h>
#include <iomanip>
#include <iostream>
#include <string>

using namespace ofw;
using namespace std;

const wchar_t *suits[] = {
    L"Amazon",    L"Apple",     L"DCInside", L"Discord",  L"Ebay",
    L"Facebook",  L"Github",    L"Google",   L"LOL",      L"Steam",
    L"Tumblr",    L"Twitch",    L"Twitter",  L"Namuwiki", L"Naver",
    L"Microsoft", L"Wikipedia",
};

static const size_t count = (sizeof suits) / sizeof(wchar_t *);

int main() {
  std::locale::global(std::locale("kor"));
  std::wcout.imbue(std::locale("kor"));
  std::wcin.imbue(std::locale("kor"));

  wcout << L"RollRat Software Passgen\n\n";

  while (true) {
    wcout << L"Write your master password: ";

    wstring line;
    getline(std::wcin, line);

    for (size_t i = 0; i < ::count; i++) {
      wchar_t specific = L"/+=^$#@!&*"[Random::getRandomNumber() % 10];
      wchar_t upper = L'A' + (Random::getRandomNumber() % ('z' - 'a' + 1));
      WString plain = WString(specific) + WString(upper) +
                      Sha256::FromString(suits[i] + line).Remove(10);
      wcout << setw(15) << suits[i] << L": " << plain << endl;
    }
  }
  return 0;
}