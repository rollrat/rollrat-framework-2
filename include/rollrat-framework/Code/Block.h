//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  Block.h - 01/26/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _BLOCK_9bf1541fdf7efd41b7b39543fd870ac4_
#define _BLOCK_9bf1541fdf7efd41b7b39543fd870ac4_

#include <vector>
#include "WString.h"
#include "Code/Type.h"

namespace ofw
{
class BaseBlock;

class Space
{
  WString name;
  size_t  index;
  unsigned HasName : 1;
  std::vector<BaseBlock *> blocks;

public:
  using iterator = std::vector<BaseBlock *>::iterator;
  using const_interator = std::vector<BaseBlock *>::const_iterator;

  Space(size_t Index) : index(Index), HasName(false) { }

  bool hasName() const { return HasName; }
  WString getName() const { return name; }
  void setName(const WString& name) { HasName = true; this->name = name; }

  iterator getBaseBlocks() { return blocks.begin(); }
  const_interator getBaseBlocks() const { return blocks.begin(); }
};

class Value
{
  WString name;
  Space *super;
  unsigned HasName : 1;
public:
  Value(Space *S) : super(S), HasName(false) { }

  bool hasName() const { return HasName; }
  WString getName() const { return name; }
  void setName(const WString& name) { HasName = true; this->name = name; }

  const Space *getParent() const { return super; }
  Space *getParent() { return super; }
};

class ConstValue : public Value
{
  Type *type;
public:
  ConstValue(Space *S) : Value(S) { }

  void setType(Type *type) { this->type = type; }
  Type *getType() { return this->type; }
  const Type *getType() const { return this->type; }
};

class VariableValue : public Value
{
  Type *type;
public:
  VariableValue(Space *S) : Value(S) { }

  void setType(Type *type) { this->type = type; }
  Type *getType() { return this->type; }
  const Type *getType() const { return this->type; }
};

class BaseBlock : public Value
{
public:
  BaseBlock(Space *S) : Value(S) { }
};

class UnaryBlock : public BaseBlock
{
  BaseBlock *O;
public:
  UnaryBlock(BaseBlock *O) : O(O), BaseBlock(O->getParent()) { }
  BaseBlock *getOperand() { return O; }
  const BaseBlock *getOperand() const { return O; }
};

class BinaryBlock : public BaseBlock
{
  BaseBlock *L;
  BaseBlock *R;
public:
  BinaryBlock(BaseBlock *L, BaseBlock *R) 
    : L(L), R(R), BaseBlock(L->getParent()) { }

  BaseBlock *getLOperand() { return L; }
  const BaseBlock *getLOperand() const { return L; }
  BaseBlock *getROperand() { return R; }
  const BaseBlock *getROperand() const { return R; }
};

#define CODE_BLOCK_DEF_UNARY(name) \
  class name##Block : public UnaryBlock { \
    public: name##Block(BaseBlock *O) : UnaryBlock(O) {} };
#define CODE_BLOCK_DEF_BINARY(name)\
  class name##Block : public BinaryBlock { \
    public: name##Block(BaseBlock *L, BaseBlock *R) \
      : L(L), R(R), BaseBlock(L->getParent()) { };
#include "Code/Blocks.def"

}

#endif