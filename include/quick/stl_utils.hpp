// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef QUICK_STL_UTILS_HPP_
#define QUICK_STL_UTILS_HPP_

#include <utility>
#include <unordered_set>
#include <set>
#include <vector>
#include <map>
#include <string>
#include <sstream>

namespace quick {
namespace detail {


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

template<typename Container1, typename Container2>
void InsertToVector(const Container1& input, Container2* output) {
  output->insert(output->end(), input.begin(), input.end());
}

template<typename Container1, typename Container2>
void InsertToVectorMoving(Container1&& input, Container2* output) {
  output->insert(output->end(),
                 std::make_move_iterator(input.begin()),
                 std::make_move_iterator(input.end()));
}

template<typename Container1, typename Container2>
void InsertToSet(const Container1& input, Container2* output) {
  output->insert(input.begin(), input.end());
}

template<typename Container1, typename Container2>
void InsertToMap(const Container1& input, Container2* output) {
  output->insert(input.begin(), input.end());
}


template<typename Container1, typename Container2>
Container1 SetUnion(const Container1& input1, const Container2& input2) {
  Container1 output;
  InsertToSet(input1, &output);
  InsertToSet(input2, &output);
  return output;
}

template<typename Container1, typename Container2>
Container1 SetMinus(const Container1& input1, const Container2& input2) {
  Container1 output;
  for (auto& item : input1) {
    if (not quick::ContainsKey(input2, item)) {
      output.insert(item);
    }
  }
  return output;
}

template<typename Container>
auto ToSet(const Container& input) {
  std::set<typename Container::value_type> output(input.begin(),
                                                  input.end());
  return output;
}

template<typename Container>
auto ToMap(const Container& input) {
  using OutputType = std::map<typename Container::key_type,
                              typename Container::mapped_type>;
  OutputType output(input.begin(), input.end());
  return output;
}

template<typename Container>
auto ToUnorderedSet(
    const Container& input) {
  std::unordered_set<typename Container::value_type> output(input.begin(),
                                                            input.end());
  return output;
}


template<typename T>
std::string StringJoin(const T& container, const std::string& join_by = " ") {
  std::ostringstream oss;
  bool is_first = true;
  for (auto& item : container) {
    if (not is_first) {
      oss << join_by;
    } else {
      is_first = false;
    }
    oss << item;
  }
  return oss.str();
}

}  // namespace quick

namespace qk = quick;

#endif  // QUICK_STL_UTILS_HPP_
