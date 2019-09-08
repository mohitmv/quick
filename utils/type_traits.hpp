// Copyright: ThoughtSpot Inc. 2019
// Author: Mohit Saini (mohit.saini@thoughtspot.com)

#ifndef TS_TYPE_TRAITS_HPP_
#define TS_TYPE_TRAITS_HPP_

#include <boost/type_traits/is_detected.hpp>
#include <tuple>
#include <utility>

namespace ts {


// this type_trait is defined since C++17, so we can delete it when we move to
// C++17.
// This is a dummy template, which can take arbitary number of arguments
// but remains an alias for `void`, which is helpful in building SFINAE
// utilities.
// https://en.cppreference.com/w/cpp/types/void_t
//
// ToDo(Mohit): Create a common document to be shared across ThoughtSpot,
// where everyone can list all the changes we need to make while migrating
// from C++14 to C++17.
// Add this point in that document : Remove ts::void_t from ts's type traits.
// Use std::void_t instead.
template<typename...> using void_t = void;


// Given a type, it checks whether DebugString member is defined for it or not.
template<typename T>
struct HasDebugString {
  template<typename T2> using has_debug_string_t = decltype(&T2::DebugString);
  static const bool value = boost::is_detected<has_debug_string_t, T>::value;
};

// Given a type, it checks whether GetHash member is defined for it or not.
// GetHash is special member function, which can be defined for a custom class.
// If GetHash is defined, class can be used as key of ts::unordered_map.
template<typename T>
struct HasGetHash {
  template<typename T2> using has_get_hash_t = decltype(&T2::GetHash);
  static const bool value = boost::is_detected<has_get_hash_t, T>::value;
};





}  // namespace ts


#endif  // TS_TYPE_TRAITS_HPP_
