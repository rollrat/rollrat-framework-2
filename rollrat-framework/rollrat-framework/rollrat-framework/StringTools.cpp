//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  StringTools.cpp - 01/20/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#include <memory.h>
#include <string.h>
#include "StringTools.h"
#include "Defines.h"

using namespace ofw;

#define align_address(n) \
    ((ptr_type *)( (ptr_type)(n) & ~(ptr_type)( sizeof(ptr_type) - 1 ) ) + 1)

#define align_address2(n) \
    ((ptr_type *)( (ptr_type)(n) & ~(ptr_type)( sizeof(ptr_type) - 1 ) ))

// find zero in string
static const char *findzero(const char *str)
{
  if (!str[0]) return str;
  else if (!str[1]) return str + 1;
  else if (!str[2]) return str + 2;
  else if (!str[3]) return str + 3;
  if (sizeof(ptr_type) == sizeof(uint64_t))
  {
    if (!str[4]) return str + 4;
    else if (!str[5]) return str + 5;
    else if (!str[6]) return str + 6;
    else if (!str[7]) return str + 7;
  }
}

/* this function provide counting string-length method by null-terminated string 
   buf is too big, to use this function should be considered prior using.
   or you can make partition routines. */
size_t ofw::StringTools::strlen(const char *str)
{
  if (sizeof(char) != 1)
    return ::strlen(str);

  ptr_type* trim;

  // set has zero checker byte
  const ptr_type less_magic = (ptr_type)(~0ULL / 0xff);
  const ptr_type most_magic = (less_magic << 7);

  trim = align_address(str);

  // Routine 1 Find 0 and 128~255 Byte value in pointer type value.
  if ((*(ptr_type *)str - less_magic) & most_magic)
  {
    if ((*(ptr_type *)str - less_magic) & (~*(ptr_type *)str & most_magic))
      return findzero(str) - str;
  }
  else
  {
    while (1)
    {
      if ((trim[0] - less_magic) & most_magic)
        { trim = &trim[0]; break; }
      if ((trim[1] - less_magic) & most_magic)
        { trim = &trim[1]; break; }
      if ((trim[2] - less_magic) & most_magic)
        { trim = &trim[2]; break; }
      if ((trim[3] - less_magic) & most_magic)
        { trim = &trim[3]; break; }
      if (sizeof(ptr_type) == sizeof(uint32_t))
      {
        if ((trim[4] - less_magic) & most_magic)
          { trim = &trim[4]; break; }
        if ((trim[5] - less_magic) & most_magic)
          { trim = &trim[5]; break; }
        if ((trim[6] - less_magic) & most_magic)
          { trim = &trim[6]; break; }
        if ((trim[7] - less_magic) & most_magic)
          { trim = &trim[7]; break; }
      }
      trim += (sizeof(uint64_t) + sizeof(uint32_t) - sizeof(ptr_type));
    }
  }

  // Routine 2 Find Zero Byte in pointer type value.
  if ((*trim - less_magic) & (~*trim & most_magic))
  {
    return findzero((const char *)trim) - str;
  }

  while (1)
  {
    if ((trim[0] - less_magic) & (~trim[0] & most_magic))
      return findzero((const char *)(trim + 0)) - str;
    if ((trim[1] - less_magic) & (~trim[1] & most_magic))
      return findzero((const char *)(trim + 1)) - str;
    if ((trim[2] - less_magic) & (~trim[2] & most_magic))
      return findzero((const char *)(trim + 2)) - str;
    if ((trim[3] - less_magic) & (~trim[3] & most_magic))
      return findzero((const char *)(trim + 3)) - str;
    if (sizeof(ptr_type) == sizeof(uint32_t))
    {
      if ((trim[4] - less_magic) & (~trim[4] & most_magic))
        return findzero((const char *)(trim + 4)) - str;
      if ((trim[5] - less_magic) & (~trim[5] & most_magic))
        return findzero((const char *)(trim + 5)) - str;
      if ((trim[6] - less_magic) & (~trim[6] & most_magic))
        return findzero((const char *)(trim + 6)) - str;
      if ((trim[7] - less_magic) & (~trim[7] & most_magic))
        return findzero((const char *)(trim + 7)) - str;
    }
    trim += (sizeof(uint64_t) + sizeof(uint32_t) - sizeof(ptr_type));
  }
}

static const wchar_t *wfindzero(const wchar_t *str)
{    
  if (!str[0]) return str;
  else if (!str[1]) return str + 1;
  if (sizeof(ptr_type) == sizeof(uint64_t))
  {
    if (!str[3]) return str + 3;
    else if (!str[4]) return str + 4;
  }
}

