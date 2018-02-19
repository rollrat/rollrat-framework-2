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

#include "BigInteger/BigFraction.h"

using namespace ofw;

BigFraction::BigFraction()
  : high(uint32_t(0))
  , low(1)
{
}

BigFraction::BigFraction(std::string wstr)
  : high(wstr)
  , low(1)
{
}

BigFraction::BigFraction(integer_operation io)
  : high(io)
  , low(1)
{
}

BigFraction::BigFraction(std::string wstr1, std::string wstr2)
  : high(wstr1)
  , low(wstr2)
{
  reduction();
}

BigFraction::BigFraction(uint32_t uip1, uint32_t uip2)
  : high(uip1)
  , low(uip2)
{
}

BigFraction::this_type& BigFraction::operator=(std::string wstr)
{
  high = wstr;
  low = 1;
  return *this;
}

BigFraction::this_type& BigFraction::operator=(const this_type& refer)
{
  high = refer.high;
  low = refer.low;
  return *this;
}

BigFraction::this_type& BigFraction::operator*=(const this_type& refer)
{
  integer_operation hit = refer.high * high;
  integer_operation lot = refer.low * low;

  integer_operation gcd = hit.abs().gcd(lot.abs());

  high = hit / gcd;
  low = lot / gcd;

  return *this;
}

BigFraction::this_type BigFraction::operator*(const this_type& refer) const
{
  this_type tmp = *this;
  return tmp *= refer;
}

BigFraction::this_type& BigFraction::operator/=(const this_type& refer)
{
  integer_operation hit = refer.low * high;
  integer_operation lot = refer.high * low;

  integer_operation gcd = hit.abs().gcd(lot.abs());

  high = hit / gcd;
  low = lot / gcd;

  return *this;
}

BigFraction::this_type BigFraction::operator/(const this_type& refer) const
{
  this_type tmp = *this;
  return tmp /= refer;
}

BigFraction::this_type& BigFraction::operator+=(const this_type& refer)
{
  integer_operation lcm = low.abs().lcm(refer.low.abs());

  high = high * lcm / low + refer.high * lcm / refer.low;
  low = lcm;

  reduction();

  return *this;
}

BigFraction::this_type BigFraction::operator+(const this_type& refer) const
{
  this_type tmp = *this;
  return tmp += refer;
}

BigFraction::this_type& BigFraction::operator-=(const this_type& refer)
{
  integer_operation lcm = low.abs().lcm(refer.low.abs());

  high = high * lcm / low - refer.high * lcm / refer.low;
  low = lcm;

  return *this;
}

BigFraction::this_type BigFraction::operator-(const this_type& refer) const
{
  this_type tmp = *this;
  return tmp -= refer;
}

bool BigFraction::operator==(const this_type& refer) const
{
  return refer.high == high && refer.low == low;
}

bool BigFraction::operator!=(const this_type& refer) const
{
  return !(*this == refer);
}

bool BigFraction::operator<(const this_type& refer) const
{
  integer_operation lcm = low.abs().lcm(refer.low.abs());

  return (high * lcm / low) < (refer.high * lcm / refer.low);
}

bool BigFraction::operator>(const this_type& refer) const
{
  return refer < *this;
}

bool BigFraction::operator<=(const this_type& refer) const
{
  return !(refer < *this);
}

bool BigFraction::operator>=(const this_type& refer) const
{
  return !(*this < refer);
}

void BigFraction::reverse() const
{
  high.swap(low);
}

void BigFraction::reduction() const
{
  integer_operation gcd = high.abs().gcd(low.abs());
  high /= gcd;
  low /= gcd;
}

std::string BigFraction::operator*() const
{
  std::string build;

  if (high.sign() != low.sign())
    build.append("-");

  reduction();

  build.append(high.abs().to_string());

  if (low != "1")
  {
    build.append("/");
    build.append(low.abs().to_string());
  }

  return build;
}

std::string BigFraction::high_part() const
{
  return high.to_string();
}

std::string BigFraction::low_part() const
{
  return low.to_string();
}

bool BigFraction::sign() const
{
  if (high.sign() != low.sign())
    return true;
  else
    return false;
}

void BigFraction::cutting(int size)
{
  high >>= size;
  low >>= size;
}

std::string BigFraction::fraction_point(size_t valid_point) const
{
  integer_operation hit = high;
  integer_operation lot = low;
  integer_operation div = hit / lot;

  std::string builder;

  builder.append(div.to_string());
  builder.append(".");

  hit -= div * lot; // high % low

  for (int i = 0; i < valid_point && hit != 0; i++)
  {
    hit *= 10;
    div = hit / lot;
    builder.append(div.to_string());
    hit -= div * lot;
  }

  return builder;
}