// Copyright: ThoughtSpot Inc. 2019
// Author: Mohit Saini (mohit.saini@thoughtspot.com)

#ifndef TS_TUPLE_HPP_
#define TS_TUPLE_HPP_

#include <tuple>
#include <utility>

namespace ts {


template <typename Tuple, size_t x, size_t ...xs>
constexpr auto GetTupleTailHelper(const Tuple &t,
                                  std::index_sequence<x, xs...>) {
  return std::make_tuple(std::get<xs>(t)...);
}

// Sample input output :
// 1. TupleTail(tuple(11, true, "mohit", 44.5})) =
//    tuple(true, "mohit", 44.5)
// 2. TupleTail(tuple<>) => compilation error (Template Substitution Failure)
template <typename Tuple>
constexpr auto GetTupleTail(const Tuple &t) {
  constexpr size_t tuple_size = std::tuple_size<Tuple>::value;
  return GetTupleTailHelper(t, std::make_index_sequence<tuple_size>());
}



}  // namespace ts


#endif  // TS_TUPLE_HPP_
