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
#include <tuple>
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
static bool is_count = false;

typedef enum {
  Alphabet,
  Digit,
  Sign,
} WordType;

typedef struct {
  WordType Type;
  WString String;
} Word;

using token_pair = pair<vector<Word>, int>;

vector<WString> files;
vector<token_pair> tokens;
vector<vector<Word>> patterns;

///===-----------------------------------------------------------------------===
///               Tokenize
///===-----------------------------------------------------------------------===
vector<Word> tokenizeFile(WString fullname) {
  vector<Word> words;

  // Get file name form full name.
  WString name;
  if (fullname.Contains(L"/"))
    name = fullname.Substring(fullname.FindLast(L'/') + 1);
  else if (fullname.Contains(L"\\"))
    name = fullname.Substring(fullname.FindLast('\\') + 1);

  const wchar_t *ptr = name.Reference();
  while (*ptr) {
    Word word;
    WStringBuilder builder(50);
    if (SignString.FindFirst(*ptr) != WString::error) {
      word.Type = WordType::Sign;
      while (SignString.FindFirst(*ptr) != WString::error && *ptr)
        builder.Append(*ptr++);
    } else if (isdigit(*ptr)) {
      word.Type = WordType::Digit;
      while (isdigit(*ptr) && *ptr)
        builder.Append(*ptr++);
    } else {
      word.Type = WordType::Alphabet;
      while (!isdigit(*ptr) && 
        SignString.FindFirst(*ptr) == WString::error && *ptr)
        builder.Append(*ptr++);
    }
    word.String = builder.ToString();
    words.push_back(word);
  }
  
  return words;
}

///===-----------------------------------------------------------------------===
///               Token Equal
///===-----------------------------------------------------------------------===
bool tokenEqual(vector<Word>& token1, vector<Word>& token2) {
  if (token1.size() != token2.size()) return false;
  for (size_t i = 0; i < token1.size(); i++) {
    if (token1[i].Type != token2[i].Type) return false;
    if (is_count && token1[i].String.Length() != token2[i].String.Length())
      return false;
  }
  return true;
}

///===-----------------------------------------------------------------------===
///               Build Pattern
///===-----------------------------------------------------------------------===
void buildPattern() {
  for (WString path : files) {
    vector<Word> tok = tokenizeFile(path);

    // Find previous overlap pattern.
    size_t i = 0;
    for ( ; i < patterns.size(); i++) {
      if (tokenEqual(patterns[i], tok))
        break;
    }

    // If pattern is not found, then add new pattern.
    if (i != patterns.size()) {
      patterns.push_back(std::move(tok));
    }
    
    tokens.push_back(token_pair(std::move(tok), i));
  }
}

///===-----------------------------------------------------------------------===
///               Check Overlap
///===-----------------------------------------------------------------------===
vector<bool> checkOverlap(int pattern) {
  size_t target_count = patterns[pattern].size();
  vector<bool> overlap(target_count, true);
  for (size_t i = 0; i < target_count; i++) {
    WString target;
    for (token_pair& token : tokens) {
      if (token.second == pattern) {
        if (target.Empty()) {
          target.CloneSet(token.first[i].String);
          continue;
        }

        if (target != token.first[i].String) {
          overlap[i] = false;
          break;
        }
      }
    }
  }

  return overlap;
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

///===-----------------------------------------------------------------------===
///               Pattern to string.
///===-----------------------------------------------------------------------===
WString getPatternString(vector<Word>&& pattern) {
  WStringBuilder builder;
  for (size_t i = 0; i < pattern.size(); i++) {
    if (i != 0) builder.Append(L'+');
    switch (pattern[i].Type)
    {
    case WordType::Alphabet: builder.Append(L"Alpha"); break;
    case WordType::Digit: builder.Append(L"Digit"); break;
    case WordType::Sign: builder.Append(L"Sign"); break;
    }
    if (is_count) {
      builder.Append(L'(');
      builder.Append(pattern[i].String.Length());
      builder.Append(L')');
    }
  }
  return builder.ToString();
}

///===-----------------------------------------------------------------------===
///               Token to string.
///===-----------------------------------------------------------------------===
WString getTokenString(token_pair&& token) {
  WStringBuilder builder;
  builder.Append(L'[');
  builder.Append(token.second);
  builder.Append(L']');
  builder.Append(getPatternString(std::move(token.first)));
  return builder.ToString();
}

///===-----------------------------------------------------------------------===
///               Set renamer state option.
///===-----------------------------------------------------------------------===
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
    else if (!Cover.CompareTo(L"-count"))
      is_count = true;
  }
  is_view_option = view_pattern || view_overlap || view_nonoverlap;
}

///===-----------------------------------------------------------------------===
///               Set renamer sources.
///===-----------------------------------------------------------------------===
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

///===-----------------------------------------------------------------------===
///               Print help message.
///===-----------------------------------------------------------------------===
void clHelp() {
  wcout << L"Renamer " << Version << "\n";
  wcout << L"Copyright (C) rollrat. 2018. All rights reserved.\n";
}

///===-----------------------------------------------------------------------===
///               Print option
///===-----------------------------------------------------------------------===
void viewOptions() {
  if (view_pattern) {
    for (auto& v : tokens) {
      wcout << getTokenString(std::move(v)) << "\n";
    }
  }
}

int main(int argc, char* argv[])
{
  is_count = true;
  wcout << getPatternString(tokenizeFile(LR"(C:\»õ Æú´õ\llvm-control-pass\CustomPass.cpp)"));

  //////////////////

  if (argc == 1) {
    clHelp();
    return 0;
  }

  clSpecificOption(argc, argv);
  clAddFile(argc, argv);
  buildPattern();

  if (is_view_option) {
    viewOptions();
  }

  return 0;
}