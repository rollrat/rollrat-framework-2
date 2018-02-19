//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  Sorting.h - 01/26/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _SORTING_9bf1541fdf7efd41b7b39543fd870ac4_
#define _SORTING_9bf1541fdf7efd41b7b39543fd870ac4_

#include <functional>
#include "WString.h"

namespace ofw
{

///===-----------------------------------------------------------------------===
///
///               Sorting Base
///
///===-----------------------------------------------------------------------===

template<typename type>
class SortingBase
{
public:
  SortingBase(type *what, size_t size) : source(what), size(size) { }
  void Sort();

private:
  int flog2();
  virtual void Swap(int t1, int t2);
  virtual void SwapCompare(int t1, int t2);
  void IntroSort(int l, int h, int depth);
  virtual int pivot(int l, int h);
  virtual void InsertionSort(int l, int h);
  virtual void DownHeap(int i, int n, int l);
  void HeapSort(int l, int h);

protected:
  size_t size;
  type*  source;
};

template<typename type>
void SortingBase<type>::Sort()
{
  IntroSort(0, size - 1, flog2() * 2);
}

template<typename type>
int SortingBase<type>::flog2()
{
  size_t tsz = size;
  int ret = 0;
  for (; tsz; tsz >>= 1) ret++;
  return ret;
}

template<typename type>
void SortingBase<type>::Swap(int t1, int t2)
{
  if (t1 != t2) {
    type x = source[t1];
    source[t1] = source[t2];
    source[t2] = x;
  }
}

template<typename type>
void SortingBase<type>::SwapCompare(int t1, int t2)
{
  if (t1 != t2) {
    if (source[t1] > source[t2]) {
      type x = source[t1];
      source[t1] = source[t2];
      source[t2] = x;
    }
  }
}

template<typename type>
void SortingBase<type>::IntroSort(int l, int h, int depth)
{
  while (l < h)
  {
    int partition = h - l + 1;

    // small part
    if (partition <= 32)
    {
      if (partition == 1) {
        return;
      } else if (partition == 2) {
        SwapCompare(l, h);
        return;
      } else if (partition == 3) {
        SwapCompare(l, h - 1);
        SwapCompare(l, h);
        SwapCompare(h - 1, h);
        return;
      }

      InsertionSort(l, h);
      return;
    }

    if (depth == 0) {
      HeapSort(l, h);
      return;
    }

    int p = pivot(l, h);
    IntroSort(p + 1, h, --depth);
    h = p - 1;
  }
}

template<typename type>
int SortingBase<type>::pivot(int l, int h)
{
  int mid = l + (h - l) >> 1;

  SwapCompare(l, mid);
  SwapCompare(l, h);
  SwapCompare(mid, h);

  type pivot = source[mid];
  Swap(mid, h - 1);

  int left = l, right = h - 1;

  while (left < right)
  {
    while (source[++left] < pivot)
      ;
    while (pivot < source[--right])
      ;

    if (left >= right)
      break;

    Swap(left, right);
  }

  Swap(left, h - 1);
  return left;
}

template<typename type>
void SortingBase<type>::InsertionSort(int l, int h)
{
  int i, j;
  type t;
  for (i = l; i < h; i++)
  {
    j = i;
    t = source[i + 1];
    while (j >= l && source[j] > t)
    {
      source[j + 1] = source[j];
      j--;
    }
    source[j + 1] = t;
  }
}

template<typename type>
void SortingBase<type>::DownHeap(int i, int n, int l)
{
  type d = source[l + i - 1];
  int ch;
  while (i <= n >> 1)
  {
    ch = i << 1;
    if (ch < n && source[l + ch - 1] < source[l + ch])
      ch++;
    if (!(d < source[l + ch - 1]))
      break;
    source[l + i - 1] = source[l + ch - 1];
    i = ch;
  }
  source[l + i - 1] = d;
}

template<typename type>
void SortingBase<type>::HeapSort(int l, int h)
{
  int n = h - l + 1;
  for (int i = n >> 1; i >= 1; i--) {
    DownHeap(i, n, l);
  } {
    Swap(l, l + i - 1);
    DownHeap(1, i - 1, l);
  }
}

///===-----------------------------------------------------------------------===
///
///               Sorting
///
///===-----------------------------------------------------------------------===

template<typename type>
class Sorting : public SortingBase<type>
{
public:
  Sorting(type *what, size_t size) :
    SortingBase(what, size) { }
};

///===-----------------------------------------------------------------------===
///
///               Sorting WString
///
///===-----------------------------------------------------------------------===

template<>
class Sorting<WString>
{
public:
  Sorting(WString *what, size_t size) : source(what), size(size) { }
  void Sort();

private:
  int flog2();
  virtual void Swap(int t1, int t2);
  virtual void SwapCompare(int t1, int t2);
  void IntroSort(int l, int h, int depth);
  int pivot(int l, int h);
  void InsertionSort(int l, int h);
  void DownHeap(int i, int n, int l);
  void HeapSort(int l, int h);

protected:
  size_t size;
  WString*  source;
};

inline void Sorting<WString>::Sort()
{
  IntroSort(0, size - 1, flog2() * 2);
}

inline int Sorting<WString>::flog2()
{
  size_t tsz = size;
  int ret = 0;
  for (; tsz; tsz >>= 1)
    ret++;
  return ret;
}

inline void Sorting<WString>::Swap(int t1, int t2)
{
  if (t1 != t2) {
    source[t1].Swap(source[t2]);
  }
}

inline void Sorting<WString>::SwapCompare(int t1, int t2)
{
  if (t1 != t2 && source[t1] > source[t2]) {
    source[t1].Swap(source[t2]);
  }
}

inline void Sorting<WString>::IntroSort(int l, int h, int depth)
{
  while (l < h)
  {
    int partition = h - l + 1;

    // small part
    if (partition <= 32)
    {
      if (partition == 1)
        return;
      else if (partition == 2) {
        SwapCompare(l, h);
        return;
      } else if (partition == 3) {
        SwapCompare(l, h - 1);
        SwapCompare(l, h);
        SwapCompare(h - 1, h);
        return;
      }

      InsertionSort(l, h);
      return;
    }

    if (depth == 0)
    {
      HeapSort(l, h);
      return;
    }
    depth--;

    int p = pivot(l, h);
    IntroSort(p + 1, h, depth);
    h = p - 1;
  }
}

inline int Sorting<WString>::pivot(int l, int h)
{
  int mid = l + ((h - l) >> 1);

  SwapCompare(l, mid);
  SwapCompare(l, h);
  SwapCompare(mid, h);

  WString pivot;
  pivot.CloneSet(source[mid]);
  Swap(mid, h - 1);

  int left = l, right = h - 1;

  while (left < right)
  {
    while (source[++left] < pivot)
      ;
    while (pivot < source[--right])
      ;

    if (left >= right)
      break;

    Swap(left, right);
  }

  Swap(left, h - 1);
  return left;
}

inline void Sorting<WString>::InsertionSort(int l, int h)
{
  int i, j;
  WString t;
  for (i = l; i < h; i++) {
    j = i;
    t.CloneSet(source[i + 1]);
    while (j >= l && source[j] > t) {
      source[j + 1].CloneSet(source[j]);
      j--;
    }
    source[j + 1].CloneSet(t);
  }
}

inline void Sorting<WString>::DownHeap(int i, int n, int l)
{
  WString d;
  d.CloneSet(source[l + i - 1]);
  int ch;
  while (i <= n >> 1)
  {
    ch = i << 1;
    if (ch < n && source[l + ch - 1] < source[l + ch])
      ch++;
    if (!(d < source[l + ch - 1]))
      break;
    source[l + i - 1].CloneSet(source[l + ch - 1]);
    i = ch;
  }
  source[l + i - 1].CloneSet(d);
}

inline void Sorting<WString>::HeapSort(int l, int h)
{
  int n = h - l + 1;
  for (int i = n >> 1; i >= 1; i--) {
    DownHeap(i, n, l);
  }
  for (int i = n; i > 1; i--) {
    Swap(l, l + i - 1);
    DownHeap(1, i - 1, l);
  }
}

///===-----------------------------------------------------------------------===
///
///               Sorting With
///
///===-----------------------------------------------------------------------===

template<typename type, typename another_type>
class SortingWith : public Sorting<type>
{
  another_type *another;
public:
  SortingWith(type *what, another_type *another, size_t size) :
    SortingBase(what, size) { }
private:
  virtual void Swap(int t1, int t2);
  virtual void SwapCompare(int t1, int t2);
  virtual void InsertionSort(int l, int h);
  virtual void DownHeap(int i, int n, int l);
};

template<typename type, typename another_type>
inline void SortingWith<type, another_type>::Swap(int t1, int t2)
{
  if (t1 != t2) {
    type x = source[t1];
    source[t1] = source[t2];
    source[t2] = x;

    another_type y = another[t1];
    another[t1] = another[t2];
    another[t2] = y;
  }
}

template<typename type, typename another_type>
inline void SortingWith<type, another_type>::SwapCompare(int t1, int t2)
{
  if (t1 != t2 && source[t1] > source[t2]) {
    type x = source[t1];
    source[t1] = source[t2];
    source[t2] = x;

    another_type y = another[t1];
    another[t1] = another[t2];
    another[t2] = y;
  }
}

template<typename type, typename another_type>
inline void SortingWith<type, another_type>::InsertionSort(int l, int h)
{
  int i, j;
  type t;
  another_type ta;
  for (i = l; i < h; i++)
  {
    j = i;
    t = source[i + 1];
    ta = another[i + 1];
    while (j >= l && source[j] > t)
    {
      source[j + 1] = source[j];
      another[j + 1] = another[j];
      j--;
    }
    source[j + 1] = t;
    another[j + 1] = ta;
  }
}

template<typename type, typename another_type>
inline void SortingWith<type, another_type>::DownHeap(int i, int n, int l)
{
  type d = source[l + i - 1];
  another_type da = another[l + i - 1];
  int ch;
  while (i <= n / 2)
  {
    ch = i * 2;
    if (ch < n && source[l + ch - 1] < source[l + ch])
      ch++;
    if (!(d < source[l + ch - 1]))
      break;
    source[l + i - 1] = source[l + ch - 1];
    another[l + i - 1] = another[l + ch - 1];
    i = ch;
  }
  source[l + i - 1] = d;
  another[l + i - 1] = da;
}

///===-----------------------------------------------------------------------===
///
///               Sorting With WString
///
///===-----------------------------------------------------------------------===

template<typename another_type>
class SortingWith<WString, another_type> : public Sorting<WString>
{
  another_type *another;
public:
  SortingWith(WString *what, another_type *another, size_t size) :
    SortingBase(what, size) { }
private:
  virtual void Swap(int t1, int t2) {
    if (t1 != t2) {
      source[t1].Swap(source[t2]);

      another_type y = another[t1];
      another[t1] = another[t2];
      another[t2] = x;
    }
  }
  virtual void SwapCompare(int t1, int t2) {
    if (t1 != t2 && source[t1] > source[t2]) {
        source[t1].Swap(source[t2]);

        another_type y = another[t1];
        another[t1] = another[t2];
        another[t2] = y;
    }
  }
  virtual void InsertionSort(int l, int h) {
    int i, j;
    WString t;
    another_type ta;
    for (i = l; i < h; i++)
    {
      j = i;
      t.Clone(source[i + 1]);
      ta = another[i + 1];
      while (j >= l && source[j] > t)
      {
        source[j + 1].Clone(source[j]);
        another[j + 1] = another[j];
        j--;
      }
      source[j + 1].Clone(t);
      another[j + 1] = ta;
    }
  }
  virtual void DownHeap(int i, int n, int l) {
    WString d;
    d.Clone(ource[l + i - 1]);
    another_type da = another[l + i - 1];
    int ch;
    while (i <= n / 2)
    {
      ch = i * 2;
      if (ch < n && source[l + ch - 1] < source[l + ch])
        ch++;
      if (!(d < source[l + ch - 1]))
        break;
      source[l + i - 1].Clone(source[l + ch - 1]);
      another[l + i - 1] = another[l + ch - 1];
      i = ch;
    }
    source[l + i - 1].Clone(d);
    another[l + i - 1] = da;
  }
};

}

#endif