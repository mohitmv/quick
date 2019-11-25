// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef QUICK_UNORDERED_SET_HPP_
#define QUICK_UNORDERED_SET_HPP_

#include <unordered_set>
#include <utility>
#include <memory>
#include <functional>

#include "quick/hash.hpp"

namespace quick {

template <class Key,
          typename Hasher = quick::hash<Key>,
          typename Pred = std::equal_to<Key>,
          typename Alloc = std::allocator<Key>>
using unordered_set = std::unordered_set<Key, Hasher, Pred, Alloc>;

}  // namespace quick


#endif  // QUICK_UNORDERED_SET_HPP_
