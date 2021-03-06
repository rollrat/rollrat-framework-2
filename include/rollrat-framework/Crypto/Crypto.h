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

#ifndef _CRYPTO_9bf1541fdf7efd41b7b39543fd870ac4_
#define _CRYPTO_9bf1541fdf7efd41b7b39543fd870ac4_

#include "rollrat-framework/Crypto/Hash.h"

namespace ofw {

class HashCrypto {
 public:
  using CryptoArray = ReadOnlyArray<uint8_t>;

  static void Run(const CryptoArray& plain, CryptoArray& key);
};

}  // namespace ofw

#endif