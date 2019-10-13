// Copyright: ThoughtSpot Inc. 2019
// Author: Mohit Saini (mohit.saini@thoughtspot.com)

#ifndef QUICK_UNORDERED_MAP_HPP_
#define QUICK_UNORDERED_MAP_HPP_

#include <unordered_map>
#include <utility>
#include <memory>
#include <functional>

#include "quick/hash.hpp"

namespace quick {

template <class Key,
          typename T,
          typename Hasher = quick::hash<Key>,
          typename Pred = std::equal_to<Key>,
          typename Alloc = std::allocator<std::pair<const Key, T>>>
using unordered_map = std::unordered_map<Key,
                                          T,
                                          Hasher,
                                          Pred,
                                          Alloc>;

}  // namespace quick


#endif  // QUICK_UNORDERED_MAP_HPP_
