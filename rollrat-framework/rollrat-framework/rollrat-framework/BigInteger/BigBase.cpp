//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  BigBase.cpp - 01/21/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#include "BigBase.h"

using namespace ofw;

BigBase::BigBase()
{
}

BigBase::BigBase(uint32_t uiptr)
{
  if (uiptr > 0)
  {
    allocate(1);
    blocks[0] = uiptr;
  }
}

BigBase::BigBase(const this_type& refer)
{
  copy(refer);
}

BigBase::BigBase(const char* str)
  : BigBase(std::string(str))
{
}

BigBase::BigBase(const std::string str)
{
  if (str[0] == 'x' /*|| str[0] == 'o'*/)
  {
    init_hex(str);
  }
  else
  {
    init_decimal(str);
  }
}

void BigBase::aly_hex(const std::string str)
{
  init_hex(str);
}

void BigBase::aly_dec(const std::string str)
{
  init_decimal(str);
}

BigBase::this_type& BigBase::operator=(const this_type& integer)
{
  copy(integer);
  return *this;
}

BigBase::this_type& BigBase::operator<<=(uint32_t len)
{
  if (length == 0)
    return *this;

  uint32_t shift = len / 32;
  uint32_t modul = len % 32;
  uint32_t modur = 32 - modul;

  reallocate(length + shift + (modul > 0));

  if (shift > 0)
  {
    if (modur == 0 || modul == 0)
    {
      for (int i = length - 1; i >= shift; i--)
      {
        blocks[i] = blocks[i - shift];
      }
      for (int i = 0; i < shift; i++)
      {
        blocks[i] = 0;
      }
    }
    else
    {
      for (int i = length - 1; i > shift; i--)
      {
        blocks[i] = (blocks[i - shift] << modul) | (blocks[i - shift - 1] >> modur);
      }
      blocks[shift] = blocks[0] << modul;
      for (int i = 0; i < shift; i++)
      {
        blocks[i] = 0;
      }
    }
  }
  else
  {
    for (int i = length - 1; i > 0; i--)
    {
      blocks[i] = (blocks[i] << modul) | (blocks[i - 1] >> modur);
    }
    blocks[0] <<= modul;
  }

  trim();

  return *this;
}

BigBase::this_type BigBase::operator<<(uint32_t len) const
{
  this_type tmp = *this;
  return tmp <<= len;
}

BigBase::this_type& BigBase::operator>>=(uint32_t len)
{
  if (length == 0)
    return *this;

  uint32_t shift = len / 32;
  uint32_t modur = len % 32;
  uint32_t modul = 32 - modur;

  if (shift > 0 && shift <= length)
  {
    if (modur == 0 || modul == 0)
    {
      for (int i = 0; i < length - shift; i++)
      {
        blocks[i] = blocks[i + shift];
      }
      for (int i = length - 1, j = 0; j < shift; i--, j++)
      {
        blocks[i] = 0;
      }
    }
    else
    {
      for (int i = 0; i < length - shift - 1; i++)
      {
        blocks[i] = (blocks[i + shift] >> modur) | (blocks[i + shift + 1] << modul);
      }
      blocks[length - shift - 1] = blocks[length - 1] >> modur;
      for (int i = length - 1, j = 0; j < shift; i--, j++)
      {
        blocks[i] = 0;
      }
      blocks[shift] >>= modur;
    }
  }
  else
  {
    for (int i = 0; i < length - 1; i++)
    {
      blocks[i] = (blocks[i] >> modur) | (blocks[i + 1] << modul);
    }
    blocks[length - 1] >>= modur;
  }

  trim();

  return *this;
}


BigBase::this_type BigBase::operator>>(uint32_t len) const
{
  this_type tmp = *this;
  return tmp >>= len;
}

BigBase::this_type& BigBase::operator++()
{
  if (length == 0)
  {
    allocate(1);
    blocks[0] = 1;
    return *this;
  }

  uint32_t carry = 0;

  uint64_t tmp = (uint64_t)(blocks[0] + 1);
  blocks[0] = (uint32_t)(tmp);
  carry = tmp >> 32;

  for (int i = 1; carry && i < length; i++)
  {
    uint64_t tmp = (uint64_t)blocks[i] + carry;
    blocks[i] = (uint32_t)(tmp);
    carry = tmp >> 32;
  }

  if (carry)
  {
    reallocate(length + 1);
    blocks[length - 1] = 1;
  }

  trim();

  return *this;
}

