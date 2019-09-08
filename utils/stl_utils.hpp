// Copyright: ThoughtSpot Inc. 2019
// Author: Mohit Saini (mohit.saini@thoughtspot.com)

#ifndef TS_STL_UTILS_HPP_
#define TS_STL_UTILS_HPP_

#include <utility>
#include <unordered_set>
#include <set>
#include <vector>

namespace ts {


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
  for (auto& item: input) {
    output->insert(item.first);
  }
}

template<typename MapContainer>
std::unordered_set<typename MapContainer::key_type>
    STLGetKeysReturnUnorderedSet(const MapContainer& input) {
  std::unordered_set<typename MapContainer::key_type> output;
  STLGetKeys(input, &output);
  return output;
}

template<typename Container1, typename Container2>
void STLInsertToVector(const Container1& input, Container2* output) {
  output->insert(output->end(), input.begin(), input.end());
}

template<typename Container1, typename Container2>
void STLInsertToSet(const Container1& input, Container2* output) {
  output->insert(input.begin(), input.end());
}

template<typename Container1, typename Container2>
void STLMoveAndInsertToVector(Container1& input,
                                     Container2* output) {
  output->insert(output->end(),
                 std::make_move_iterator(input.begin()),
                 std::make_move_iterator(input.end()));
}

template<typename Container1, typename Container2>
void STLInverseOfMap(const Container1& input, Container2* output) {
  for (auto& item: input) {
    output->insert(std::make_pair(item.second, item.first));
  }
}

template<typename Set1, typename Set2, typename SetOutput>
void STLSetIntersection(const Set1& set1,
                               const Set2& set2,
                               SetOutput* output) {
  for (auto& item: set1) {
    if (ContainsKey(set2, item)) {
      output->insert(item);
    }
  }
}

template< class InputContainer, class OutputContainer, class UnaryOperation>
void STLTransform(InputContainer input,
                         UnaryOperation unary_op,
                         OutputContainer* output) {
  for (auto& item: input) {
    output->insert(unary_op(item));
  }
}

template< class InputContainer, class UnaryOperation>
InputContainer STLTransform(InputContainer input,
                                   UnaryOperation unary_op) {
  InputContainer output;
  // ToDo(Mohit): Learn how to use perfect forwarding in C++ and use here.
  STLTransform(input, unary_op, &output);
  return output;
}

template< class InputContainer, class OutputContainer, class UnaryOperation>
void STLTransformMap(InputContainer input,
                            UnaryOperation unary_op,
                            OutputContainer* output) {
  for (auto& item: input) {
    output->insert(unary_op(item));
  }
}


template<typename Set1, typename Set2>
std::unordered_set<typename Set1::key_type> STLSetIntersection(
    const Set1& set1,
    const Set2& set2) {
  std::unordered_set<typename Set1::key_type> output;
  STLSetIntersection(set1, set2, &output);
  return output;
}

template<typename Set1, typename Set2>
std::unordered_set<typename Set1::key_type> STLSetDifference(
    const Set1& set1,
    const Set2& set2) {
  std::unordered_set<typename Set1::key_type> output;
  for (auto& item: set1) {
    if (not ContainsKey(set2, item)) {
      output.insert(item);
    }
  }
  return output;
}




}  // namespace ts

#endif  // TS_STL_UTILS_HPP_
