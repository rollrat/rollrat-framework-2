//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  Array.h - 01/21/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _ARRAY_
#define _ARRAY_

namespace ofw
{

template<typename type>
class ArrayBase
{
public:

  ArrayBase()
    : m_size(0)
    , m_pointer(nullptr)
    , m_constptr(nullptr)
  {
  }

  ArrayBase(size_t size, type *pointer)
    : m_size(size)
    , m_pointer(pointer)
    , m_constptr(pointer)
  {
  }

  virtual ~ArrayBase()
  {
    if (!m_nodel)
    {
      _class_delete(*m_constptr, m_constptr);
      delete[] m_constptr;
      m_constptr = nullptr;
    }
  }

  template<typename type>
  void _class_delete(type&, type*&)
  {
  }
  template<typename type>
  void _class_delete(type*&, type* const*& arr)
  {
    for (size_t i = 0; i < m_size; i++)
    {
      arr[i].~type();
      delete arr[i];
    }
  }

protected:

  bool   m_nodel = false;
  size_t m_size;
  type  *m_pointer;
  type  *m_constptr;

};

// Read Only Array [ const array iterator ]
template<typename type>
class ReadOnlyArray
  : public ArrayBase<type>
{
  typedef ReadOnlyArray<type> this_type;

public:
  ReadOnlyArray(type *ptr, size_t size) : ArrayBase<type>(size, ptr) { }
  ReadOnlyArray() : ArrayBase<type>() { }
  ~ReadOnlyArray() { }

  type operator*() const { return *this->m_pointer; }
  type* operator->() const { return **this; }
  type operator[](size_t off) const { return *(*this + off); }
  this_type& operator--() { --this->m_pointer; return *this; }

  this_type operator--(int)
  {
    this_type tmp = *this;
    tmp.m_nodel = true;
    --this->m_pointer;
    return tmp;
  }

  this_type& operator++()
  {
    ++this->m_pointer;
    return *this;
  }

  this_type operator++(int)
  {
    this_type tmp = *this;
    tmp.m_nodel = true;
    ++*this->m_pointer;
    return tmp;
  }

  this_type& operator+=(size_t size)
  {
    this->m_pointer += size;
    return *this;
  }

  this_type operator+(size_t size) const
  {
    this_type tmp = *this;
    tmp.m_nodel = true;
    return (tmp += size);
  }

  this_type& operator-=(size_t size)
  {
    this->m_pointer -= size;
    return *this;
  }

  this_type operator-(size_t size) const
  {
    this_type tmp = *this;
    tmp.m_nodel = true;
    return (tmp -= size);
  }

  bool operator==(const this_type& refer) const
  { return (this->m_pointer == refer.m_pointer); }
  bool operator!=(const this_type& refer) const
  { return (!(*this == refer)); }
  bool operator<(const this_type& refer) const
  { return (this->m_pointer < refer.m_pointer); }
  bool operator>(const this_type& refer) const
  { return (refer < *this); }
  bool operator<=(const this_type& refer) const
  { return (!(refer < *this)); }
  bool operator>=(const this_type& refer) const
  { return (!(*this < refer)); }
  void Reset() { this->m_pointer = this->m_constptr; }
  type* Array() const { return this->m_constptr; }
  size_t Size() const { return this->m_size; }

  // For Each <Item> In Type _ etc...
  template<typename func>
  void Each(func& function)
  {
    for (size_t i = 0; i < this->m_size; i++)
      function(this->m_constptr[i]);
  }

};

}

#endif