// Copyright: ThoughtSpot Inc. 2019
// Author: Mohit Saini (mohit.saini@thoughtspot.com)

#ifndef TS_HASHED_SEQUENTIAL_COUNTER_HPP_
#define TS_HASHED_SEQUENTIAL_COUNTER_HPP_

#include <utility>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <vector>
#include <memory>
#include <functional>
#include "ts/hash.hpp"
#include "ts/stl_utils.hpp"

namespace ts {
// Nesting for incomplete utilities. Once all the TODOs are fixed, it can be
// added in `ts::`.
namespace incomplete {

// HashedSequentialCounter can be used for assigning unique integers to
// complex objects.
// Example:
//  HashedSequentialCounter<string> hsc;
//  cout << hsc["abc"] << endl;  // output = 0
//  cout << hsc["abd"] << endl;  // output = 1
//  cout << hsc["abc"] << endl;  // output = 0
//  cout << hsc["abe"] << endl;  // output = 2
//  cout << hsc["abd"] << endl;  // output = 1
//  cout << hsc["abqz"] << endl; // output = 3
//  cout << hsc["abd"] << endl;  // output = 1
//  cout << hsc["abe"] << endl;  // output = 2


// ToDo(Mohit): 1. Make this counter std container compliant so that general
//                 STL features can be directly used. ex: iterator, insert etc.
//              2. Write test cases and create reference page.
template<class Key,
         class Hasher = ts::Hash<Key>,  // NOLINT
         class KeyEqual = std::equal_to<Key>,
         class Allocator = std::allocator<Key>>
class HashedSequentialCounter {
 private:
  using BaseMap = std::unordered_map<Key, int, Hasher, KeyEqual, Allocator>;

 public:
  HashedSequentialCounter(): counter_(0) {};
  explicit HashedSequentialCounter(int counter_seed): counter_(counter_seed) {};
  bool Insert(Key k) {
    if (not ts::ContainsKey(counter_map_, k)) {
      counter_map_[k] = counter_++;
      return true;
    }
    return false;
  }
  int operator[](Key k) {
    return InsertIfNotExists(k);
  }
  int at(Key k) const {
    return counter_map_.at(k);
  }

  // ToDo(Mohit): delete this and define .begin(), .find(), .end() so that
  //              ts::ContainsKey can be used directly.
  bool DoesKeyExists(Key k) {
    return ts::ContainsKey(counter_map_, k);
  }
  BaseMap& GetMap() {
    return counter_map_;
  }

 private:
  int InsertIfNotExists(Key k) {
    if (ts::ContainsKey(counter_map_, k)) {
      return counter_map_.at(k);
    } else {
      return counter_map_[k] = counter_++;
    }
  }
  int counter_ = 0;
  BaseMap counter_map_;
};
}  // namespace incomplete

using incomplete::HashedSequentialCounter;

}  // namespace ts

#endif  // TS_HASHED_SEQUENTIAL_COUNTER_HPP_