BigBase::this_type& BigBase::operator--()
{
  if (length == 0)
  {
    init();
    return *this;
  }

  uint32_t borrow = 0;

  uint64_t tmp = (uint64_t)(blocks[0] - 1);
  blocks[0] = (uint32_t)(tmp);
  borrow = (uint32_t)(tmp >> 32) & 1;

  for (int i = 0; i < length && borrow; i++)
  {
    uint64_t tmp = (uint64_t)blocks[i] - borrow;
    blocks[i] = (uint32_t)(tmp);
    borrow = (uint32_t)(tmp >> 32) & 1;
  }

  if (borrow)
  {
    init();
  }
  else
  {
    trim();
  }

  return *this;
}

BigBase::this_type BigBase::operator++(int)
{
  this_type tmp = *this;
  ++*this;
  return tmp;
}

BigBase::this_type BigBase::operator--(int)
{
  this_type tmp = *this;
  --*this;
  return tmp;
}

BigBase::this_type& BigBase::operator+=(const this_type& integer)
{
  if (length >= integer.length && length > 0)
  {
    uint32_t carry = 0;
    int i = 0;

    for (; i < integer.length; i++)
    {
      uint64_t tmp = (uint64_t)(blocks[i]) + 
        (uint64_t)(integer.blocks[i]) + carry;
      blocks[i] = (uint32_t)(tmp);
      carry = tmp >> 32;
    }

    for (; carry && i < length; i++)
    {
      uint64_t tmp = (uint64_t)blocks[i] + carry;
      blocks[i] = (uint32_t)(tmp);
      carry = tmp >> 32;
    }

    if (carry)
    {
      reallocate(length + 1);
      blocks[length - 1] = 1;
    }

    trim();

    return *this;
  }
  else if (integer.length > 0)
  {
    *this = integer + *this;
    return *this;
  }
  else
  {
    return *this;
  }
}

BigBase::this_type& BigBase::operator-=(const this_type& integer)
{
  if (length >= integer.length && length > 0)
  {
    uint32_t borrow = 0;
    int i = 0;

    for (; i < integer.length; i++)
    {
      uint64_t tmp = (uint64_t)(blocks[i]) - 
        (uint64_t)(integer.blocks[i]) - borrow;
      blocks[i] = (uint32_t)(tmp);
      borrow = (uint32_t)(tmp >> 32) & 1;
    }

    for (; i < length && borrow; i++)
    {
      uint64_t tmp = (uint64_t)blocks[i] - borrow;
      blocks[i] = (uint32_t)(tmp);
      borrow = (uint32_t)(tmp >> 32) & 1;
    }

    if (borrow)
    {
      init();
    }
    else
    {
      trim();
    }

    return *this;
  }
  else
  {
    init();
    return *this;
  }
}

BigBase::this_type BigBase::operator+(const this_type& integer) const
{
  this_type tmp = *this;
  return tmp += integer;
}

BigBase::this_type BigBase::operator-(const this_type& integer) const
{
  this_type tmp = *this;
  return tmp -= integer;
}

BigBase::this_type& BigBase::operator*=(const this_type& integer)
{
  size_t   len = (length + 1) * (integer.length + 1);
  uint32_t *u = new uint32_t[len];
  uint32_t carry = 0;

  memset(u, 0, sizeof(uint32_t) * len);

  for (int i = 0; i < length; i++)
  {
    int k = i;
    for (int j = 0; j < integer.length; j++, k++)
    {
      uint64_t l = (uint64_t)(blocks[i]) * 
        (uint64_t)(integer.blocks[j]) + carry + u[k];
      u[k] = (uint32_t)l;
      carry = (uint32_t)(l >> 32);
    }
    for (; carry > 0; k++)
    {
      uint64_t l = (uint64_t)(u[k]) + carry;
      u[k] = (uint32_t)(l);
      carry = l >> 32;
    }
  }

  safe_allocate(len);
  memcpy(blocks, u, sizeof(uint32_t) * len);

  delete[] u;

  length = len;
  trim();

  return *this;
}

