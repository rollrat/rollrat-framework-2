//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  Type.h - 01/26/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _TYPE_9bf1541fdf7efd41b7b39543fd870ac4_
#define _TYPE_9bf1541fdf7efd41b7b39543fd870ac4_

#include <stdint.h>
#include "rollrat-framework/BigInteger/BigInteger.h"
#include "rollrat-framework/WString.h"

namespace ofw
{

class Type
{
public:
  typedef enum class Types{
    BooleanType,
    IntegerType,
    FloatType,
    BigIntegerType,
    StringType,
  } TypeList;

private:
  TypeList type;

public:
  Type(TypeList type) : type(type) { }
  TypeList getType() const { return type; }
};

class BooleanType : public Type
{
  bool value;
public:
  BooleanType(bool value) : value(value), Type(TypeList::BooleanType) { }

  void setValue(bool value) { this->value = value; }
  bool getValue() const { return value; }
};

class IntegerType : public Type
{
  uint64_t value;
public:
  IntegerType(uint64_t value) : value(value), Type(TypeList::IntegerType) { }

  void setValue(uint64_t value) { this->value = value; }
  uint64_t getValue() const { return value; }
};

class FloatType : public Type
{
  double value;
public:
  FloatType(double value) : value(value), Type(TypeList::FloatType) { }

  void setValue(double value) { this->value = value; }
  double getValue() const { return value; }
};

class BigIntegerType : public Type
{
  BigInteger value;
public:
  BigIntegerType(BigInteger& value) : value(value), Type(TypeList::BigIntegerType) { }

  void setValue(BigInteger value) { this->value = value; }
  BigInteger getValue() const { return value; }
};

class StringType : public Type
{
  WString value;
public:
  StringType(WString& value) : value(value), Type(TypeList::StringType) { }

  void setValue(WString value) { this->value = value; }
  WString getValue() const { return value; }
};

}

#endif