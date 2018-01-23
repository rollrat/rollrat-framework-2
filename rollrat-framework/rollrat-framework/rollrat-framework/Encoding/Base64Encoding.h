//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  Base64Encoding.h - 01/23/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _BASE64_ENCODING_
#define _BASE64_ENCODING_

#include "../WString.h"

namespace ofw
{

class Base64Encoding
{
public:

  using Base64Array = ReadOnlyArray<unsigned char>;

  static WString Base64Encode(unsigned char *bytes, size_t size);
  static WString Base64Encode(const Base64Array& bat) {
    return Base64Encode(bat.Array(), bat.Size());
  }
  static WString Base64Encode(const WString& refer) {
    return Base64Encode((unsigned char *)refer.Reference(), 
      refer.Length() * sizeof(wchar_t)); 
  }
  static WString Base64Encode(const wchar_t *wchs) { 
    return Base64Encode((unsigned char *)wchs, 
      StringTools::wcslen(wchs) * sizeof(wchar_t)); 
  }
  static WString Base64Encode(const char *chs) {
    return Base64Encode((unsigned char *)chs,
      StringTools::strlen(chs));
  }

  static Base64Array Base64Decode(const wchar_t *wchs, size_t size);
  static Base64Array Base64Decode(const WString& refer) {
    return Base64Decode(refer.Reference(), refer.Length());
  }
  static Base64Array Base64Decode(const wchar_t *wchs) {
    return Base64Decode(wchs, StringTools::wcslen(wchs));
  }
  static Base64Array Base64Decode(const char *chs, size_t size);
  static Base64Array Base64Decode(const char *chs) {
    return Base64Decode(chs, StringTools::strlen(chs));
  }

};

}

#endif