BigBase::this_type& BigBase::operator%=(const this_type& integer)
{
  divideWithRemainder(integer, false);
  trim();
  return *this;
}

BigBase::this_type& BigBase::operator/=(const this_type& integer)
{
  uint32_t *qutient = new uint32_t[length];
  memset(qutient, 0, sizeof(uint32_t) * length);
  divideWithRemainder(integer, true, qutient);
  memcpy(blocks, qutient, sizeof(uint32_t) * length);
  delete[] qutient;
  trim();
  return *this;
}

BigBase::this_type BigBase::operator*(const this_type& integer) const
{
  this_type tmp = *this;
  return tmp *= integer;
}

BigBase::this_type BigBase::operator%(const this_type& integer) const
{
  this_type tmp = *this;
  return tmp %= integer;
}

BigBase::this_type BigBase::operator/(const this_type& integer) const
{
  this_type tmp = *this;
  return tmp /= integer;
}

bool BigBase::operator==(const this_type& integer) const
{
  if (length == integer.length)
    return !memcmp(blocks, integer.blocks, sizeof(uint32_t) * length);
  else
    return false;
}

bool BigBase::operator!=(const this_type& integer) const
{
  return !(*this == integer);
}

bool BigBase::operator<(const this_type& integer) const
{
  if (length > integer.length)
    return false;
  else if (length < integer.length)
    return true;
  else if (length == 0 && integer.length == 0)
    return false;
  else {
    for (size_t ptr = length; --ptr >= 0; )
      if (blocks[ptr] < integer.blocks[ptr]) return true;
    return false;
  }
}

bool BigBase::operator>(const this_type& integer) const
{
  return integer < *this;
}

bool BigBase::operator<=(const this_type& integer) const
{
  return !(integer < *this);
}

bool BigBase::operator>=(const this_type& integer) const
{
  return !(*this < integer);
}

bool BigBase::iszero() const
{
  return length == 0;
}

BigBase::this_type BigBase::gcd(const this_type& integer) const
{
  this_type tmp1;
  this_type tmp2;
  this_type tmp3;
  if (*this < integer)
  {
    tmp1 = integer;
    tmp2 = *this;
  }
  else
  {
    tmp1 = *this;
    tmp2 = integer;
  }
  while (!tmp2.iszero())
  {
    if (tmp1 == tmp2)
      break;
    tmp3 = tmp1 % tmp2;
    tmp1 = tmp2;
    tmp2 = tmp3;
  }
  return tmp1;
}

BigBase::this_type BigBase::lcm(const this_type& integer) const
{
  return *this * integer / gcd(integer);
}

std::string BigBase::to_string() const
{
  std::string tmp;

  const uint32_t base = 1000000000;
  const int chbase = 9;

  int src = length;
  int max = length * 10 / 9 + 2;

  if (src == 1)
  {
    return std::to_string(blocks[0]);
  }
  else if (src == 0)
  {
    return "0";
  }

  uint32_t *ddst = new uint32_t[max];
  memset(ddst, 0, max * sizeof(uint32_t));
  int cdst = 0;

  for (int i = src; --i >= 0; )
  {
    uint32_t carry = blocks[i];
    for (int j = 0; j < cdst; j++)
    {
      uint64_t res = ((uint64_t)(ddst[j]) << 32) | carry;
      ddst[j] = (uint32_t)(res % base);
      carry = (uint32_t)(res / base);
    }
    if (carry != 0)
    {
      ddst[cdst++] = carry % base;
      carry /= base;
      if (carry != 0)
        ddst[cdst++] = carry;
    }
  }

  int cmax = cdst * chbase;
  char *str = new char[cmax + 1];
  int idst = cmax;


  for (int i = 0; i < cdst - 1; i++)
  {
    uint32_t d = ddst[i];
    for (int j = chbase; --j >= 0; )
    {
      str[--idst] = (char)('0' + d % 10);
      d /= 10;
    }
  }
  for (int d = ddst[cdst - 1]; d != 0; )
  {
    str[--idst] = (char)('0' + d % 10);
    d /= 10;
  }

  str[cmax] = 0;
  str += idst;

  return std::string(str);
}

