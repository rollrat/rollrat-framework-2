//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  BigInteger.h - 01/21/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _BIGINTEGER_9bf1541fdf7efd41b7b39543fd870ac4_
#define _BIGINTEGER_9bf1541fdf7efd41b7b39543fd870ac4_

#include "rollrat-framework/BigInteger/BigBase.h"

namespace ofw {

class BigInteger {
  typedef BigInteger this_type;

  BigBase base;
  int _sign = 0;

  explicit BigInteger(BigBase v, int s);

 public:
  BigInteger();
  BigInteger(uint32_t uiptr);
  BigInteger(int32_t iptr);
  BigInteger(const this_type& refer);
  BigInteger(const char* str);
  BigInteger(const std::string str);
  ~BigInteger();

  this_type& operator=(const this_type& integer);

  this_type& operator<<=(uint32_t len);
  this_type operator<<(uint32_t len) const;
  this_type& operator>>=(uint32_t len);
  this_type operator>>(uint32_t len) const;

  this_type& operator++();
  this_type operator++(int);
  this_type& operator--();
  this_type operator--(int);
  this_type& operator+=(const this_type& integer);
  this_type operator+(const this_type& integer) const;
  this_type& operator-=(const this_type& integer);
  this_type operator-(const this_type& integer) const;
  this_type& operator*=(const this_type& integer);
  this_type operator*(const this_type& integer) const;
  this_type& operator%=(const this_type& integer);
  this_type operator%(const this_type& integer) const;
  this_type& operator/=(const this_type& integer);
  this_type operator/(const this_type& integer) const;

  bool operator==(const this_type& integer) const;
  bool operator!=(const this_type& integer) const;
  bool operator<(const this_type& integer) const;
  bool operator>(const this_type& integer) const;
  bool operator<=(const this_type& integer) const;
  bool operator>=(const this_type& integer) const;

  this_type gcd(const this_type& integer) const;
  this_type lcm(const this_type& integer) const;

  std::string to_string() const;
  std::string to_hex() const;
  std::string to_binary() const;
  std::string operator*() const;

  bool sign() const;
  void swap(this_type& refer);

  this_type abs() const;
  size_t length() const;

  friend std::ostream& operator<<(std::ostream& os, const BigInteger& refer) {
    os << *refer;
    return os;
  }
};

inline BigInteger operator"" _i(const char* str, size_t length) {
  return BigInteger{str};
}

inline BigInteger operator"" _i(unsigned long long value) {
  return BigInteger{(uint32_t)value};
}

}  // namespace ofw

#endif