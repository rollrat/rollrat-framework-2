//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  Sha256.h - 08/27/2015
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _SHA_256_
#define _SHA_256_

#ifdef __cplusplus
extern "C" {
#endif
	
typedef unsigned long  SHA_ULONG, *SHA_PULONG;
typedef unsigned char SHA_BYTE, *SHA_PBYTE;

void SHA256(const SHA_BYTE *target, SHA_ULONG len, SHA_BYTE *source);

#ifdef __cplusplus
}
#endif

#endif