std::string BigBase::to_hex() const
{
  int decmax = (size_t)(0.30102999566398119521373889472449 * 
    (long double)(length * 32)) + 1;

  char *tmp = new char[decmax];
  char *pp = tmp;

  for (int i = 0; i < length; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      int bc = (this->blocks[i] >> (j * 4)) & 0xf;
      if (bc < 10)
        *pp++ = bc + '0';
      else
        *pp++ = bc + 'a' - 10;
    }
  }

  if (pp != tmp)
  {
    while (*--pp == '0');
    pp++;
  }

  *pp = 0;

  return std::string(_strrev(tmp));
}

std::string BigBase::to_binary() const
{
  char *tmp = new char[length * 32 + 1];
  char *pp = tmp;

  for (int i = 0; i < length; i++)
  {
    for (int j = 0; j < 32; j++)
    {
      *pp++ = ((this->blocks[i] >> j) & 1) + '0';
    }
  }

  if (pp != tmp)
  {
    while (*--pp == '0');
    pp++;
  }

  *pp = 0;

  return std::string(_strrev(tmp));
}

void BigBase::swap(this_type& t)
{
  uint32_t *tmp = blocks;
  blocks = t.blocks;
  t.blocks = tmp;
  size_t len = length;
  length = t.length;
  t.length = len;
}

size_t BigBase::_length() const
{
  return length;
}

inline uint32_t BigBase::div_shift(uint32_t v, int c, bool inv)
{
  if (inv == false)
  {
    if (c > 0)
      return v << c;
    else
      return v >> (-c);
  }
  else
  {
    if (c > 0)
      return v >> (32 - c);
    else
      return v << (32 + c);
  }
}

inline uint32_t BigBase::div_multiget(const uint32_t* v, size_t s, int sh)
{
  uint32_t t = v[s] << sh;
  if (s > 0 && sh > 0)
  {
    t |= v[s - 1] >> (32 - sh);
  }
  return t;
}

void BigBase::divideWithRemainder(const this_type& integer, 
  bool quotient, uint32_t *quotient_array)
{
  if (length < integer.length)
  {
    if (quotient)
      memset(blocks, 0, sizeof(uint32_t) * length);
  }
  else
  {
    int l1 = length - 1;
    int l2 = integer.length - 1;

    int c1;
    int c2 = count_leading_zero(integer.blocks[l2]);

    uint32_t t2 = div_multiget(integer.blocks, l2, c2);

    for (int i = l1; i >= 0 && i >= l2; i--)
    {
      if (blocks[i] == 0) continue;

      c1 = count_leading_zero(blocks[i]);

      uint32_t t1 = div_multiget(blocks, i, c1);
      int c_gap = c2 - c1;

      if ((i == l2) && ((c_gap < 0) || (c_gap == 0 && t1 < t2)))
        continue;

      if (t1 == t2)
      {
        for (int j = l2 - 1; j >= 0; j--)
        {
          uint32_t t1_1 = div_multiget(blocks, i + j - l2, c1);
          uint32_t t2_1 = div_multiget(integer.blocks, j, c2);
          if (t1_1 != t2_1)
          {
            c_gap -= t1_1 < t2_1;
            break;
          }
        }
      }
      else if (t1 < t2)
      {
        c_gap -= 1;
      }

      uint32_t borrow = 0;
      int g = c_gap < 0;

      for (int j = i - l2 - g, k = 0; j <= l1 && k <= (l2 + g); j++, k++)
      {
        uint32_t s = 0;
        if (k <= l2)
          s = div_shift(integer.blocks[k], c_gap, g);
        if (k > 0 && c_gap != 0 && c_gap != -32)
          s |= div_shift(integer.blocks[k - 1], c_gap, c_gap > 0);
        uint64_t tmp = (uint64_t)(blocks[j]) - (uint64_t)(s)-borrow;
        blocks[j] = (uint32_t)(tmp);
        borrow = (uint32_t)(tmp >> 32) & 1;
      }

      if (quotient)
      {
        if (c_gap < 0)
          quotient_array[i - l2 - 1] |= 1 << (32 + c_gap);
        else
          quotient_array[i - l2] |= 1 << c_gap;
      }

      if (blocks[i] > 0)
        i++;
    }
  }
}

