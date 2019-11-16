// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)
//
// Extends std::ostream for commonly used objects, ex: std::vector, std::pair,
// std::map, std::set etc.
// It is very helpful for printing complex objects.
// google's LOG(INFO), cout, cerr, std::ostringstream are extended from
// std::ostream only. So we can easily use it in logging as well as creating
// DebugString() method in custom class.

// How to use ?
// vector<vector<unordered_map<int, string>>> a;
// a = {{{{10, "Mohit"}, {20, "Saini"}}, {}}, {}};
// cout << a;
//
// Output will look like:
// [[{10: Mohit, 20: "Saini"}, {}], []];
//
//
// Another use case: for creating DebugString.
// class CarCompany {
//    using CarModel = pair<int, string>;
//    unordered_map<CarModel, int, PairHash> model_stock_map;
//    unordered_set<int> customer_id_set;
//    string DebugString() const {
//      std::ostringstream oss;
//      oss << "model_stock_map = " << model_stock_map << "\n";
//      oss << "customer_id_set = " << customer_id_set << "\n";
//      return oss.str();
//    }
// }


#ifndef QUICK_DEBUG_HPP_
#define QUICK_DEBUG_HPP_

#include <iostream>  // NOLINT
#include <map>
#include <utility>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <sstream>
#include <tuple>
#include <list>

#include <quick/debug_stream.hpp>

namespace quick {
namespace detail {

// Assumes `ostream << element` is defined for elements of input.
template<typename Container>
std::ostream& PrintContainer(std::ostream& os, const Container& input) {
  os << "[";
  bool is_first_item = true;
  for (auto& item : input) {
    os << (is_first_item ? "" : ", ") << item;
    is_first_item = false;
  }
  os << "]";
  return os;
}

// Assumes both `ostream << key` and `ostream << value` is defined for the
// input map.
template<typename MapContainer>
std::ostream& PrintMap(std::ostream& os, const MapContainer& input) {
  os << "{";
  bool is_first_item = true;
  for (auto& item : input) {
    os << (is_first_item ? "" : ", ") << item.first << ": " << item.second;
    is_first_item = false;
  }
  os << "}";
  return os;
}

template<typename... Ts>
inline void PrintTupleImpl(std::ostream&,
                           const std::tuple<Ts...>&,
                           std::index_sequence<>) {}

template<typename... Ts, std::size_t index_head, std::size_t... index_tail>
inline void PrintTupleImpl(std::ostream& os,
                           const std::tuple<Ts...>& input,
                           std::index_sequence<index_head, index_tail...>) {
  if (index_head > 0) {
    os << ", ";
  }
  os << std::get<index_head>(input);
  PrintTupleImpl(os, input, std::index_sequence<index_tail...>());
}

template<typename... Ts>
void PrintTuple(std::ostream& os, const std::tuple<Ts...>& input) {
  os << "(";
  constexpr std::size_t num_elements
                            = std::tuple_size<std::tuple<Ts...>>::value;
  PrintTupleImpl(os, input, std::make_index_sequence<num_elements>());
  os << ")";
}



// In case of success: output type is an instance of std::true_type
// In case of failure: Either fails while substituting or output type is an
//                     instance of std::false_type
// Need a wrapper to cover substitution failure ? use this =>
// quick::specialize_if_can<std::false_type, HasDebugString, T>::value
// It will fall back on `std::false_type` in the case of substitution failure.
template<typename T>
using HasDebugString = std::is_same<
                          std::string,
                          decltype(std::declval<const T&>().DebugString())>;

template<typename T>
using HasDebugStream = std::is_same<void,
                                    decltype(
                                      std::declval<const T&>().DebugStream(
                                        std::declval<quick::DebugStream&>()))>;

template<typename T>
using IsOstreamDefinedForPair = decltype(
      std::operator<<(std::declval<std::ostream&>(), std::declval<const T&>()));



}  // namespace detail

template<typename T>
std::string ToString(const T& input) {
  std::ostringstream oss;
  oss << input;
  return oss.str();
}

}  // namespace quick

namespace std {


template<typename T>
std::enable_if_t<(quick::detail::HasDebugString<T>::value &&
                  not(quick::specialize_if_can<std::false_type,
                                               quick::detail::HasDebugStream,
                                               T>::value)), ostream>&
operator<<(ostream& os, const T& input) {
  os << input.DebugString();
  return os;
}

template<typename T>
std::enable_if_t<quick::detail::HasDebugStream<T>::value, ostream>&
operator<<(ostream& os, const T& input) {
  os << quick::DebugStream(input).str();
  return os;
}

template<typename T>
std::enable_if_t<(quick::is_specialization<T, std::pair>::value &&
  not quick::test_specialization<quick::detail::IsOstreamDefinedForPair,
                                 T>::value)
, ostream>&
operator<<(ostream& os, const T& input) {
  os << "(" << input.first << ", " << input.second << ")";
  return os;
}


template<typename T>
std::enable_if_t<(quick::is_specialization<T, std::vector>::value ||
                  quick::is_specialization<T, std::list>::value ||
                  quick::is_specialization<T, std::unordered_set>::value ||
                  quick::is_specialization<T, std::set>::value), ostream>&
operator<<(ostream& os, const T& input) {
  return quick::detail::PrintContainer(os, input);
}

template<typename T>
std::enable_if_t<(quick::is_specialization<T, std::map>::value ||
                  quick::is_specialization<T, std::unordered_map>::value),
                 ostream>&
operator<<(ostream& os, const T& input) {
  return quick::detail::PrintMap(os, input);
}


template<typename... Ts>
ostream& operator<<(ostream& os, const std::tuple<Ts...>& input) {
  quick::detail::PrintTuple(os, input);
  return os;
}





}  // namespace std

namespace qk = quick;

#endif  // QUICK_DEBUG_HPP_