size_t ofw::StringTools::wcslen(const wchar_t * str)
{
  if (sizeof(wchar_t) != 2)
    return ::wcslen(str);

  ptr_type* trim;

  // set has zero checker byte
  const ptr_type less_magic = (ptr_type)(~0ULL / 0xffff);
  const ptr_type most_magic = (less_magic << 15);

  trim = align_address(str);
  
  if ((*(ptr_type *)str  - less_magic) & (~*(ptr_type *)str  & most_magic))
  {
    return wfindzero(str) - str;
  }

  // Find Zero Byte in pointer type value.
  while (1)
  {
    if ((trim[0] - less_magic) & (~trim[0] & most_magic))
      return wfindzero((const wchar_t *)(trim + 0)) - str;
    if ((trim[1] - less_magic) & (~trim[1] & most_magic))
      return wfindzero((const wchar_t *)(trim + 1)) - str;
    if ((trim[2] - less_magic) & (~trim[2] & most_magic))
      return wfindzero((const wchar_t *)(trim + 2)) - str;
    if ((trim[3] - less_magic) & (~trim[3] & most_magic))
      return wfindzero((const wchar_t *)(trim + 3)) - str;
    if (sizeof(ptr_type) == sizeof(uint32_t))
    {
      if ((trim[4] - less_magic) & (~trim[4] & most_magic))
        return wfindzero((const wchar_t *)(trim + 4)) - str;
      if ((trim[5] - less_magic) & (~trim[5] & most_magic))
        return wfindzero((const wchar_t *)(trim + 5)) - str;
      if ((trim[6] - less_magic) & (~trim[6] & most_magic))
        return wfindzero((const wchar_t *)(trim + 6)) - str;
      if ((trim[7] - less_magic) & (~trim[7] & most_magic))
        return wfindzero((const wchar_t *)(trim + 7)) - str;
    }
    trim += (sizeof(uint64_t) + sizeof(uint32_t) - sizeof(ptr_type));
  }
}

wchar_t *ofw::StringTools::wcsrnchr(wchar_t * ptr, size_t len, wchar_t ch)
{
  wchar_t *lptr = ptr + len - 1;
  wchar_t *aptr = (wchar_t *)align_address2(lptr);

  const ptr_type less_magic = (ptr_type)(~0ULL / 0xffff);
  const ptr_type most_magic = (less_magic << 15);

  ptr_type wide_checker = ((ptr_type)ch << 0) | ((ptr_type)ch << 16);
  if (sizeof(ptr_type) == sizeof(uint64_t))
    wide_checker |= ((ptr_type)ch << 32) | ((ptr_type)ch << 48);

  if (aptr < ptr) aptr = ptr;

  for (; lptr >= aptr; --lptr)
    if (*lptr == ch) return lptr;

  const size_t count = sizeof(ptr_type) / 2;
  for (lptr = aptr - count; lptr >= ptr; lptr -= count)
  {
    ptr_type nptr = *(ptr_type *)lptr ^ wide_checker;

    if ((nptr - less_magic) & (~nptr & most_magic))
    {
      if (sizeof(ptr_type) == sizeof(uint64_t))
      {
        if (lptr[3] == ch) return lptr + 3;
        if (lptr[2] == ch) return lptr + 2;
      }
      if (lptr[1] == ch) return lptr + 1;
      return lptr;
    }
  }

  const size_t edge = sizeof(ptr_type) - 1;
  for (lptr += edge; lptr >= aptr; --lptr)
    if (*lptr == ch) return lptr;

  return nullptr;
}

wchar_t *ofw::StringTools::wcsrnstrn(wchar_t * ptr, size_t ptrlen, 
  const wchar_t * dest, size_t destlen)
{
  wchar_t *tptr;
  size_t len2 = destlen * sizeof(wchar_t);

  while (tptr = wcsrnchr(ptr, ptrlen, *dest))
  {
    ptrlen = tptr - ptr;
    if (!memcmp(tptr, dest, len2))
      return tptr;
  }

  return NULL;
}

void ofw::StringTools::wcsnset(wchar_t * ptr, wchar_t ch, size_t len)
{
  wchar_t *trim = (wchar_t *)align_address(ptr);

  if (trim < ptr) trim = ptr;

  for (; ptr < trim && len; ptr++, len--)
    *ptr = ch;

  if (len > 0)
  {
    ptr_type put = ((ptr_type)ch << 0) | ((ptr_type)ch << 16);
    if (sizeof(ptr_type) == sizeof(uint64_t))
      put |= ((ptr_type)ch << 32) | ((ptr_type)ch << 48);

    while ( len >= sizeof(ptr_type)/2 )
    {
      *(ptr_type *)trim = put;
      trim += sizeof(ptr_type) / 2;
      len  -= sizeof(ptr_type) / 2;
    }

    if (sizeof(ptr_type) == sizeof(uint64_t))
    {
      if ( len == 3 ) {*(uint32_t *)trim = put; trim[2] = ch; return;}
      if ( len == 2 ) {*(uint32_t *)trim = put; return;}
    }
    if ( len == 1 ) {*(uint16_t *)trim = put; return;}
  }
}

size_t ofw::StringTools::wcountch(wchar_t * ptr, wchar_t * last, wchar_t ch)
{
  size_t count = 0;
  wchar_t *trim = (wchar_t *)align_address(ptr);
  
  const ptr_type less_magic = (ptr_type)(~0ULL / 0xffff);
  const ptr_type most_magic = (less_magic << 15);

  ptr_type wide_checker = ((ptr_type)ch << 0) | ((ptr_type)ch << 16);
  if (sizeof(ptr_type) == sizeof(uint64_t))
    wide_checker |= ((ptr_type)ch << 32) | ((ptr_type)ch << 48);

  if (trim < ptr) trim = ptr;

  for (; ptr < trim; ptr++)
    if (*ptr == ch) count++;
  
  const size_t countc = sizeof(ptr_type) / 2;
  for (ptr = trim; last - countc >= ptr; ptr += countc)
  {
    ptr_type nptr = *(ptr_type *)ptr ^ wide_checker;
    
    if ((nptr - less_magic) & (~nptr & most_magic))
    {
      if (ptr[0] == ch) count++;
      if (ptr[1] == ch) count++;
      
      if (sizeof(ptr_type) == sizeof(uint64_t))
      {
        if (ptr[2] == ch) count++;
        if (ptr[3] == ch) count++;
      }
    }
  }

  return count;
}
