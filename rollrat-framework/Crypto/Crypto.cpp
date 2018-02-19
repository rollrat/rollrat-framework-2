//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  Crypto.h - 01/25/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#include "Crypto.h"

using namespace ofw;

void ofw::HashCrypto::Run(const CryptoArray& plain, CryptoArray& key)
{
  uint8_t *hash = Sha256::FromArray(key.Array(), key.Size());
  uint8_t chain[256] = { 0, };
  size_t round = plain.Size() / 8;
  size_t remain = plain.Size() % 8;
  for (size_t i = 0; i < round; i++)
    ((uint64_t *)plain.Array())[i] ^= ((uint64_t *)hash)[i % 8];

  uint8_t *arr = plain.Array() + round * 8;
  for (size_t i = 0; i < remain; i++)
    arr[i] ^= hash[i];
}