// Example program
#include <iostream>
#include <string>
#include <type_traits>


template<typename...> using void_t = void;

template<template<class...> class Template, class AlwaysVoid, class... Args>
struct test_specialization_impl: std::false_type {};

template<template<class...> class Template, class... Args>
struct test_specialization_impl<Template, void_t<Template<Args...>>, Args...>: std::true_type {};

template<template<class...> class Template, class... Args>
using test_specialization = test_specialization_impl<Template, void, Args...>;


using namespace std;

template<typename T>
struct S: std::true_type {};

struct P1 {};
struct P2 {
    void Member();
};

template<typename T>
using S2 = std::is_same<void, decltype(&T::Member)>;


int main() {

    cout << test_specialization<S, int>::value << endl;
    cout << test_specialization<S2, P1>::value << endl;
    cout << test_specialization<S2, P2>::value << endl;

}
