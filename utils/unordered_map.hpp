// Copyright: ThoughtSpot Inc. 2019
// Author: Mohit Saini (mohit.saini@thoughtspot.com)

#ifndef TS_UNORDERED_MAP_HPP_
#define TS_UNORDERED_MAP_HPP_

#include <unordered_map>
#include <utility>
#include <memory>
#include <functional>

#include "ts/hash.hpp"

namespace ts {

template <class Key,
          typename T,
          typename Hasher = ts::Hash<Key>,
          typename Pred = std::equal_to<Key>,
          typename Alloc = std::allocator<std::pair<const Key, T>>>
class unordered_map : public std::unordered_map<Key,
                                                T,
                                                Hasher,
                                                Pred,
                                                Alloc> {};

}  // namespace ts


#endif  // TS_UNORDERED_MAP_HPP_
