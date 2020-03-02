// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef QUICK_TYPE_TRAITS_FUNCTION_TYPE_HPP_
#define QUICK_TYPE_TRAITS_FUNCTION_TYPE_HPP_

#include <type_traits>

namespace quick {


template<typename...> using void_t = void;


template <typename T, typename D=void>
struct function_type_impl {};

// for function pointers
template <typename ReturnType, typename... Args>
struct function_type_impl<ReturnType (*)(Args...)>  {
  using type = std::function<ReturnType (Args...)>;
};

// For generic types that are functors, delegate to its 'operator()'
template <typename T>
struct function_type_impl<T, void_t<decltype(&T::operator())>> {
    using type = typename function_type_impl<decltype(&T::operator())>::type;
};

// for pointers to member function
template <typename ClassType, typename ReturnType, typename... Args>
struct function_type_impl<ReturnType(ClassType::*)(Args...) const> {
  using type = std::function<ReturnType (Args...)>;
};

// for pointers to member function
template <typename ClassType, typename ReturnType, typename... Args>
struct function_type_impl<ReturnType(ClassType::*)(Args...) > {
  using type = std::function<ReturnType (Args...)>;
};

template<typename T>
using function_type = typename function_type_impl<T>::type;


}  // namespace quick

namespace qk = quick;


#endif  // QUICK_TYPE_TRAITS_FUNCTION_TYPE_HPP_
