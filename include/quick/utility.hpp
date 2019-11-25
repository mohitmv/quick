// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef QUICK_UTILITY_HPP_
#define QUICK_UTILITY_HPP_

#include <utility>
#include <tuple>
#include <functional>



// std::apply borrowed from C++17
namespace quick {
namespace detail {
template <class F, class Tuple, std::size_t... I>
constexpr decltype(auto) apply_impl(F&& f,
                                    Tuple&& t,
                                    std::index_sequence<I...>) {
  return f(std::get<I>(std::forward<Tuple>(t))...);
}
}  // namespace detail

template <class F, class Tuple>
constexpr decltype(auto) apply(F&& f, Tuple&& t) {
  using std::size_t;
  constexpr size_t siz = std::tuple_size<std::remove_reference_t<Tuple>>::value;
  return detail::apply_impl(std::forward<F>(f),
                            std::forward<Tuple>(t),
                            std::make_index_sequence<siz>{});
}

}  // namespace quick

namespace qk = quick;


#endif  // QUICK_UTILITY_HPP_
