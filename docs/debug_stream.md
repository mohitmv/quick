

quick::DebugStream
--------------------------
Defined in header `<quick/debug_stream.hpp>`

```C++
class DebugStream;
```
`quick::DebugStream` is super intuitive and easy to use utility for constructing human readable representation of complex and deeply nested C++ objects. 

`DebugStream` is somewhat similar to `std::cout` ,  or `std::ostringstream`, however it's different from simple string stream, because it maintains a tree structure ( Json like structure ), which is used for printing the object in a structured form similar to prettified Json.
`DebugStream` reserves a few special character pairs (example: "{" and "}" ) for branching instructions.

## Example 1
Consider this simple code for constructing human readable string representation of  `Person` object.
```C++
quick::DebugStream ds;
Person p;
ds << "About Me = " << "{"
   << "name = " << p.name << "\n"
   << "age = " << p.age << "\n"
   << "parents = " << "{"
   << "mother = " << "{"
   << "name = " << p.mother.name << "\n"
   << "age = " << p.mother.age
   << "}"
   << "father = " << "{"
   << "name = " << p.father.name << "\n"
   << "age = " << p.father.age
   << "}"
   << "}"
   << "siblings = " << p.signlings_list << "\n"
   << "}";
cout << ds.str() << endl;
```
It produces following prettified and well-indented string representation. 
```
About Me = {
  name = Mohit
  age = 123
  parents = {
    mother = {
      name = AA
      age = 3342
    }
    father = {
      name = BB
      age = 334
    }
  }
  siblings = [QQ, AAs, IIo]
}
```

## Example 2

```C++
class Car {
  struct SeatDetails {
    int foam_price;
    int quality_score;
    void DebugStream(ts::DebugStream& ds) const {
      ds << "foam_price: " << foam_price << '\n';
      ds << "quality_score:"  << quality_score;
    }
  };
  int distance_travelled;
  string model_number;
  vector<SeatDetails> seats;
  void DebugStream(ts::DebugStream& ds) const {
    ds << "Seats : " << seats;
    ds << "model_number : " << model_number << '\n';
    ds << "distance_travelled : " << distance_travelled << '\n';
  }
};

class CarCompany {
  vector<Car> cars;
  unordered_map<int, pair<int, string>> model_details_map;
  void DebugStream(ts::DebugStream& ds) const {
    ds << "Cars = " << cars;
    ds << "model_details_map = " << model_details_map << '\n';
  }
};
CarCompany car_company(..., ..., ...);
cout << qk::DebugStream(car_company).str();
```
It produces following prettified and well-indented string representation.

```
Cars = {
  [0]: {
    Seats : {
      [0]: {
        foam_price: 110
        quality_score: 444
      }
      [1]{
        foam_price: 1888
        quality_score: 444666
      }
    }
    model_number: 4545
    distance_travelled: 8855
  }
  [1]: {
    Seats : {
      [0]{
        foam_price: 110
        quality_score: 444
      }
      [1]{
        foam_price: 1888
        quality_score: 444666
      }
    }
    model_number: 4545
    distance_travelled: 8855
  }
}
model_details_map = {
  11: {
    (33, "KYZ44")
  }
  21: {
    (313, "KYZ9966")
  }
}
```

## `operator<<` overloading for `DebugStream`

`operator<<(quick::DebugStream&, const T&)` is defined for  `primitive types` , `std::string`, `const char*`, `std::vector`, `std::pair`, `std::tuple`, `std::map`, `std::set`, `std::unordered_map`, `std::unordered_set`, `Any type T having "void T::DebugStream(quick::DebugStream&) const" member function`. 


Member Functions
-----------------------------------

## DebugStream::DebugStream()
- Creates default instance of DebugStream.


## DebugStream::DebugStream(const T&)
```C++
template<typename T>
DebugStream::DebugStream(const T& x);
```
- Creates a default instance of DebugStream and run `operator<<(*this, x);`
- Defined only if T is one of supported type for  `operator<<(DebugStream&, const T&)`.


## DebugStream::str( ) const
- Returns the prettified string.


Test Case
-------------------
- [Unit Tests](../tests/debug_stream_test.cpp)

