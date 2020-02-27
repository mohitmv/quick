// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef QUICK_UTILITY_HPP_
#define QUICK_UTILITY_HPP_



namespace quick {

// A type, which can safely hold the unique_ptr/shared_ptr of any type derived
// from it.
struct AbstractType {
  virtual ~AbstractType() = default;
};


}  // namespace quick

namespace qk = quick;


#endif  // QUICK_UTILITY_HPP_
