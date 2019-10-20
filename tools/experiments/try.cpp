#include <iostream>

#include <unordered_set>
#include <utility>
#include <memory>
#include <functional>

namespace quick {

template<typename T>
struct hash: std::hash<T> {};

template <class Key,
          typename Hasher = quick::hash<Key>,
          typename Pred = std::equal_to<Key>,
          typename Alloc = std::allocator<Key>>
using unordered_set = std::unordered_set<Key, Hasher, Pred, Alloc>;

}


namespace quick {

template <typename ContainerType, class KeyType>
bool ContainsKey28(const ContainerType& container, const KeyType& key) {
  return container.find(key) != container.end();
}

}


inline bool F2() {
  bool a = ContainsKey28(quick::unordered_set<int>{3}, 3);
  return a;
}


using std::cout;
using std::endl;

int main() {
  cout << "All Good 4" << endl;
  return 0;
}