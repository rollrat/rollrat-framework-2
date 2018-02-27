//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  Random.h - 01/26/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _RANDOM_9bf1541fdf7efd41b7b39543fd870ac4_
#define _RANDOM_9bf1541fdf7efd41b7b39543fd870ac4_

#include <cmath>
#include <random>
#include <string>
#include <vector>

namespace ofw {

class Random {
 public:
  using rand_type = unsigned long long;

  Random(rand_type lvalue, rand_type rvalue, rand_type seed) {
    rand_type size = rvalue - lvalue + 1;
    std::mt19937_64 rnd(seed);
    std::vector<bool> overlap_check(size);

    for (rand_type i = 0; i < size; i++) {
      do {
        rand_type real = rnd() % size + lvalue;
        if (!overlap_check[real - lvalue]) {
          vec.push_back(real);
          overlap_check[real - lvalue] = true;
          break;
        }
      } while (true);
    }
  }

  Random(rand_type lvalue, rand_type rvalue)
      : Random(lvalue, rvalue, std::random_device()()) {}

  inline rand_type operator[](rand_type index) const { return vec[index]; }

  inline rand_type size() const { return vec.size(); }

  static rand_type getRandomNumber(rand_type seed) {
    std::mt19937_64 rnd(seed);
    return rnd();
  }

  static rand_type getRandomNumber() {
    return getRandomNumber(std::random_device()());
  }

 private:
  std::vector<rand_type> vec;
};

}  // namespace ofw

#endif