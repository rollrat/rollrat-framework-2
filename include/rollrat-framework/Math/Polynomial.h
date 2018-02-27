//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  Polynomial.h - 01/21/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _POLYNOMIAL_
#define _POLYNOMIAL_

#include <memory.h>
#include <string>

namespace ofw {

template <typename type>
class PolynomialNode {
 public:
  typedef PolynomialNode<type> this_type;

  this_type *next;
  this_type *prev;

  type degree;
  type coeff;
};

template <typename type>
class Polynomial {
  typedef Polynomial<type> this_type;
  typedef PolynomialNode<type> node_type;
  typedef typename type number_type;

  const number_type zero = static_cast<number_type>(0);
  const number_type one = static_cast<number_type>(1);

  node_type *start;
  node_type *end;

 public:
  Polynomial() { init(); }
  Polynomial(const this_type &clone) : Polynomial() { this->clone(clone); }
  ~Polynomial() { destroy(); }

  void set(number_type coeff, number_type degree) {
    node_type *iter = start;

    if (iter->next != end) iter = iter->next;

    while (iter->next != end) {
      if (iter->degree < degree) {
        // Cannot find target degree's node
        iter = iter->prev;
        break;
      } else if (iter->degree == degree) {
        // Target degree's node is found!
        iter->coeff = coeff;
        return;
      }
      iter = iter->next;
    }

    // Push new node, if target degree is not exist.
    push(iter, coeff, degree);
  }

  this_type &operator+=(const this_type &poly) {
    return plus_minus(poly, true);
  }
  this_type operator+(const this_type &poly) const {
    this_type tmp = *this;
    return tmp += poly;
  }

  this_type &operator-=(const this_type &poly) {
    return plus_minus(poly, false);
  }
  this_type operator-(const this_type &poly) const {
    this_type tmp = *this;
    return tmp -= poly;
  }

  this_type &operator*=(const this_type &poly) {
    this_type p;
    node_type *iter = poly.start->next;

    while (iter != poly.end) {
      this_type tmp(*this);
      node_type *iter_n = tmp.start->next;
      while (iter_n != tmp.end) {
        iter_n->coeff *= iter->coeff;
        iter_n->degree += iter->degree;
        iter_n = iter_n->next;
      }
      p += tmp;
      iter = iter->next;
    }
    *this = p;
    return *this;
  }
  this_type operator*(const this_type &poly) const {
    this_type tmp = *this;
    return tmp *= poly;
  }

  this_type &operator*=(const number_type num) {
    if (num == zero) {
      to_zero();
    } else {
      node_type *iter = start->next;
      while (iter != end) {
        iter->coeff *= num;
        iter = iter->next;
      }
    }
    return *this;
  }
  this_type operator*(const number_type num) const {
    this_type tmp = *this;
    return tmp *= num;
  }

  this_type &operator/=(const number_type num) {
    node_type *iter = start->next;
    while (iter != end) {
      iter->coeff /= num;
      iter = iter->next;
    }
    return *this;
  }
  this_type operator/(const number_type num) const {
    this_type tmp = *this;
    return tmp /= num;
  }
  this_type &operator/=(const this_type &poly) { return div_rest(poly, true); }
  this_type operator/(const this_type &poly) const {
    this_type tmp = *this;
    return tmp /= poly;
  }

  this_type &operator%=(const this_type &poly) { return div_rest(poly, false); }
  this_type operator%(const this_type &poly) const {
    this_type tmp = *this;
    return tmp %= poly;
  }

  this_type &derivative() {
    node_type *iter = start->next;
    while (iter != end) {
      if (iter->degree != zero) {
        iter->coeff *= iter->degree;
        iter->degree -= one;
      } else {
        node_type *tmp = iter->next;
        pop(iter);
        iter = tmp;
      }
    }
    return *this;
  }

  std::string operator*() const {
    std::string build;
    node_type *iter = start->next;

    while (iter != end) {
      if (iter->coeff != one) {
        build.append(std::to_string(iter->coeff));
        if (iter->degree != zero) build.append("*x");
      } else {
        if (iter->degree != zero) build.append("x");
      }
      if (iter->degree != zero && iter->degree != one) {
        build.append("^");
        build.append(std::to_string(iter->degree));
      }
      if (iter->next != end && iter->next->coeff > zero) build.append("+");
      iter = iter->next;
    }

    return build;
  }

  this_type &operator=(const this_type &poly) {
    to_zero();
    clone(poly);
    return *this;
  }

