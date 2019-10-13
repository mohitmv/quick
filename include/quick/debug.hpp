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


template<typename T, typename...> using FirstType = T;


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
quick::detail::FirstType<ostream, decltype(&T::DebugString)>& operator<<(
    ostream& os,
    const T& input) {
  os << input.DebugString();
  return os;
}

// Prints a std::pair
template<typename T1, typename T2>
ostream& operator<<(ostream& os, const std::pair<T1, T2>& input) {
  os << "(" << input.first << ", " << input.second << ")";
  return os;
}

template<typename... Ts>
ostream& operator<<(ostream& os, const std::vector<Ts...>& input) {
  return quick::detail::PrintContainer(os, input);
}

template<typename... Ts>
ostream& operator<<(ostream& os, const std::list<Ts...>& input) {
  return quick::detail::PrintContainer(os, input);
}

template<typename... Ts>
ostream& operator<<(ostream& os, const std::unordered_set<Ts...>& input) {
  return quick::detail::PrintContainer(os, input);
}

template<typename... Ts>
ostream& operator<<(ostream& os, const std::set<Ts...>& input) {
  return quick::detail::PrintContainer(os, input);
}

template<typename... Ts>
ostream& operator<<(ostream& os, const std::map<Ts...>& input) {
  return quick::detail::PrintMap(os, input);
}

template<typename... Ts>
ostream& operator<<(ostream& os, const std::unordered_map<Ts...>& input) {
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
