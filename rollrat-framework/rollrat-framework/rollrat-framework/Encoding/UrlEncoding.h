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

#ifndef _URL_ENCODING_9bf1541fdf7efd41b7b39543fd870ac4_
#define _URL_ENCODING_9bf1541fdf7efd41b7b39543fd870ac4_

#include "WString.h"
#include "WStringBuilder.h"

namespace ofw
{

class UrlEncoding
{
public:

  using UrlEncodingArray = ReadOnlyArray<unsigned char>;

  static WString UrlEncode(unsigned char *bytes, size_t size);
  static WString UrlEncode(const UrlEncodingArray& bat) {
    return UrlEncode(bat.Array(), bat.Size());
  }
  static WString UrlEncode(const char *chs) {
    return UrlEncode((unsigned char *)chs, StringTools::strlen(chs));
  }
  static WString UrlEncode(const wchar_t *wchs, size_t length);
  static WString UrlEncode(const wchar_t *wchs) {
    return UrlEncode(wchs, StringTools::wcslen(wchs));
  }
  static WString UrlEncode(const WString& refer) {
    return UrlEncode(refer.Reference(), refer.Length());
  }

  static WString UrlDecode(const wchar_t *wchs, size_t length);
  static WString UrlDecode(const wchar_t *wchs) {
    return UrlDecode(wchs, StringTools::wcslen(wchs));
  }
  static WString UrlDecode(const WString& refer) {
    return UrlDecode(refer.Reference(), refer.Length());
  }

};

}

#endif