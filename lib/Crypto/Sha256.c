//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  Sha256.c - 08/27/2015
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#include <stdio.h>
#include <memory.h>
#include <string.h>

#include "rollrat-framework/Crypto/Sha256.h"

#define _Rotate_Left(x, r, n, t)  ((t)((x) << (r)) | (t)((x) >> ((n)-(r))))
#define _Rotate_Right(x, r, n, t)  ((t)((x) >> (r)) | (t)((x) << ((n)-(r))))
#define RR_UL(x, r)      _Rotate_Right(x, r, 32, SHA_ULONG)
#define RL_UL(x, r)      _Rotate_Left(x, r, 32, SHA_ULONG
#define RR_ULL(x, r)     _Rotate_Right(x, r, 64, unsigned long long)
#define RL_ULL(x, r)     _Rotate_Left(x, r, 64, unsigned long long)

#define CONVERT_TO_LITTLE_ENDIAN(x) ((0x000000ff & x) << 24 | (0x0000ff00 & x) << 8 | (0x00ff0000 & x) >> 8 | (0xff000000 & x) >> 24)

const SHA_ULONG SHA_256_K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

const SHA_ULONG SHA_256_H[8] = {
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

#define SHA_256_SIGMA0(A)  (RR_UL(A, 2) ^ RR_UL(A, 13) ^ RR_UL(A, 22))
#define SHA_256_SIGMA1(E)  (RR_UL(E, 6) ^ RR_UL(E, 11) ^ RR_UL(E, 25))

#define SHA_256_Ch(E,F,G)  (((E) & (F)) ^ ((~E) & (G)))
#define SHA_256_Maj(A,B,C)  (((A) & (B)) ^ ((B) & (C)) ^ ((C) & (A)))

#define SHA_256_s0(wi_15)  (RR_UL(wi_15, 7) ^ RR_UL(wi_15, 18) ^ (wi_15 >>  3))
#define SHA_256_s1(wi_2)  (RR_UL(wi_2, 17) ^ RR_UL(wi_2,  19) ^ (wi_2  >> 10))

#define SHA_256_LOOP(a, b, c, d, e, f, g, h ,i, t1, t2) {\
	t1 = h+SHA_256_SIGMA1(e)+SHA_256_Ch(e,f,g)+SHA_256_K[i]+w[i];\
	t2 = SHA_256_SIGMA0(a)+SHA_256_Maj(a,b,c);\
	}

#define SHA_256_XROLL(a, b, c, d, e, f, g, h, t1, t2) {\
 h = g, g = f, f = e, e = d + t1, d = c, c = b, b = a, a = t1 + t2;\
	 }

typedef struct _sha256_context {
	SHA_ULONG outdexs[8];
	SHA_BYTE buffer[64];
	SHA_ULONG residue, inces;
} SHA256_CONTEXT, *PSHA256_CONTEXT;

