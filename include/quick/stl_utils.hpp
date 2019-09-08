// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef QUICK_STL_UTILS_HPP_
#define QUICK_STL_UTILS_HPP_

#include <utility>
#include <unordered_set>
#include <set>
#include <vector>

namespace quick {
namespace detail {

template<typename MapContainer>
using MapKeysSet = std::unordered_set<typename MapContainer::key_type>;


}

template<typename T, typename... Ts>
std::vector<T> ArgsToVector(T arg, Ts... args) {
  return std::vector<T>({arg, args...});
}

template <typename ContainerType, class KeyType>
bool ContainsKey(const ContainerType& container, const KeyType& key) {
  return container.find(key) != container.end();
}

template<typename MapContainer, typename OutputContainer>
void STLGetKeys(const MapContainer& input, OutputContainer* output) {
  for (auto& item : input) {
    output->insert(item.first);
  }
}

template<typename MapContainer>
auto STLGetKeys(const MapContainer& input) {
  std::unordered_set<typename MapContainer::key_type> output;
  STLGetKeys(input, &output);
  return output;
}



}  // namespace quick

namespace qk = quick;

#endif  // QUICK_STL_UTILS_HPP_
