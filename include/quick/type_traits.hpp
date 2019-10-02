// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef QUICK_TYPE_TRAITS_HPP_
#define QUICK_TYPE_TRAITS_HPP_

#include <type_traits>

namespace quick {
namespace detail {
}  // namespace detail

template<typename T, typename...> using first_type = T;

template<typename...> using void_t = void;


template<typename Spec, template<typename...> class Template>
struct is_specialization: std::false_type {};

template<template<typename...> class Template, typename... Args>
struct is_specialization<Template<Args...>, Template>: std::true_type {};





}  // namespace quick

namespace qk = quick;


#endif  // QUICK_TYPE_TRAITS_HPP_
