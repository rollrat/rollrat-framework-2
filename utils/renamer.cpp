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
#include <algorithm>
#include <vector>
#include <tuple>
#include <iostream>

using namespace ofw;
using namespace std;

static WString Version = L"0.1";
static WString SignString = L"._+-=$[]{}()^%!#&";

static bool is_view_option = false;
static bool view_pattern = false;
static bool view_pattern_rank = false;
static bool view_overlap = false;
static bool is_test = false;
static bool is_count = false;
static bool is_force = false;
static bool is_digit_letter = false;
static bool is_space_sign = false;
static size_t limit_pattern = (size_t)50;
static bool is_without_extension = false;

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

static vector<WString> files;
static vector<token_pair> tokens;
static vector<vector<Word>> patterns;
static vector<vector<bool>> overlaps;


///===-----------------------------------------------------------------------===
///               Get name form fullname
///===-----------------------------------------------------------------------===
WString getFileName(WString fullname) {
  WString name;
  if (fullname.Contains(L"/"))
    name = fullname.Substring(fullname.FindLast(L'/') + 1);
  else if (fullname.Contains(L"\\"))
    name = fullname.Substring(fullname.FindLast('\\') + 1);
  if (is_without_extension && name.FindFirst(L'.')) {
    return name.Remove(name.FindLast(L'.'));
  }
  return name;
}