void SHA256_Mixing(SHA_PULONG indexs, SHA_PULONG outdexs)
{
	SHA_ULONG w[64];
	SHA_ULONG i, a, b, c, d, e, f, g, h, t1, t2;

	for (i = 0; i < 16; i++)
		w[i] = CONVERT_TO_LITTLE_ENDIAN(indexs[i]);

	for (i = 16; i < 64; i++)
		w[i] = SHA_256_s0(w[i - 15]) + w[i - 16] + w[i - 7] + SHA_256_s1(w[i - 2]);

	a = outdexs[0];
	b = outdexs[1];
	c = outdexs[2];
	d = outdexs[3];
	e = outdexs[4];
	f = outdexs[5];
	g = outdexs[6];
	h = outdexs[7];

	t1 = h + SHA_256_SIGMA1(e) + SHA_256_Ch(e, f, g) + 0x428a2f98 + w[0];
	t2 = SHA_256_SIGMA0(a) + SHA_256_Maj(a, b, c);    d += t1;    h = t1 + t2;
	t1 = g + SHA_256_SIGMA1(d) + SHA_256_Ch(d, e, f) + 0x71374491 + w[1];
	t2 = SHA_256_SIGMA0(h) + SHA_256_Maj(h, a, b);    c += t1;    g = t1 + t2;
	t1 = f + SHA_256_SIGMA1(c) + SHA_256_Ch(c, d, e) + 0xb5c0fbcf + w[2];
	t2 = SHA_256_SIGMA0(g) + SHA_256_Maj(g, h, a);    b += t1;    f = t1 + t2;
	t1 = e + SHA_256_SIGMA1(b) + SHA_256_Ch(b, c, d) + 0xe9b5dba5 + w[3];
	t2 = SHA_256_SIGMA0(f) + SHA_256_Maj(f, g, h);    a += t1;    e = t1 + t2;
	t1 = d + SHA_256_SIGMA1(a) + SHA_256_Ch(a, b, c) + 0x3956c25b + w[4];
	t2 = SHA_256_SIGMA0(e) + SHA_256_Maj(e, f, g);    h += t1;    d = t1 + t2;
	t1 = c + SHA_256_SIGMA1(h) + SHA_256_Ch(h, a, b) + 0x59f111f1 + w[5];
	t2 = SHA_256_SIGMA0(d) + SHA_256_Maj(d, e, f);    g += t1;    c = t1 + t2;
	t1 = b + SHA_256_SIGMA1(g) + SHA_256_Ch(g, h, a) + 0x923f82a4 + w[6];
	t2 = SHA_256_SIGMA0(c) + SHA_256_Maj(c, d, e);    f += t1;    b = t1 + t2;
	t1 = a + SHA_256_SIGMA1(f) + SHA_256_Ch(f, g, h) + 0xab1c5ed5 + w[7];
	t2 = SHA_256_SIGMA0(b) + SHA_256_Maj(b, c, d);    e += t1;    a = t1 + t2;

	t1 = h + SHA_256_SIGMA1(e) + SHA_256_Ch(e, f, g) + 0xd807aa98 + w[8];
	t2 = SHA_256_SIGMA0(a) + SHA_256_Maj(a, b, c);    d += t1;    h = t1 + t2;
	t1 = g + SHA_256_SIGMA1(d) + SHA_256_Ch(d, e, f) + 0x12835b01 + w[9];
	t2 = SHA_256_SIGMA0(h) + SHA_256_Maj(h, a, b);    c += t1;    g = t1 + t2;
	t1 = f + SHA_256_SIGMA1(c) + SHA_256_Ch(c, d, e) + 0x243185be + w[10];
	t2 = SHA_256_SIGMA0(g) + SHA_256_Maj(g, h, a);    b += t1;    f = t1 + t2;
	t1 = e + SHA_256_SIGMA1(b) + SHA_256_Ch(b, c, d) + 0x550c7dc3 + w[11];
	t2 = SHA_256_SIGMA0(f) + SHA_256_Maj(f, g, h);    a += t1;    e = t1 + t2;
	t1 = d + SHA_256_SIGMA1(a) + SHA_256_Ch(a, b, c) + 0x72be5d74 + w[12];
	t2 = SHA_256_SIGMA0(e) + SHA_256_Maj(e, f, g);    h += t1;    d = t1 + t2;
	t1 = c + SHA_256_SIGMA1(h) + SHA_256_Ch(h, a, b) + 0x80deb1fe + w[13];
	t2 = SHA_256_SIGMA0(d) + SHA_256_Maj(d, e, f);    g += t1;    c = t1 + t2;
	t1 = b + SHA_256_SIGMA1(g) + SHA_256_Ch(g, h, a) + 0x9bdc06a7 + w[14];
	t2 = SHA_256_SIGMA0(c) + SHA_256_Maj(c, d, e);    f += t1;    b = t1 + t2;
	t1 = a + SHA_256_SIGMA1(f) + SHA_256_Ch(f, g, h) + 0xc19bf174 + w[15];
	t2 = SHA_256_SIGMA0(b) + SHA_256_Maj(b, c, d);    e += t1;    a = t1 + t2;

	t1 = h + SHA_256_SIGMA1(e) + SHA_256_Ch(e, f, g) + 0xe49b69c1 + w[16];
	t2 = SHA_256_SIGMA0(a) + SHA_256_Maj(a, b, c);    d += t1;    h = t1 + t2;
	t1 = g + SHA_256_SIGMA1(d) + SHA_256_Ch(d, e, f) + 0xefbe4786 + w[17];
	t2 = SHA_256_SIGMA0(h) + SHA_256_Maj(h, a, b);    c += t1;    g = t1 + t2;
	t1 = f + SHA_256_SIGMA1(c) + SHA_256_Ch(c, d, e) + 0x0fc19dc6 + w[18];
	t2 = SHA_256_SIGMA0(g) + SHA_256_Maj(g, h, a);    b += t1;    f = t1 + t2;
	t1 = e + SHA_256_SIGMA1(b) + SHA_256_Ch(b, c, d) + 0x240ca1cc + w[19];
	t2 = SHA_256_SIGMA0(f) + SHA_256_Maj(f, g, h);    a += t1;    e = t1 + t2;
	t1 = d + SHA_256_SIGMA1(a) + SHA_256_Ch(a, b, c) + 0x2de92c6f + w[20];
	t2 = SHA_256_SIGMA0(e) + SHA_256_Maj(e, f, g);    h += t1;    d = t1 + t2;
	t1 = c + SHA_256_SIGMA1(h) + SHA_256_Ch(h, a, b) + 0x4a7484aa + w[21];
	t2 = SHA_256_SIGMA0(d) + SHA_256_Maj(d, e, f);    g += t1;    c = t1 + t2;
	t1 = b + SHA_256_SIGMA1(g) + SHA_256_Ch(g, h, a) + 0x5cb0a9dc + w[22];
	t2 = SHA_256_SIGMA0(c) + SHA_256_Maj(c, d, e);    f += t1;    b = t1 + t2;
	t1 = a + SHA_256_SIGMA1(f) + SHA_256_Ch(f, g, h) + 0x76f988da + w[23];
	t2 = SHA_256_SIGMA0(b) + SHA_256_Maj(b, c, d);    e += t1;    a = t1 + t2;

	t1 = h + SHA_256_SIGMA1(e) + SHA_256_Ch(e, f, g) + 0x983e5152 + w[24];
	t2 = SHA_256_SIGMA0(a) + SHA_256_Maj(a, b, c);    d += t1;    h = t1 + t2;
	t1 = g + SHA_256_SIGMA1(d) + SHA_256_Ch(d, e, f) + 0xa831c66d + w[25];
	t2 = SHA_256_SIGMA0(h) + SHA_256_Maj(h, a, b);    c += t1;    g = t1 + t2;
	t1 = f + SHA_256_SIGMA1(c) + SHA_256_Ch(c, d, e) + 0xb00327c8 + w[26];
	t2 = SHA_256_SIGMA0(g) + SHA_256_Maj(g, h, a);    b += t1;    f = t1 + t2;
	t1 = e + SHA_256_SIGMA1(b) + SHA_256_Ch(b, c, d) + 0xbf597fc7 + w[27];
	t2 = SHA_256_SIGMA0(f) + SHA_256_Maj(f, g, h);    a += t1;    e = t1 + t2;
	t1 = d + SHA_256_SIGMA1(a) + SHA_256_Ch(a, b, c) + 0xc6e00bf3 + w[28];
	t2 = SHA_256_SIGMA0(e) + SHA_256_Maj(e, f, g);    h += t1;    d = t1 + t2;
	t1 = c + SHA_256_SIGMA1(h) + SHA_256_Ch(h, a, b) + 0xd5a79147 + w[29];
	t2 = SHA_256_SIGMA0(d) + SHA_256_Maj(d, e, f);    g += t1;    c = t1 + t2;
	t1 = b + SHA_256_SIGMA1(g) + SHA_256_Ch(g, h, a) + 0x06ca6351 + w[30];
	t2 = SHA_256_SIGMA0(c) + SHA_256_Maj(c, d, e);    f += t1;    b = t1 + t2;
	t1 = a + SHA_256_SIGMA1(f) + SHA_256_Ch(f, g, h) + 0x14292967 + w[31];
	t2 = SHA_256_SIGMA0(b) + SHA_256_Maj(b, c, d);    e += t1;    a = t1 + t2;

	t1 = h + SHA_256_SIGMA1(e) + SHA_256_Ch(e, f, g) + 0x27b70a85 + w[32];
	t2 = SHA_256_SIGMA0(a) + SHA_256_Maj(a, b, c);    d += t1;    h = t1 + t2;
	t1 = g + SHA_256_SIGMA1(d) + SHA_256_Ch(d, e, f) + 0x2e1b2138 + w[33];
	t2 = SHA_256_SIGMA0(h) + SHA_256_Maj(h, a, b);    c += t1;    g = t1 + t2;
	t1 = f + SHA_256_SIGMA1(c) + SHA_256_Ch(c, d, e) + 0x4d2c6dfc + w[34];
	t2 = SHA_256_SIGMA0(g) + SHA_256_Maj(g, h, a);    b += t1;    f = t1 + t2;
	t1 = e + SHA_256_SIGMA1(b) + SHA_256_Ch(b, c, d) + 0x53380d13 + w[35];
	t2 = SHA_256_SIGMA0(f) + SHA_256_Maj(f, g, h);    a += t1;    e = t1 + t2;
	t1 = d + SHA_256_SIGMA1(a) + SHA_256_Ch(a, b, c) + 0x650a7354 + w[36];
	t2 = SHA_256_SIGMA0(e) + SHA_256_Maj(e, f, g);    h += t1;    d = t1 + t2;
	t1 = c + SHA_256_SIGMA1(h) + SHA_256_Ch(h, a, b) + 0x766a0abb + w[37];
	t2 = SHA_256_SIGMA0(d) + SHA_256_Maj(d, e, f);    g += t1;    c = t1 + t2;
	t1 = b + SHA_256_SIGMA1(g) + SHA_256_Ch(g, h, a) + 0x81c2c92e + w[38];
	t2 = SHA_256_SIGMA0(c) + SHA_256_Maj(c, d, e);    f += t1;    b = t1 + t2;
	t1 = a + SHA_256_SIGMA1(f) + SHA_256_Ch(f, g, h) + 0x92722c85 + w[39];
	t2 = SHA_256_SIGMA0(b) + SHA_256_Maj(b, c, d);    e += t1;    a = t1 + t2;

	t1 = h + SHA_256_SIGMA1(e) + SHA_256_Ch(e, f, g) + 0xa2bfe8a1 + w[40];
	t2 = SHA_256_SIGMA0(a) + SHA_256_Maj(a, b, c);    d += t1;    h = t1 + t2;
	t1 = g + SHA_256_SIGMA1(d) + SHA_256_Ch(d, e, f) + 0xa81a664b + w[41];
	t2 = SHA_256_SIGMA0(h) + SHA_256_Maj(h, a, b);    c += t1;    g = t1 + t2;
	t1 = f + SHA_256_SIGMA1(c) + SHA_256_Ch(c, d, e) + 0xc24b8b70 + w[42];
	t2 = SHA_256_SIGMA0(g) + SHA_256_Maj(g, h, a);    b += t1;    f = t1 + t2;
	t1 = e + SHA_256_SIGMA1(b) + SHA_256_Ch(b, c, d) + 0xc76c51a3 + w[43];
	t2 = SHA_256_SIGMA0(f) + SHA_256_Maj(f, g, h);    a += t1;    e = t1 + t2;
	t1 = d + SHA_256_SIGMA1(a) + SHA_256_Ch(a, b, c) + 0xd192e819 + w[44];
	t2 = SHA_256_SIGMA0(e) + SHA_256_Maj(e, f, g);    h += t1;    d = t1 + t2;
	t1 = c + SHA_256_SIGMA1(h) + SHA_256_Ch(h, a, b) + 0xd6990624 + w[45];
	t2 = SHA_256_SIGMA0(d) + SHA_256_Maj(d, e, f);    g += t1;    c = t1 + t2;
	t1 = b + SHA_256_SIGMA1(g) + SHA_256_Ch(g, h, a) + 0xf40e3585 + w[46];
	t2 = SHA_256_SIGMA0(c) + SHA_256_Maj(c, d, e);    f += t1;    b = t1 + t2;
	t1 = a + SHA_256_SIGMA1(f) + SHA_256_Ch(f, g, h) + 0x106aa070 + w[47];
	t2 = SHA_256_SIGMA0(b) + SHA_256_Maj(b, c, d);    e += t1;    a = t1 + t2;

	t1 = h + SHA_256_SIGMA1(e) + SHA_256_Ch(e, f, g) + 0x19a4c116 + w[48];
	t2 = SHA_256_SIGMA0(a) + SHA_256_Maj(a, b, c);    d += t1;    h = t1 + t2;
	t1 = g + SHA_256_SIGMA1(d) + SHA_256_Ch(d, e, f) + 0x1e376c08 + w[49];
	t2 = SHA_256_SIGMA0(h) + SHA_256_Maj(h, a, b);    c += t1;    g = t1 + t2;
	t1 = f + SHA_256_SIGMA1(c) + SHA_256_Ch(c, d, e) + 0x2748774c + w[50];
	t2 = SHA_256_SIGMA0(g) + SHA_256_Maj(g, h, a);    b += t1;    f = t1 + t2;
	t1 = e + SHA_256_SIGMA1(b) + SHA_256_Ch(b, c, d) + 0x34b0bcb5 + w[51];
	t2 = SHA_256_SIGMA0(f) + SHA_256_Maj(f, g, h);    a += t1;    e = t1 + t2;
	t1 = d + SHA_256_SIGMA1(a) + SHA_256_Ch(a, b, c) + 0x391c0cb3 + w[52];
	t2 = SHA_256_SIGMA0(e) + SHA_256_Maj(e, f, g);    h += t1;    d = t1 + t2;
	t1 = c + SHA_256_SIGMA1(h) + SHA_256_Ch(h, a, b) + 0x4ed8aa4a + w[53];
	t2 = SHA_256_SIGMA0(d) + SHA_256_Maj(d, e, f);    g += t1;    c = t1 + t2;
	t1 = b + SHA_256_SIGMA1(g) + SHA_256_Ch(g, h, a) + 0x5b9cca4f + w[54];
	t2 = SHA_256_SIGMA0(c) + SHA_256_Maj(c, d, e);    f += t1;    b = t1 + t2;
	t1 = a + SHA_256_SIGMA1(f) + SHA_256_Ch(f, g, h) + 0x682e6ff3 + w[55];
	t2 = SHA_256_SIGMA0(b) + SHA_256_Maj(b, c, d);    e += t1;    a = t1 + t2;

	t1 = h + SHA_256_SIGMA1(e) + SHA_256_Ch(e, f, g) + 0x748f82ee + w[56];
	t2 = SHA_256_SIGMA0(a) + SHA_256_Maj(a, b, c);    d += t1;    h = t1 + t2;
	t1 = g + SHA_256_SIGMA1(d) + SHA_256_Ch(d, e, f) + 0x78a5636f + w[57];
	t2 = SHA_256_SIGMA0(h) + SHA_256_Maj(h, a, b);    c += t1;    g = t1 + t2;
	t1 = f + SHA_256_SIGMA1(c) + SHA_256_Ch(c, d, e) + 0x84c87814 + w[58];
	t2 = SHA_256_SIGMA0(g) + SHA_256_Maj(g, h, a);    b += t1;    f = t1 + t2;
	t1 = e + SHA_256_SIGMA1(b) + SHA_256_Ch(b, c, d) + 0x8cc70208 + w[59];
	t2 = SHA_256_SIGMA0(f) + SHA_256_Maj(f, g, h);    a += t1;    e = t1 + t2;
	t1 = d + SHA_256_SIGMA1(a) + SHA_256_Ch(a, b, c) + 0x90befffa + w[60];
	t2 = SHA_256_SIGMA0(e) + SHA_256_Maj(e, f, g);    h += t1;    d = t1 + t2;
	t1 = c + SHA_256_SIGMA1(h) + SHA_256_Ch(h, a, b) + 0xa4506ceb + w[61];
	t2 = SHA_256_SIGMA0(d) + SHA_256_Maj(d, e, f);    g += t1;    c = t1 + t2;
	t1 = b + SHA_256_SIGMA1(g) + SHA_256_Ch(g, h, a) + 0xbef9a3f7 + w[62];
	t2 = SHA_256_SIGMA0(c) + SHA_256_Maj(c, d, e);    f += t1;    b = t1 + t2;
	t1 = a + SHA_256_SIGMA1(f) + SHA_256_Ch(f, g, h) + 0xc67178f2 + w[63];
	t2 = SHA_256_SIGMA0(b) + SHA_256_Maj(b, c, d);    e += t1;    a = t1 + t2;

	outdexs[0] += a;
	outdexs[1] += b;
	outdexs[2] += c;
	outdexs[3] += d;
	outdexs[4] += e;
	outdexs[5] += f;
	outdexs[6] += g;
	outdexs[7] += h;
}

