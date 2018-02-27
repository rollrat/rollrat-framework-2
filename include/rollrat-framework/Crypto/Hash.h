//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  Hash.h - 01/24/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _HASH_9bf1541fdf7efd41b7b39543fd870ac4_
#define _HASH_9bf1541fdf7efd41b7b39543fd870ac4_

#include "rollrat-framework/Crypto/Sha256.h"
#include "rollrat-framework/WString.h"
#include "rollrat-framework/WStringBuffer.h"

namespace ofw {

class Sha256 {
 public:
  static uint8_t *FromArray(uint8_t *hash, size_t size) {
    uint8_t *arr = new uint8_t[32];
    SHA256(hash, size, arr);
    return arr;
  }

  static WString FromString(const WString &wstr) {
    const char *hexmap = "0123456789abcdef";
    uint8_t *hash = FromArray((uint8_t *)wstr.Reference(), wstr.Length() * 2);
    WStringBuffer buffer(64);
    for (int i = 0; i < 32; i++) {
      buffer.Append((wchar_t)hexmap[hash[i] >> 4]);
      buffer.Append((wchar_t)hexmap[hash[i] & 0xF]);
    }
    return buffer.ToString();
  }

  static WString FromString(const char *str, size_t length) {
    const char *hexmap = "0123456789abcdef";
    uint8_t *hash = FromArray((uint8_t *)str, length);
    WStringBuffer buffer(64);
    for (int i = 0; i < 32; i++) {
      buffer.Append((wchar_t)hexmap[hash[i] >> 4]);
      buffer.Append((wchar_t)hexmap[hash[i] & 0xF]);
    }
    return buffer.ToString();
  }

  static WString FromString(const char *str) {
    return FromString(str, StringTools::strlen(str));
  }
};

}  // namespace ofw

#endif