
quick::variant
--------------------------
Defined in header `<quick/variant.hpp>`

```C++
template< class... Types >
class variant;
```

The class template `quick::variant` represents a type-safe union. An instance of std::variant at any given time either holds a value of one of its alternative types, or no value.

As with unions, if a variant holds a value of some object type T, the object representation of T is allocated in heap with default allocator. sizeof a variant object will always be `std::size_t`.

A variant is not permitted to hold references, arrays, or the type void. A variants can be empty.

`quick::variant` is always be default constructible irrespective of it's types. A default constructed variant doesn't construct any of it's type. Hence default construction of `quick::variant` is feasible even if types are forward declared.

`Types...` can have repeated types. In `quick::variant`, active type is accessed by it's index  in `Types...` pack, not by exact type (unlike `std::variant` and `boost::variant`).

```C++
quick::variant<int, string, vector<quick::variant<int, string>>> v;
v.at<0>() = 11;
v.at<1>() = "Abc";
v.at<2>().resize(3);
v.at<2>().at(0).at<2>() = "Xyz";
```


Member Functions
-----------------------------------

## variant::selected_type( ) const
- Returns the currently active type. Possible values are {0, 1, .... `sizeof...(Types)-1` }
-  By default no type is active. undefined selected_type is represented by `sizeof...(Types)`.

## variant::initialized( ) const
- Returns true if and only if one of the variant's types is active type. By default no type is active.

## variant::variant( )
- Default constructor. Always default construable.
- A default constructed variant is uninitialized. i.e. none of it's type is constructed.


## variant::at\<index\>(Args... args)
```C++
template<std::size_t i, typename... Args>
Type_i& variant::at<i>(Args... args);
```
- Return Type: `i'th` type from variant's types. (`Type_i`)
- If variant is not initialized or currently selected_type is not equal to `i`, then destroys the active object  and default construct the object of type `Type_i` by forwarding the supplied arguments `args...`, and update the `selected_type`.
- if variant is initialized and currently selected_type is equal to `i` then ignores the arguments `args...`.
- In both cases, return the non-const reference of active object.

## variant::at\<index\>() const
```C++
template<std::size_t i>
const Type_i& variant::at<i>() const;
```
- Return Type: `i'th` type from variant's types. (`Type_i`)
- If variant is not initialized or currently selected_type is not equal to `i`, then throw bad_access exception.
- if variant is initialized and currently selected_type is equal to `i` then return the const reference of active object.

## variant::clear( )
- Destroy the active object if any.
- variant will be said uninitialized after this operation.
- selected_type will be `sizeof...(Types)`

## variant::variant(const variant& other)
- If `other` variant is initialized, then set the current selected_type to `other.selected_type` and copy construct the selected type from other to `this`. 
- if `other` variant is not initialized, then `variant::clear()` itself.
- Defined if and only if all the types are copy constructible.

## variant::variant(variant&& other)
- If `other` variant is initialized, then set the current selected_type to `other.selected_type` and move construct the selected type from other to `this`. 
- if `other` variant is not initialized, then do `variant::clear()` itself.
- Defined if and only if all the types are move constructible.

## variant::operator=(const variant& other)
- If `other` variant is initialized.
  - If current selected type is not same as other.selected_type then destroys currently active object and set the  selected_type type to `other.selected_type` and copy construct the selected type from other's selected type.
  - if current selected type is same as other.selected_type then copy assign selected object to selected object of other variant.
- if `other` is not initialized then `variant::clear()` itself.
- Defined if an only if all the types are copy constructible as well as copy assignable.

## variant::operator=(variant&& other)
- If `other` variant is initialized.
  - If current selected type is not same as other.selected_type then destroys currently active object and set the  selected_type type to `other.selected_type` and move construct the selected type from other's selected type.
  - if current selected type is same as other.selected_type then move assign selected object to selected object of other variant.
- if `other` is not initialized then `variant::clear()` itself.
- Defined if an only if all the types are move constructible as well as move assignable.


## variant::~variant()
- Same as `variant::clear()`, i.e. destroys if there is any active object.



## Example
- [Example-1](../tests/variant_test.cpp)