void SHA256_Init(PSHA256_CONTEXT init_tgt)
{
	SHA_ULONG i;

	init_tgt->residue = 0;
	init_tgt->inces = 0;

	init_tgt->outdexs[0] = 0x6a09e667;
	init_tgt->outdexs[1] = 0xbb67ae85;
	init_tgt->outdexs[2] = 0x3c6ef372;
	init_tgt->outdexs[3] = 0xa54ff53a;
	init_tgt->outdexs[4] = 0x510e527f;
	init_tgt->outdexs[5] = 0x9b05688c;
	init_tgt->outdexs[6] = 0x1f83d9ab;
	init_tgt->outdexs[7] = 0x5be0cd19;

}

void SHA256_Update(PSHA256_CONTEXT cnxt, const SHA_BYTE *target, SHA_ULONG size)
{
	SHA_ULONG i;

	cnxt->inces += size << 3;

	if (cnxt->inces < 0)
		cnxt->residue++;

	cnxt->residue += size >> 29;

	while (size >= 64) {
		memcpy((SHA_PULONG)cnxt->buffer, target, (size_t)size);
		SHA256_Mixing(target, cnxt->outdexs);
		target += 64;
		size -= 64;
	}

	memcpy((SHA_PULONG)cnxt->buffer, target, size);
}

