//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  Base64Encoding.cpp - 01/23/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#include "Base64Encoding.h"

#include "../WStringBuffer.h"

using namespace ofw;

WString ofw::Base64Encoding::Base64Encode(unsigned char * bytes, size_t size)
{
  const wchar_t *table =
    L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    L"abcdefghijklmnopqrstuvwxyz"
    L"0123456789+/";

  WStringBuffer wb(size * 4 / 3 + 2);
  size_t i = 0;

  if (size > 1)
  {
    for (; i < size - 2; i += 3)
    {
      wb.Append(table[(bytes[i] & 0xfc) >> 2]);
      wb.Append(table[((bytes[i] & 0x03) << 4) | 
        ((*(bytes + i + 1) & 0xf0) >> 4)]);
      wb.Append(table[((*(bytes + i + 1) & 0x0f) << 2) | 
        ((*(bytes + i + 2) & 0xc0) >> 6)]);
      wb.Append(table[*(bytes + i + 2) & 0x3f]);
    }
  }

  if (i < size)
  {
    wb.Append(table[(bytes[i] & 0xfc) >> 2]);
    if (i + 1 == size)
    {
      wb.Append(table[(bytes[i] & 0x03) << 4]);
      wb.Append(L"==");
    }
    else
    {
      wb.Append(table[((bytes[i] & 0x03) << 4) | 
        (*(bytes + i + 1) & 0xf0) >> 4]);
      //      (*(bytes + i + 1) & 0x0f) << 2;
      wb.Append(table[(*(bytes + i + 1) & 0x0f) << 2]);
      wb.Append(L"=");
    }
  }

  return wb.ToString();
}

Base64Encoding::Base64Array ofw::Base64Encoding::Base64Decode(
  const wchar_t * wchs, size_t size)
{
  if (size % 4 != 0)
    return Base64Encoding::Base64Array(nullptr, 0);

  const unsigned char reversetable[] = {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,52,53,
    54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,
    13,14,15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,-1,26,27,28,29,30,31,
    32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
  };

  size_t totalsz;

  unsigned char *ret = new unsigned char[
    totalsz = (size * 3) / 4 -
      ((wchs[size - 1] == L'=') + (wchs[size - 2] == L'='))
  ];
  unsigned char *ptr = ret;

  for (size_t i = 0; i < size; i += 4)
  {
    *ptr++ = (unsigned char)((reversetable[wchs[i]] << 2) | 
      (reversetable[wchs[i + 1]] >> 4));
    *ptr++ = (unsigned char)((reversetable[wchs[i + 1]] << 4) | 
      (reversetable[wchs[i + 2]] >> 2));
    *ptr++ = (unsigned char)((reversetable[wchs[i + 2]] << 6) | 
      (reversetable[wchs[i + 3]]));
  }

  return Base64Encoding::Base64Array(ret, totalsz);
}

Base64Encoding::Base64Array ofw::Base64Encoding::Base64Decode(const char * chs, size_t size)
{
  if (size % 4 != 0)
    return Base64Encoding::Base64Array(nullptr, 0);

  const unsigned char reversetable[] = {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,52,53,
    54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,
    13,14,15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,-1,26,27,28,29,30,31,
    32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
  };

  size_t totalsz;

  unsigned char *ret = new unsigned char[
    totalsz = (size * 3) / 4 -
      ((chs[size - 1] == L'=') + (chs[size - 2] == L'='))
  ];
  unsigned char *ptr = ret;

  for (size_t i = 0; i < size; i += 4)
  {
    *ptr++ = (unsigned char)((reversetable[chs[i]] << 2) | 
      (reversetable[chs[i + 1]] >> 4));
    *ptr++ = (unsigned char)((reversetable[chs[i + 1]] << 4) | 
      (reversetable[chs[i + 2]] >> 2));
    *ptr++ = (unsigned char)((reversetable[chs[i + 2]] << 6) | 
      (reversetable[chs[i + 3]]));
  }

  return Base64Encoding::Base64Array(ret, totalsz);
}
