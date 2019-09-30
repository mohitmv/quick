// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef QUICK_ALIAS_HPP_
#define QUICK_ALIAS_HPP_

#include <utility>

#define QUICK_FUNCTION_ALIAS(A, B)        \
template<typename... Args>                \
auto A(Args&&... args) {                  \
  return B(std::forward<Args>(args)...);  \
}

#endif  // QUICK_ALIAS_HPP_