void BigBase::init()
{
  if (blocks != nullptr)
    delete[] blocks;
  blocks = nullptr;
  length = 0;
}

void BigBase::allocate(size_t i)
{
  blocks = new uint32_t[i];
  length = i;
}

void BigBase::reallocate(size_t i)
{
  uint32_t *tmp = new uint32_t[i];
  memset(tmp, 0, i * sizeof(uint32_t));
  memcpy(tmp, blocks, length * sizeof(uint32_t));
  delete[] blocks;
  blocks = tmp;
  length = i;
}

void BigBase::safe_allocate(size_t i)
{
  if (blocks != nullptr)
    delete[] blocks;
  blocks = new uint32_t[i];
  length = i;
}

void BigBase::copy(const this_type& g)
{
  init();
  if (g.length > 0)
  {
    allocate(g.length);
    memcpy(blocks, g.blocks, sizeof(uint32_t)*g.length);
  }
}

void BigBase::init_hex(std::string hex)
{
  size_t len = hex.length() - 1;

  allocate(len / 8 + 1);
  memset(blocks, 0, sizeof(uint32_t) * length);

  for (int i = 0, j = (int)::ceilf((float)(len) / 8); i < j; i++)
  {
    int lc = 8;

    if (i == j - 1 && len % 8)
      lc = len % 8;

    for (int k = 0; k < lc; k++)
    {
      int s, l = len - i * 8 - k;
      if (isdigit(hex[l]))
        s = hex[l] - '0';
      else
        s = tolower(hex[l]) - 'a' + 10;
      blocks[i] |= s << (k * 4);
    }
  }

  trim();
}

void BigBase::init_decimal(std::string dec)
{
  std::string nstr = dec;

  size_t len = nstr.length();
  size_t vlen = len * 4;

  allocate(vlen);
  memset(blocks, 0, sizeof(uint32_t) * vlen);

  uint8_t *bcd = new uint8_t[len];
  memset(bcd, 0, sizeof(uint8_t) * len);

  for (int i = len, j = 0; --i >= 0; j++)
  {
    bcd[j] = nstr[i] & 15;
  }

  for (int i = 0; i < vlen; i++)
  {
    for (int j = 0; j < vlen - 1; j++)
    {
      blocks[j] = (blocks[j] >> 1) | (blocks[j + 1] << 31);
    }
    blocks[vlen - 1] >>= 1;
    blocks[vlen - 1] |= bcd[0] << 31;

    for (int j = 0; j < len - 1; j++)
    {
      bcd[j] = (bcd[j] >> 1) | ((bcd[j + 1] << 3) & 8);
    }
    bcd[len - 1] >>= 1;

    for (int j = len; --j >= 0; )
    {
      if (bcd[j] >= 8)
        bcd[j] -= 3;
    }
  }

  delete[] bcd;

  length = vlen;

  *this >>= vlen * 32 - vlen;
}

void BigBase::trim()
{
  int i = length - 1;
  for (; i >= 0; i--)
    if (blocks[i] != 0)
    {
      break;
    }
  if (i >= 0)
    length = i + 1;
  else
  {
    length = 0;
    delete[] blocks;
  }
}

int BigBase::count_leading_zero(uint32_t val) const
{
  if (val == 0) return 32;
  int n = 0;
  if ((val & 0xffff0000) == 0) { n += 16; val <<= 16; }
  if ((val & 0xff000000) == 0) { n += 8; val <<= 8; }
  if ((val & 0xf0000000) == 0) { n += 4; val <<= 4; }
  if ((val & 0xc0000000) == 0) { n += 2; val <<= 2; }
  if ((val & 0x80000000) == 0) { n += 1; }
  return n;
}

int BigBase::count_tailing_zero(uint32_t val) const
{
  if (val == 0) return 0;
  int n = 32;
  if ((val & 0x0000ffff) == 0) { n -= 16; val >>= 16; }
  if ((val & 0x000000ff) == 0) { n -= 8; val >>= 8; }
  if ((val & 0x0000000f) == 0) { n -= 4; val >>= 4; }
  if ((val & 0x00000003) == 0) { n -= 2; val >>= 2; }
  if ((val & 0x00000001) == 0) { n -= 1; }
  return n;
}