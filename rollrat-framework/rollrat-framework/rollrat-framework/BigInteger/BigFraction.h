//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  BigFraction.h - 01/21/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _BIGFRACTION_
#define _BIGFRACTION_

#include "BigInteger.h"

namespace ofw
{

class BigFraction
{
  typedef BigFraction this_type;
  typedef BigInteger integer_operation;

  mutable integer_operation high;
  mutable integer_operation low;

public:

  BigFraction();
  BigFraction(std::string wstr);
  explicit BigFraction(integer_operation io);
  BigFraction(std::string wstr1, std::string wstr2);
  BigFraction(uint32_t uip1, uint32_t uip2 = 1);

  this_type& operator=(std::string wstr);
  this_type& operator=(const this_type& refer);
  this_type& operator*=(const this_type& refer);
  this_type operator*(const this_type& refer) const;
  this_type& operator/=(const this_type& refer);
  this_type operator/(const this_type& refer) const;
  this_type& operator+=(const this_type& refer);
  this_type operator+(const this_type& refer) const;
  this_type& operator-=(const this_type& refer);
  this_type operator-(const this_type& refer) const;

  bool operator==(const this_type& refer) const;
  bool operator!=(const this_type& refer) const;
  bool operator<(const this_type& refer) const;
  bool operator>(const this_type& refer) const;
  bool operator<=(const this_type& refer) const;
  bool operator>=(const this_type& refer) const;

  void reverse() const;
  void reduction() const;

  std::string operator*() const;

  std::string high_part() const;
  std::string low_part() const;

  bool sign() const;
  void cutting(int size);

  std::string fraction_point(size_t valid_point = 10) const;

  friend std::ostream& operator<<(std::ostream& os, const this_type& refer)
  {
    os << *refer;
    return os;
  }

};

inline BigFraction operator"" _fr(const char* str, size_t length)
{
  const char * delimeter = strchr(str, '/');
  if (delimeter)
  {
    char buffer[256];
    memcpy(buffer, str, delimeter - str);
    buffer[delimeter - str] = 0;

    return BigFraction{ std::string(buffer), std::string(delimeter + 1) };
  }
  else
  {
    const char * delimeter = strchr(str, '.');

    if (delimeter)
    {
      char buffer_high[256];
      char buffer_low[256];
      int lowlen = length - (delimeter - str);

      buffer_low[0] = '1';
      memset(buffer_low + 1, '0', lowlen - 1);
      buffer_low[lowlen] = 0;

      memcpy(buffer_high, str, delimeter - str);
      memcpy(buffer_high + (delimeter - str), delimeter + 1, lowlen + 1);

      return BigFraction{ std::string(buffer_high), std::string(buffer_low) };
    }
    else
    {
      return BigFraction{ std::string(str) };
    }
  }
}

inline BigFraction operator"" _fr(unsigned long long i)
{
  return BigFraction{ std::to_string(i) };
}

}

#endif