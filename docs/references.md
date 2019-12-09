# Welcome to Quick
Quick is collection of highly reusable C++ utilities, which are not available in `boost` or `std`.
```c++
namespace qk = quick;
```

quick::variant
--------------------------
Defined in `<quick/variant.hpp>`

The utility `quick::variant` serves the purpose of `std::variant` or `boost::variant`. However it differs in usage patterns.  Learn More.

quick::hash
--------------------------
Defined in `<quick/hash.hpp>`

The utility `quick::hash<T>` extends the  `std::hash<T>` and support the default hashing for: `std::vector`, `std::list`,  `std::set`, `std::tuple`, `std::pair`, `enum types`, `std::map`, `Custom type T having "std::size_t T::GetHash() const" member` . Learn More.

quick::unordered_set
--------------------------
Defined in `<quick/unordered_set.hpp>`

The utility `quick::unordered_set<..>` in an alias to `std::unordered_set<..>` with `default hasher = quick::hash<Key>`.
Learn More.


quick::unordered_map
--------------------------
Defined in `<quick/unordered_map.hpp>`

The utility `quick::unordered_map<..>` in an alias to `std::unordered_map<..>` with `default hasher = quick::hash<Key>`. Learn More.


quick::DebugStream
--------------------------
Defined in `<quick/debug_stream.hpp>`

`class quick::DebugStream` is super intuitive and easy to use utility for constructing human readable representation of complex and deeply nested C++ objects. Learn More.

quick::ByteStream
--------------------------
Defined in `<quick/byte_stream.hpp>`

`class quick::ByteStream` is super intuitive, safe, reliable and easy to use utility for byte serialisation and deserialization of complex and deeply nested C++ objects. Learn More.

`#include <quick/debug.hpp>`
--------------------------
 The utility `quick/debug.hpp` populates the
 `operator<<(std::ostream&, const T&)` for commonly used types, ex: `std::map`, `std::tuple`, `std::pair`, `std::tuple`, `std::list`, `std::set`, `std::unordered_set`, `std::unordered_map`, 
 `Custom type T having "void T::DebugStream(quick::DebugStream& ds) const" member`, 
 `Custom type T having "std::string T::DebugString() const" member`. 

quick::GetEpochMicroSeconds
--------------------------
Defined in `<quick/time.hpp>`

`int64_t quick::GetEpochMicroSeconds()` - Returns the micro seconds elapsed since epoch. Learn More.

quick::MicroSecondTimer
--------------------------
Defined in `<quick/time.hpp>`

`class quick::MicroSecondTimer` - A timer utility useful for measuring the time taken in the processing of a task. Learn More.

quick::test_specialization
--------------------------
Defined in `<quick/type_traits.hpp>`

```C++
template<template<class...> class Template, class... Args>
class test_specialization;
```
`quick::test_specialization` is a type trait used for testing if a template can be specialised on given `Args...`. 

quick::ContainsKey
-------------------------
Defined in `<quick/container_utils.hpp>`

`bool ContainsKey(const C& container, const V& value);`
Checks if a key is present in the given container.

------------------------------------------------------------------


ToDo(*) : Document all other utilities as well.
