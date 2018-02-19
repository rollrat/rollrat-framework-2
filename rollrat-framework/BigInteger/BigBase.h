//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  BigBase.h - 01/21/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _BIGBASE_9bf1541fdf7efd41b7b39543fd870ac4_
#define _BIGBASE_9bf1541fdf7efd41b7b39543fd870ac4_

#include <stdint.h>
#include <memory>
#include <string>
#include <cassert>
#include <iostream>

namespace ofw
{

class BigBase
{
  typedef BigBase this_type;

  uint32_t *blocks = nullptr;
  size_t    length = 0;

public:

  BigBase();
  BigBase(uint32_t uiptr);
  BigBase(const this_type& refer);
  BigBase(const char* str);
  BigBase(const std::string str);

  ~BigBase() { delete[] blocks; }

  void aly_hex(const std::string str);
  void aly_dec(const std::string str);

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
  bool iszero() const;

  this_type gcd(const this_type& integer) const;
  this_type lcm(const this_type& integer) const;

  std::string to_string() const;
  std::string to_hex() const;
  std::string to_binary() const;

  void swap(this_type& t);

  size_t _length() const;

private:

  inline uint32_t div_shift(uint32_t v, int c, bool inv = false);
  inline uint32_t div_multiget(const uint32_t* v, size_t s, int sh);

  void divideWithRemainder(const this_type& integer, bool quotient,
    uint32_t *quotient_array = nullptr);

  void init();

  void allocate(size_t i);
  void reallocate(size_t i);
  void safe_allocate(size_t i);

  void copy(const this_type& g);

  void init_hex(std::string hex);
  void init_decimal(std::string dec);

  void trim();

  int count_leading_zero(uint32_t val) const;
  int count_tailing_zero(uint32_t val) const;

};

}

#endif