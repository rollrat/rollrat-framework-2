//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  UrlEncoding.h - 01/26/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#include "rollrat-framework/Encoding/UrlEncoding.h"

using namespace ofw;

static unsigned long ToUtf8Ch(wchar_t ch) {
  if (ch > 0x7F) {
    int ret = (ch & 0x3F) | ((ch << 2) & 0x00003F00);
    if (ch < 0x800) return ret | 0x0000C080;
    ret |= ((ch << 4) & 0x003F0000);
    if (ch < 0x10000) return ret | 0x00E08080;
    return ret | ((ch << 6) & 0x3F000000) | 0xF0808080;
  }
  return ch;
}

static void AppendHex(uint8_t hex, WStringBuilder& builder) {
  const char* hexmap = "0123456789ABCDEF";
  builder.Append(L'%');
  builder.Append((wchar_t)hexmap[hex >> 4]);
  builder.Append((wchar_t)hexmap[hex & 0xF]);
}

static int getInt(wchar_t ch) {
  if (ch > L'9') {
    if (ch < L'a') {
      return ch - L'A' + 10;
    } else {
      return ch - L'a' + 10;
    }
  }
  return ch & 0xF;
}

static wchar_t getWChar(unsigned long long low, unsigned long mask) {
  wchar_t wt;
  if ((mask & 0xfc) == 0xfc) {
    // 6bytes not exist
    // in wchar_t
  } else if ((mask & 0xf8) == 0xf8) {
    // 5bytes not exist
    // in wchar_t
  } else if ((mask & 0xf0) == 0xf0) {
    wt = ((mask & 0xf) << 18) | (low & 0x3f) | ((low & 0x3f00) >> 2) |
         ((low & 0x3f0000) >> 10);
  } else if ((mask & 0xe0) == 0xe0) {
    wt = ((mask & 0x1f) << 12) | (low & 0x3f) |
         ((low & 0x3f00) >> 2);  // >> 8 + << 6 = >> 2
  } else if ((mask & 0xc0) == 0xc0) {
    wt = (mask & 0x3f >> 6) | (low & 0x3f);
  } else {
    wt = (wchar_t)low;
  }

  return wt;
}

WString ofw::UrlEncoding::UrlEncode(unsigned char* bytes, size_t size) {
  WStringBuilder wb;

  for (size_t i = 0; i < size; i++) {
    AppendHex(bytes[i], wb);
  }

  return wb.ToString();
}

WString ofw::UrlEncoding::UrlEncode(const wchar_t* wchs, size_t length) {
  WStringBuilder wb;
  for (size_t i = 0; i < length; i++) {
    if (wchs[i] <= CHAR_MAX &&
        (iswalnum(wchs[i]) || wcschr(L"*-._", wchs[i]))) {
      wb.Append(wchs[i]);
    } else if (wchs[i] == L' ') {
      wb.Append(L"+");
    } else {
      unsigned long ul = ToUtf8Ch(wchs[i]);
      if (ul >= 0x1000000) AppendHex((ul & 0xff000000) >> 24, wb);
      if (ul >= 0x10000) AppendHex((ul & 0xff0000) >> 16, wb);
      if (ul >= 0x100) AppendHex((ul & 0xff00) >> 8, wb);
      AppendHex(ul & 0xff, wb);
    }
  }
  return wb.ToString();
}

WString ofw::UrlEncoding::UrlDecode(const wchar_t* wchs, size_t length) {
  WStringBuilder wb;
  for (size_t i = 0; i < length;) {
    if (wchs[i] == L'%') {
      unsigned long long buf;
      unsigned long mask = (getInt(wchs[i + 1]) << 4) | (getInt(wchs[i + 2]));

      i += 3;

      if ((mask & 0xfc) == 0xfc) {
        buf = (getInt(wchs[i + 1])) << 36;
        buf |= (getInt(wchs[i + 2])) << 32;
        buf |= (getInt(wchs[i + 4])) << 28;
        buf |= (getInt(wchs[i + 5])) << 24;
        buf |= (getInt(wchs[i + 7])) << 20;
        buf |= (getInt(wchs[i + 8])) << 16;
        buf |= (getInt(wchs[i + 10])) << 12;
        buf |= (getInt(wchs[i + 11])) << 8;
        buf |= (getInt(wchs[i + 13])) << 4;
        buf |= (getInt(wchs[i + 14]));

        i += 15;
      } else if ((mask & 0xf8) == 0xf8) {
        buf = (getInt(wchs[i + 1])) << 28;
        buf |= (getInt(wchs[i + 2])) << 24;
        buf |= (getInt(wchs[i + 4])) << 20;
        buf |= (getInt(wchs[i + 5])) << 16;
        buf |= (getInt(wchs[i + 7])) << 12;
        buf |= (getInt(wchs[i + 8])) << 8;
        buf |= (getInt(wchs[i + 10])) << 4;
        buf |= (getInt(wchs[i + 11]));

        i += 12;
      } else if ((mask & 0xf0) == 0xf0) {
        buf = (getInt(wchs[i + 1])) << 20;
        buf |= (getInt(wchs[i + 2])) << 16;
        buf |= (getInt(wchs[i + 4])) << 12;
        buf |= (getInt(wchs[i + 5])) << 8;
        buf |= (getInt(wchs[i + 7])) << 4;
        buf |= (getInt(wchs[i + 8]));

        i += 9;
      } else if ((mask & 0xe0) == 0xe0)  // long
      {
        buf = (getInt(wchs[i + 1])) << 12;
        buf |= (getInt(wchs[i + 2])) << 8;
        buf |= (getInt(wchs[i + 4])) << 4;
        buf |= (getInt(wchs[i + 5]));

        i += 6;
      } else if ((mask & 0xc0) == 0xc0)  // mb
      {
        buf = (getInt(wchs[i + 1])) << 4;
        buf |= (getInt(wchs[i + 2]));

        i += 3;
      } else  // ansi
      {
        buf = mask;
      }

      wb.Append(getWChar(buf, mask));
    } else {
      if (wchs[i] == L'+')
        wb.Append(L' ');
      else
        wb.Append(wchs[i]);
      i++;
    }
  }
  return wb.ToString();
}
