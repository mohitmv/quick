// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef QUICK_UTILITY_HPP_
#define QUICK_UTILITY_HPP_

namespace quick {

struct AbstractType {
  template<typename T>
  operator const T&() const {
    return static_cast<const T&>(*this);
  }
  template<typename T>
  operator T&() {
    return static_cast<T&>(*this);
  }
  virtual ~AbstractType() = default;
};


}  // namespace quick

namespace qk = quick;


#endif  // QUICK_UTILITY_HPP_
