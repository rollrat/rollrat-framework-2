//===----------------------------------------------------------------------===//
//
//                          RollRat-FrameWork V2
//
//===----------------------------------------------------------------------===//
//
//  Extension.h - 03/24/2018
//
//  Copyright (C) 2018. rollrat. All Rights Reserved.
//
//===----------------------------------------------------------------------===//

#ifndef _EXTENSION_9bf1541fdf7efd41b7b39543fd870ac4_
#define _EXTENSION_9bf1541fdf7efd41b7b39543fd870ac4_

#include <functional>
#include <memory>

namespace ofw {

template <typename container>
class Extension {
 public:
  using pointer_type = std::shared_ptr<container>;
  template <typename container_type>
  using this_type = Extension<container_type>;

  Extension(pointer_type ptr);

  this_type<container> where(
      std::function<bool(typename container::value_type)> func);

  template <typename container_type, typename type>
  this_type<container_type> select(
      std::function<type(typename container::value_type)> func);

 private:
  pointer_type container;
};

template <typename container>
Extension<container> from(std::shared_ptr<container> to) {
  return Extension<container>(to);
}

}  // namespace ofw

#endif