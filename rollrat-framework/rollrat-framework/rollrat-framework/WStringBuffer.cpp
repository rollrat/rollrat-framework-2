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

#include "WStringBuffer.h"

using namespace ofw;

ofw::WStringBuffer::WStringBuffer()
  : ptr(nullptr), length(0), capacity(0)
{
}

ofw::WStringBuffer::WStringBuffer(size_t capacity)
  : ptr(nullptr), length(0), capacity(capacity)
{
  Ensure(capacity);
}

ofw::WStringBuffer::~WStringBuffer()
{
  if (ptr) delete[] ptr;
}

void ofw::WStringBuffer::Append(const WString & refer)
{
  Append(refer.Reference(), refer.Length());
}

void ofw::WStringBuffer::Append(const wchar_t * wstr)
{
  Append(wstr, StringTools::wcslen(wstr));
}

void ofw::WStringBuffer::Append(wchar_t ch)
{
  if (length == capacity)
    EnsureCopy(length + 1);
}

WString ofw::WStringBuffer::ToString()
{
  return WString((const wchar_t *)ptr, length);
}

void ofw::WStringBuffer::EnsureMore(size_t size)
{
  EnsureCopy(size + capacity);
}

void ofw::WStringBuffer::Append(const wchar_t * wstr, size_t len)
{
  if (length + len > capacity)
    EnsureCopy(length + len);
  memcpy(ptr + length, wstr, len * sizeof(wchar_t));
  length += len;
}

void ofw::WStringBuffer::Ensure(size_t size)
{
  ptr = new wchar_t[size];
  capacity = size;
}

void ofw::WStringBuffer::EnsureCopy(size_t size)
{
  if (size > capacity)
  {
    wchar_t *tmp = ptr;
    Ensure(size);
    memcpy(ptr, tmp, length * sizeof(wchar_t));
    delete[] tmp;
  }
}
