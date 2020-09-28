// Copyright: 2020 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

// Given a callable type, it creates a `std::function<R(T, U, ..)>` type to
// hold the callable.
// Callable type can be any of
//   1. Function Pointer
//   2. Member Function Pointer
//   3. Functor (lambda or otherwise) with exactly one `operator()` member
//   4. std::function<..>
// Usage:
//   look at `tests/type_traits/function_type_test.cpp` for interface details.

#ifndef QUICK_TYPE_TRAITS_FUNCTION_TYPE_HPP_
#define QUICK_TYPE_TRAITS_FUNCTION_TYPE_HPP_

#include <type_traits>
#include <functional>

namespace quick {


template<typename...> using void_t = void;


template <typename T, typename D=void>
struct function_type_impl {};

template <typename ReturnType, typename... Args>
struct function_type_impl<ReturnType (*)(Args...)>  {
  using type = std::function<ReturnType (Args...)>;
};

template <typename T>
struct function_type_impl<T, void_t<decltype(&T::operator())>> {
    using type = typename function_type_impl<decltype(&T::operator())>::type;
};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_type_impl<ReturnType(ClassType::*)(Args...) const> {
  using type = std::function<ReturnType (Args...)>;
};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_type_impl<ReturnType(ClassType::*)(Args...) > {
  using type = std::function<ReturnType (Args...)>;
};

template<typename T>
using function_type = typename function_type_impl<T>::type;


}  // namespace quick

namespace qk = quick;


#endif  // QUICK_TYPE_TRAITS_FUNCTION_TYPE_HPP_
