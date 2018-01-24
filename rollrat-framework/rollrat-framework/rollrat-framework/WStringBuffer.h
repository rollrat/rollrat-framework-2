//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  WStringBuffer.h - 01/23/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _WSTRING_BUFFER_9bf1541fdf7efd41b7b39543fd870ac4_
#define _WSTRING_BUFFER_9bf1541fdf7efd41b7b39543fd870ac4_

#include <memory.h>

#include "WString.h"

namespace ofw 
{

class WStringBuffer
{
  wchar_t *ptr;
  size_t length;
  size_t capacity;

public:
  WStringBuffer();
  WStringBuffer(size_t capacity);
  ~WStringBuffer();

  void Append(const WString& refer);
  void Append(const wchar_t *wstr);
  void Append(wchar_t ch);
  
  template<typename wt_over>
  void Append(wt_over over) { Append(WString(over)); }

  size_t Length() const { return length; }
  WString ToString();
  void EnsureMore(size_t size);

private:
  void Append(const wchar_t *wstr, size_t len);
  void Ensure(size_t size);
  void EnsureCopy(size_t size);
};

}
#endif