  template <typename _Func>
  number_type evaluate(number_type num, _Func &func) const {
    node_type *iter = start->next;
    number_type ret = zero;

    while (iter != end) {
      ret += func(num, iter->coeff, iter->degree);
      iter = iter->next;
    }

    return ret;
  }

 private:
  this_type &plus_minus(const this_type &poly, bool pm) {
    node_type *iter1 = start->next;
    node_type *iter2 = poly.start->next;

    while (iter1 != end && iter2 != poly.end) {
      if (iter1->degree < iter2->degree) {
        if (pm == true)
          push(iter1->prev, iter2->coeff, iter2->degree);
        else
          push(iter1->prev, -iter2->coeff, iter2->degree);
        iter2 = iter2->next;
      } else if (iter1->degree > iter2->degree) {
        if (iter1->next->degree <= iter2->degree) {
          iter1 = iter1->next;
          continue;
        }
        if (pm == true)
          push(iter1->prev, iter2->coeff, iter2->degree);
        else
          push(iter1->prev, -iter2->coeff, iter2->degree);
        iter2 = iter2->next;
      } else {
        if (pm == true)
          iter1->coeff += iter2->coeff;
        else
          iter1->coeff -= iter2->coeff;
        if (iter1->coeff == zero) {
          node_type *tmp = iter1->next;
          iter2 = iter2->next;
          pop(iter1);
          iter1 = tmp;
        } else {
          iter1 = iter1->next;
          iter2 = iter2->next;
        }
      }
    }

    while (iter2 != poly.end) {
      push(iter1->prev, iter2->coeff, iter2->degree);
      iter1 = iter1->next;
      iter2 = iter2->next;
    }

    return *this;
  }

  this_type &div_rest(const this_type &poly, bool dr) {
    node_type *iter = start->next;
    node_type *siter = poly.start->next;

    if (iter != end && siter != poly.end) {
      if (iter->degree >= siter->degree) {
        this_type tmp(*this);
        this_type dim;

        iter = tmp.start->next;

        while (iter->degree >= siter->degree) {
          this_type mm(poly);
          number_type gap = iter->degree - siter->degree;
          mm.degree_plus(gap);
          dim.set(iter->coeff, gap);
          tmp -= (mm / siter->coeff) * iter->coeff;
          iter = tmp.start->next;
        }

        to_zero();

        if (dr == true)
          clone(dim);
        else
          clone(tmp);
      } else {
        to_zero();
      }
    } else {
      to_zero();
    }

    return *this;
  }

  void to_zero() {
    destroy();
    init();
  }

  void degree_plus(const number_type degree) {
    node_type *iter = start->next;

    while (iter != end) {
      iter->degree += degree;
      iter = iter->next;
    }
  }

  void degree_minus(const number_type degree) {
    node_type *iter = start->next;

    while (iter != end) {
      iter->degree -= degree;
      iter = iter->next;
    }
  }

  node_type *get_node() {
    node_type *node = new node_type;
    node->next = nullptr;
    node->prev = nullptr;
    node->degree = zero;
    node->coeff = zero;
    return node;
  }
  inline void tie(node_type *prev, node_type *next) {
    // prev ~ next
    prev->next = next;
    next->prev = prev;
  }
  inline void tie(node_type *prev, node_type *target, node_type *next) {
    // prev ~ target ~ next
    prev->next = target;
    target->prev = prev;
    target->next = next;
    next->prev = target;
  }
  inline void push(node_type *prev, number_type coeff, number_type degree) {
    // prev ~ (new) ~ prev->next
    node_type *node = get_node();
    node->coeff = coeff;
    node->degree = degree;
    tie(prev, node, prev->next);
  }
  inline void pop(node_type *target) {
    // target->prev ~ target->next
    target->prev->next = target->next;
    target->next->prev = target->prev;
    delete target;
  }
  void init() {
    start = get_node();
    end = get_node();
    tie(start, end);
    end->next = end;
  }

  void clone(const this_type &clone) {
    node_type *iter1 = start;
    node_type *iter2 = clone.start->next;

    while (iter2 != clone.end) {
      push(iter1, iter2->coeff, iter2->degree);
      iter1 = iter1->next;
      iter2 = iter2->next;
    }
  }

  void destroy() {
    node_type *c = end;

    while (c != start) {
      node_type *t = c->prev;
      delete c;
      c = t;
    }

    delete start;
  }
};

}  // namespace ofw

#endif