//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  renamer.cpp - 02/14/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#include "rollrat-framework/WString.h"
#include "rollrat-framework/WStringBuilder.h"
#include "rollrat-framework/File.h"
#include <vector>
#include <iostream>

using namespace ofw;
using namespace std;

static WString Version = "0.1";
static WString SignString = L"._+-=$[]{}()^%!#&";

static bool is_view_option = false;
static bool view_pattern = false;
static bool view_overlap = false;
static bool view_nonoverlap = false;
static bool is_test = false;

typedef enum {
  Alphabet,
  Digit,
  Sign,
} WordType;

typedef struct {
  WordType Type;
  WString String;
} Word;

vector<WString> files;

///===-----------------------------------------------------------------------===
///               Tokenize
///===-----------------------------------------------------------------------===
vector<Word> tokenFiles() {
  vector<Word> words;
  for (WString fullname : files) {
    // Get file name form full name.
    WString name;
    if (name.Contains(L"/"))
      name = name.Substring(name.FindLast(L'/'));
    else if (name.Contains(L"\\"))
      name = name.Substring(name.FindLast('\\'));


  }
  return words;
}

///===-----------------------------------------------------------------------===
///               Get character from a utf-8 block.
///===-----------------------------------------------------------------------===
static wchar_t getWChar(unsigned long long low, unsigned long mask) {
  wchar_t wt;
  if ((mask & 0xf0) == 0xf0) {
    wt = ((mask & 0xf) << 18) |
      (low & 0x3f) | ((low & 0x3f00) >> 2) | ((low & 0x3f0000) >> 10);
  } else if ((mask & 0xe0) == 0xe0) {
    wt = ((mask & 0x1f) << 12) |
      (low & 0x3f) | ((low & 0x3f00) >> 2);
  } else if ((mask & 0xc0) == 0xc0) {
    wt = (mask & 0x3f >> 6) | (low & 0x3f);
  } else {
    wt = (wchar_t)low;
  }

  return wt;
}

///===-----------------------------------------------------------------------===
///               Get unicode from utf-8 format.
///===-----------------------------------------------------------------------===
WString fromUtf8(char *literal) {
  WStringBuilder wb;
  size_t length = strlen(literal);
  for (size_t i = 0; i < length; i++) {
    unsigned long long buf = 0;
    size_t block_size = 0;

    if ((literal[i] & 0xfc) == 0xfc) {
      block_size = 5;
    } else if ((literal[i] & 0xf8) == 0xf8) {
      block_size = 4;
    } else if ((literal[i] & 0xf0) == 0xf0) {
      block_size = 3;
    } else if ((literal[i] & 0xe0) == 0xe0) {
      block_size = 2;
    } else if ((literal[i] & 0xc0) == 0xc0) {
      block_size = 1;
    }

    if (block_size > 0) {
      for (size_t j = 0; j < block_size; j++)
        buf = (buf | literal[j + 1]) << 8;
      i += block_size;
      wb.Append(getWChar(buf, literal[i]));
    } else {
      wb.Append(literal[i]);
    }
  }
  return wb.ToString();
}

void clSpecificOption(int argc, char* argv[]) {
  for (int c = 1; c < argc; c++) {
    WString Cover = std::move(fromUtf8(argv[c]));
    if (!Cover.CompareTo(L"-view-pattern"))
      view_pattern = true;
    else if (!Cover.CompareTo(L"-view-overlap"))
      view_overlap = true;
    else if (!Cover.CompareTo(L"-view-non-verlap"))
      view_nonoverlap = true;
    else if (!Cover.CompareTo(L"-test"))
      is_test = true;
  }
  is_view_option = view_pattern || view_overlap || view_nonoverlap;
}

void clAddFile(int argc, char* argv[]) {
  int c = 1;

  // Find '-s' flags.
  for ( ; c < argc; c++) {
    WString Cover = std::move(fromUtf8(argv[c]));
    if (Cover.CompareTo(L"-s")) break;
  }

  while (++c < argc) {
    WString Cover = std::move(fromUtf8(argv[c]));
    files.push_back(std::move(Cover));
  }
}

void clHelp() {
  wcout << L"Renamer " << Version << "\n";
  wcout << L"Copyright (C) rollrat. 2018. All rights reserved.\n";
}


int main(int argc, char* argv[])
{
  if (argc == 1) {
    clHelp();
    return 0;
  }

  clSpecificOption(argc, argv);
  clAddFile(argc, argv);

  return 0;
}