///===-----------------------------------------------------------------------===
///               Tokenize
///===-----------------------------------------------------------------------===
vector<Word> tokenizeFile(WString fullname) {
  vector<Word> words;

  // Get file name form full name.
  WString name = getFileName(fullname);

  const wchar_t *ptr = name.Reference();
  while (*ptr) {
    Word word;
    WStringBuilder builder(50);
    if (SignString.FindFirst(*ptr) != WString::error) {
      word.Type = WordType::Sign;
      while (SignString.FindFirst(*ptr) != WString::error && *ptr)
        builder.Append(*ptr++);
    } else if (iswdigit(*ptr) && !is_digit_letter) {
      word.Type = WordType::Digit;
      while (iswdigit(*ptr) && *ptr)
        builder.Append(*ptr++);
    } else {
      word.Type = WordType::Alphabet;
      while ((!iswdigit(*ptr) || is_digit_letter) &&
        SignString.FindFirst(*ptr) == WString::error && *ptr)
        builder.Append(*ptr++);
    }
    word.String = builder.ToString();
    words.push_back(word);

    if (words.size() == limit_pattern)
      break;
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
    if (i == patterns.size()) {
      patterns.push_back(tok);
    }
    
    tokens.push_back(token_pair(tok, i));
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
///               Build Overlap
///===-----------------------------------------------------------------------===
void buildOverlap() {
  for (size_t pattern = 0; pattern < patterns.size(); pattern++) {
    size_t target_count = patterns[pattern].size();
    vector<bool> overlap(target_count, true);

    // Find non-overlap token.
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

    overlaps.push_back(overlap);
  }
}

///===-----------------------------------------------------------------------===
///               Count patterns
///===-----------------------------------------------------------------------===
vector<size_t> countPatterns() {
  vector<size_t> count(patterns.size());
  for (size_t i = 0; i < count.size(); i++) {
    for (auto& v : tokens) {
      if (v.second == i)
        count[i]++;
    }
  }
  return count;
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
  builder.Append(L' ');
  builder.Append(getPatternString(std::move(token.first)));
  return builder.ToString();
}

///===-----------------------------------------------------------------------===
///               Set renamer state option.
///===-----------------------------------------------------------------------===
void clSpecificOption(int argc, char* argv[]) {
  for (int c = 1; c < argc; c++) {
    WString Cover = std::move(fromUtf8(argv[c]));
    if (!Cover.CompareTo(L"-view-all")) {
      view_pattern = true;
      view_pattern_rank = true;
      view_overlap = true;
    } else if (!Cover.CompareTo(L"-view-pattern"))
      view_pattern = true;
    else if (!Cover.CompareTo(L"-view-pattern-rank"))
      view_pattern_rank = true;
    else if (!Cover.CompareTo(L"-view-overlap"))
      view_overlap = true;
    else if (!Cover.CompareTo(L"-test"))
      is_test = true;
  }
  is_view_option = view_pattern
    || view_pattern_rank
    || view_overlap;
}

///===-----------------------------------------------------------------------===
///               Set renamer source files.
///===-----------------------------------------------------------------------===
void clAddFile(int argc, char* argv[]) {
  int c = 1;

  // Find '-s' flags.
  for ( ; c < argc; c++) {
    WString Cover = std::move(fromUtf8(argv[c]));
    if (Cover.CompareTo(L"-s")) break;
  }

  size_t count = fromUtf8(argv[++c]).ToUInteger();

  while (count--) {
    WString Cover = std::move(fromUtf8(argv[c]));
    files.push_back(std::move(Cover));
  }
}

///===-----------------------------------------------------------------------===
///               Set renamer source directories.
///===-----------------------------------------------------------------------===
void clAddDirectory(int argc, char* argv[]) {
  int c = 1;

  // Find '-o' flags.
  for (; c < argc; c++) {
    WString Cover = std::move(fromUtf8(argv[c]));
    if (Cover.CompareTo(L"-o")) break;
  }

  size_t count = fromUtf8(argv[++c]).ToUInteger();

  while (count--) {
    WString Cover = std::move(fromUtf8(argv[c]));
    FolderEnumerator fenum(Cover);
    do {
      if (fenum.IsFile())
        files.push_back(std::move(fenum.GetFullName()));
    } while (fenum.NextFile());
  }
}

///===-----------------------------------------------------------------------===
///               Set option.
///===-----------------------------------------------------------------------===
void clRenameOption(int argc, char* argv[]) {
  for (int c = 1; c < argc; c++) {
    WString Cover = std::move(fromUtf8(argv[c])); 
    if (!Cover.CompareTo(L"-count"))
      is_count = true;
    else if (!Cover.CompareTo(L"-force"))
      is_force = true;
    else if (!Cover.CompareTo(L"-digit-letter"))
      is_digit_letter = true;
    else if (!Cover.CompareTo(L"-space-sign"))
      is_space_sign = true;
    else if (!Cover.CompareTo(L"-limit-pattern"))
      limit_pattern = WString(argv[c++]).ToULong();
    else if (!Cover.CompareTo(L"-without-extension"))
      is_without_extension = true;
  }

  if (is_digit_letter)
    SignString = L"._+-=$[]{}()^%!#& ";
}

///===-----------------------------------------------------------------------===
///               Print help message.
///===-----------------------------------------------------------------------===
void clHelp() {
  wcout << L"Renamer ver." << Version << "\n";
  wcout << L"Copyright (C) rollrat. 2018. All rights reserved.\n";
  wcout << L"\n";
  wcout << L"  View option:\n";
  wcout << L"\n";
  wcout << L"    -view-all : Print all procedure.\n";
  wcout << L"    -view-pattern : Print renamer pattern.\n";
  wcout << L"    -view-overlap : Print overlap pattern.\n";
  wcout << L"\n";
  wcout << L"\n";
  wcout << L"  Renamer Option:\n";
  wcout << L"\n";
  wcout << L"    -test : Test rename. This option maybe print test output.\n";
  wcout << L"    -s [count] [path, ...] : Add specific file to source.\n";
  wcout << L"    -o [count] [path, ...] : Add specific directory to source.\n";
  wcout << L"\n";
  wcout << L"\n";
  wcout << L"  Replace Option:\n";
  wcout << L"\n";
  wcout << L"    -count : Tokenize with the number of string.\n";
  wcout << L"    -force : Force rename, even if there is an error.\n";
  wcout << L"    -digit-letter : Treat numbers as letter.\n";
  wcout << L"    -space-sign : Treat space as sign. [Default: Letter]\n";
  wcout << L"    -limit-pattern [count] : Limit the number of patterns.\n";
  wcout << L"    -without-extension : \n";
  wcout << L"\n";
  wcout << L"\n";
}

///===-----------------------------------------------------------------------===
///               Print option
///===-----------------------------------------------------------------------===
void viewOptions() {
  if (view_pattern) {
    wcout << "=========================================================\n";
    wcout << "                      View Pattern\n";
    wcout << "=========================================================\n";

    for (auto& v : tokens) {
      wcout << getTokenString(std::move(v)) << "\n";
    }
    wcout << "\n";
  }

  if (view_pattern_rank) {
    wcout << "=========================================================\n";
    wcout << "                      View Pattern Rank\n";
    wcout << "=========================================================\n";

    using pair_type = pair<size_t, WString>;
    vector<size_t> c = countPatterns();
    vector<pair_type> s;
    for (size_t i = 0; i < c.size(); i++) {
      s.push_back(pair_type(c[i], getPatternString(std::move(patterns[i]))));
    }

    std::sort(s.begin(), s.end(), [](pair_type& v1, pair_type& v2) {
      if (v1.first > v2.first) return true;
      return false;
    });

    for (size_t i = 0; i < s.size(); i++) {
      wcout << L"[" << s[i].first << "]" << s[i].second << "\n";
    }
    wcout << "\n";
  }

  if (view_overlap) {
    wcout << "=========================================================\n";
    wcout << "                      View Overlap\n";
    wcout << "=========================================================\n";

    vector<size_t> c = countPatterns();

    for (size_t i = 0; i < overlaps.size(); i++) {
      size_t first = 0;

      // Find first appeared token.
      for (size_t j = 0; j < tokens.size(); j++) {
        if (tokens[j].second == i) {
          first = j;
          break;
        }
      }

      // Print overlaps.
      wcout << L"Pattern(" << c[i] << "): " << 
        getPatternString(std::move(patterns[i])) << "\n";
      for (size_t j = 0; j < overlaps[i].size(); j++) {
        //if (j != 0) wcout << "+";
        if (overlaps[i][j] == true)
          wcout << tokens[first].first[j].String;
        else
          wcout << "?";
      }
      wcout << "\n\n";
    }
  }
}

int main_renamer(int argc, char* argv[])
{
#if _DEBUG

  FolderEnumerator fenum(LR"(C:\Users\rollrat\Desktop\Project)");
  do {
    if (fenum.IsFile())
      files.push_back(std::move(fenum.GetFullName()));
  } while (fenum.NextFile());

  //is_count = true;
  //is_without_extension = true;
  buildPattern();
  buildOverlap();

  view_pattern = true;
  view_pattern_rank = true;
  view_overlap = true;
  viewOptions();

  return 0;
#else

  if (argc == 1) {
    clHelp();
    return 0;
  }

  clSpecificOption(argc, argv);
  clAddFile(argc, argv);
  clRenameOption(argc, argv);
  buildPattern();
  buildOverlap();

  if (is_view_option) {
    viewOptions();
  }

  return 0;
#endif
}