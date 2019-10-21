// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef QUICK_TYPE_TRAITS_HPP_
#define QUICK_TYPE_TRAITS_HPP_

#include <type_traits>

namespace quick {
namespace detail {
}  // namespace detail

template<typename...> using void_t = void;


template<template<class...> class Template, class AlwaysVoid, class... Args>
struct test_specialization_impl: std::false_type {};

template<template<class...> class Template, class... Args>
struct test_specialization_impl<Template,
                                void_t<Template<Args...>>,
                                Args...>: std::true_type {};

template<template<class...> class Template, class... Args>
using test_specialization = test_specialization_impl<Template, void, Args...>;



template<class Fallback, template<class...> class Template, class AlwaysVoid, class... Args>
struct specialize_if_can_impl: public Fallback {};

template<class Fallback, template<class...> class Template, class... Args>
struct specialize_if_can_impl<Fallback,
                              Template,
                              void_t<Template<Args...>>,
                              Args...>: public Template<Args...> {};

template<class Fallback, template<class...> class Template, class... Args>
using specialize_if_can = specialize_if_can_impl<Fallback,
                                                 Template,
                                                 void,
                                                 Args...>;



template<typename Spec, template<typename...> class Template>
struct is_specialization: std::false_type {};

template<template<typename...> class Template, typename... Args>
struct is_specialization<Template<Args...>, Template>: std::true_type {};





}  // namespace quick

namespace qk = quick;


#endif  // QUICK_TYPE_TRAITS_HPP_