void SHA256_Final(PSHA256_CONTEXT cnxt, SHA_BYTE *bytes)
{
	SHA_ULONG i, j, index, len;

	index = (cnxt->inces >> 3) % 64;
	cnxt->buffer[index++] = 0x80;

	if (index > 56) {
		memset((SHA_ULONG)cnxt->buffer + index, 0, 64 - index);
		SHA256_Mixing((SHA_PULONG)cnxt->buffer, cnxt->outdexs);
		memset((SHA_ULONG)cnxt->buffer, 0, 56);
	}
	else
		memset((SHA_ULONG)cnxt->buffer + index, 0, 56 - index);

	cnxt->buffer[14] = CONVERT_TO_LITTLE_ENDIAN(cnxt->residue);
	cnxt->buffer[15] = CONVERT_TO_LITTLE_ENDIAN(cnxt->inces);

	SHA256_Mixing((SHA_PULONG)cnxt->buffer, cnxt->outdexs);

	for (i = 0; i < 32; i += 4)
		*(SHA_ULONG *)(&bytes[i]) = CONVERT_TO_LITTLE_ENDIAN((SHA_ULONG)(cnxt->outdexs[i / 4]));
}

void SHA256(const SHA_BYTE *target, SHA_ULONG len, SHA_BYTE *source)
{
	static SHA256_CONTEXT cnxt;

	SHA256_Init(&cnxt);
	SHA256_Update(&cnxt, target, len);
	SHA256_Final(&cnxt, source);
}
