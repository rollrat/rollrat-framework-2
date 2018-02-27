//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  BigInteger.cpp - 01/21/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#include "BigInteger/BigInteger.h"

using namespace ofw;

BigInteger::BigInteger(BigBase v, int s) : base(v), _sign(s) {}

BigInteger::BigInteger() {}

BigInteger::BigInteger(uint32_t uiptr) : base(uiptr) { _sign = !base.iszero(); }

BigInteger::BigInteger(int32_t iptr) {
  if (iptr > 0) {
    base = iptr;
    _sign = 1;
  } else if (iptr < 0) {
    base = -iptr;
    _sign = -1;
  }
}

BigInteger::BigInteger(const this_type& refer)
    : base(refer.base), _sign(refer._sign) {}

BigInteger::BigInteger(const char* str) : BigInteger(std::string(str)) {}

BigInteger::BigInteger(const std::string str) {
  if (str[0] == 'x') {
    base.aly_hex(str);
    _sign = 1;
  } else {
    if (str[0] == '-') {
      base.aly_dec(str.substr(1));
      _sign = -1;
    } else {
      base.aly_dec(str);
      if (base.iszero())
        _sign = 0;
      else
        _sign = 1;
    }
  }
}

BigInteger::~BigInteger() {}

BigInteger::this_type& BigInteger::operator=(const this_type& integer) {
  base = integer.base;
  _sign = integer._sign;
  return *this;
}

BigInteger::this_type& BigInteger::operator<<=(uint32_t len) {
  base <<= len;
  return *this;
}

BigInteger::this_type BigInteger::operator<<(uint32_t len) const {
  this_type tmp = *this;
  return tmp <<= len;
}

BigInteger::this_type& BigInteger::operator>>=(uint32_t len) {
  base >>= len;
  return *this;
}

BigInteger::this_type BigInteger::operator>>(uint32_t len) const {
  this_type tmp = *this;
  return tmp >>= len;
}

BigInteger::this_type& BigInteger::operator++() {
  if (_sign >= 0) {
    ++base;
    _sign = 1;
  } else {
    --base;
    if (base.iszero()) _sign = 0;
  }
  return *this;
}

BigInteger::this_type BigInteger::operator++(int) {
  this_type tmp = *this;
  ++*this;
  return tmp;
}

BigInteger::this_type& BigInteger::operator--() {
  if (_sign > 0) {
    --base;
    if (base.iszero()) _sign = 0;
  } else if (_sign <= 0) {
    ++base;
    _sign = -1;
  }
  return *this;
}

BigInteger::this_type BigInteger::operator--(int) {
  this_type tmp = *this;
  --*this;
  return tmp;
}

BigInteger::this_type& BigInteger::operator+=(const this_type& integer) {
  if (_sign == integer._sign) {
    base += integer.base;
  } else if (_sign == 0 || integer._sign == 0) {
    base += integer.base;
    _sign = _sign + integer._sign;
  } else if (_sign < 0) {
    if (base > integer.base)
      base -= integer.base;
    else {
      base = integer.base - base;
      _sign = 1;
    }
  } else {
    if (base > integer.base)
      base -= integer.base;
    else {
      base = integer.base - base;
      _sign = -1;
    }
  }
  return *this;
}

BigInteger::this_type BigInteger::operator+(const this_type& integer) const {
  this_type tmp = *this;
  return tmp += integer;
}

BigInteger::this_type& BigInteger::operator-=(const this_type& integer) {
  if (_sign == integer._sign) {
    if (_sign > 0) {
      if (base > integer.base)
        base -= integer.base;
      else {
        base = integer.base - base;
        if (!base.iszero())
          _sign = -1;
        else
          _sign = 0;
      }
    } else if (_sign < 0) {
      if (base > integer.base)
        base -= integer.base;
      else {
        base = integer.base - base;
        if (!base.iszero())
          _sign = 1;
        else
          _sign = 0;
      }
    }
  } else if (_sign == 0) {
    base = integer.base;
    _sign = -integer._sign;
  } else if (_sign > 0 || _sign < 0) {
    base += integer.base;
  }
  return *this;
}

BigInteger::this_type BigInteger::operator-(const this_type& integer) const {
  this_type tmp = *this;
  return tmp -= integer;
}

BigInteger::this_type& BigInteger::operator*=(const this_type& integer) {
  _sign *= integer._sign;
  base *= integer.base;
  return *this;
}

BigInteger::this_type BigInteger::operator*(const this_type& integer) const {
  this_type tmp = *this;
  return tmp *= integer;
}

BigInteger::this_type& BigInteger::operator%=(const this_type& integer) {
  _sign *= integer._sign;
  base %= integer.base;
  return *this;
}

BigInteger::this_type BigInteger::operator%(const this_type& integer) const {
  this_type tmp = *this;
  return tmp %= integer;
}

BigInteger::this_type& BigInteger::operator/=(const this_type& integer) {
  _sign *= integer._sign;
  base /= integer.base;
  return *this;
}

BigInteger::this_type BigInteger::operator/(const this_type& integer) const {
  this_type tmp = *this;
  return tmp /= integer;
}

bool BigInteger::operator==(const this_type& integer) const {
  if (_sign == integer._sign) return base == integer.base;
  return false;
}

bool BigInteger::operator!=(const this_type& integer) const {
  return !(*this == integer);
}

bool BigInteger::operator<(const this_type& integer) const {
  if (*this != integer) {
    if (_sign != integer._sign) {
      if (_sign > 0)
        return false;
      else
        return true;
    } else {
      if (_sign > 0)
        return base < integer.base;
      else
        return base > integer.base;
    }
  }
  return false;
}

bool BigInteger::operator>(const this_type& integer) const {
  return integer < *this;
}

bool BigInteger::operator<=(const this_type& integer) const {
  return !(integer < *this);
}

bool BigInteger::operator>=(const this_type& integer) const {
  return !(*this < integer);
}

BigInteger::this_type BigInteger::gcd(const this_type& integer) const {
  return this_type(base.gcd(integer.base), 1);
}

BigInteger::this_type BigInteger::lcm(const this_type& integer) const {
  return *this * integer / gcd(integer);
}

std::string BigInteger::to_string() const { return base.to_string(); }

std::string BigInteger::to_hex() const { return base.to_hex(); }

std::string BigInteger::to_binary() const { return base.to_binary(); }

std::string BigInteger::operator*() const { return base.to_string(); }

bool BigInteger::sign() const { return _sign < 0; }

void BigInteger::swap(this_type& refer) {
  base.swap(refer.base);
  int tmp = _sign;
  _sign = refer._sign;
  refer._sign = tmp;
}

BigInteger::this_type BigInteger::abs() const {
  this_type tmp = *this;
  if (_sign < 0) tmp._sign = 1;
  return tmp;
}

size_t BigInteger::length() const { return base._length(); }