//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  fraction.h - 02/13/2016
//
//  Copyright (C) 2016-2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _FRACTION_9bf1541fdf7efd41b7b39543fd870ac4_
#define _FRACTION_9bf1541fdf7efd41b7b39543fd870ac4_

#include "biginteger.h"

template <int I>
class basic_fraction {
  typedef basic_fraction<I> this_type;
  typedef basic_integer<I> integer_operation;
  mutable integer_operation high;
  mutable integer_operation low;

 public:
  basic_fraction() : high(0), low(1) {}

  basic_fraction(std::string wstr) : high(wstr), low(1) {}

  basic_fraction(std::string wstr1, std::string wstr2)
      : high(wstr1), low(wstr2) {
    reduction();
  }

  basic_fraction(pointer_type uip1, pointer_type uip2 = 1)
      : high(uip1), low(uip2) {}

  basic_fraction(integer_type ipt1, integer_type ipt2 = 1)
      : high(ipt1), low(ipt2) {}

  this_type& operator=(std::string wstr) {
    high = wstr;
    low = 1;
    return *this;
  }

  this_type& operator=(const this_type& refer) {
    high = refer.high;
    low = refer.low;
    return *this;
  }

  this_type& operator*=(const this_type& refer) {
    integer_operation hit = refer.high * high;
    integer_operation lot = refer.low * low;

    integer_operation gcd = hit.abs().gcd(lot.abs());

    high = hit / gcd;
    low = lot / gcd;

    return *this;
  }

  this_type operator*(const this_type& refer) const {
    this_type tmp = *this;
    return tmp *= refer;
  }

  this_type& operator/=(const this_type& refer) {
    integer_operation hit = refer.low * high;
    integer_operation lot = refer.high * low;

    integer_operation gcd = hit.abs().gcd(lot.abs());

    high = hit / gcd;
    low = lot / gcd;

    return *this;
  }

  this_type operator/(const this_type& refer) const {
    this_type tmp = *this;
    return tmp /= refer;
  }

  this_type& operator+=(const this_type& refer) {
    integer_operation lcm = low.abs().lcm(refer.low.abs());

    high = high * lcm / low + refer.high * lcm / refer.low;
    low = lcm;

    return *this;
  }

  this_type operator+(const this_type& refer) const {
    this_type tmp = *this;
    return tmp += refer;
  }

  this_type& operator-=(const this_type& refer) {
    integer_operation lcm = low.abs().lcm(refer.low.abs());

    high = high * lcm / low - refer.high * lcm / refer.low;
    low = lcm;

    return *this;
  }

  this_type operator-(const this_type& refer) const {
    this_type tmp = *this;
    return tmp -= refer;
  }

  bool operator==(const this_type& refer) const {
    return refer.high == high && refer.low == low;
  }

  bool operator!=(const this_type& refer) const { return !(*this == integer); }

  bool operator<(const this_type& refer) const {
    integer_operation lcm = low.abs().lcm(refer.low.abs());

    return (high * lcm / low) < (refer.high * lcm / refer.low);
  }

  bool operator>(const this_type& refer) const { return refer < *this; }

  bool operator<=(const this_type& refer) const { return !(refer < *this); }

  bool operator>=(const this_type& refer) const { return !(*this < integer); }

  void reverse() const { high.swap(low); }

  void reduction() const {
    integer_operation gcd = high.abs().gcd(low.abs());
    high /= gcd;
    low /= gcd;
  }

  std::string operator*() const {
    std::string build;

    if (high.sign() != low.sign()) build.append("-");

    reduction();

    build.append(*high.abs());

    if (low != "1") {
      build.append("/");
      build.append(*low.abs());
    }

    return build;
  }

  std::string high_part() const { return *high; }

  std::string low_part() const { return *low; }

  bool sign() const {
    if (high.sign() != low.sign())
      return true;
    else
      return false;
  }

  friend std::ostream& operator<<(std::ostream& os, const this_type& refer) {
    os << *refer;
    return os;
  }
};

typedef basic_fraction<256> fraction;

inline fraction operator"" fr(const char* str, size_t length) {
  const char* delimeter = strchr(str, '/');
  if (delimeter) {
    char buffer[256];
    memcpy(buffer, str, delimeter - str);
    buffer[delimeter - str] = 0;

    return fraction{std::string(buffer), std::string(delimeter + 1)};
  } else {
    const char* delimeter = strchr(str, '.');

    if (delimeter) {
      char buffer_high[256];
      char buffer_low[256];
      int lowlen = length - (delimeter - str);

      buffer_low[0] = '1';
      memset(buffer_low + 1, '0', lowlen - 1);
      buffer_low[lowlen] = 0;

      memcpy(buffer_high, str, delimeter - str);
      memcpy(buffer_high + (delimeter - str), delimeter + 1, lowlen + 1);

      return fraction{std::string(buffer_high), std::string(buffer_low)};
    } else {
      return fraction{std::string(str)};
    }
  }
}

inline fraction operator"" fr(unsigned long long i) {
  return fraction{std::to_string(i)};
}

#endif