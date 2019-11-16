// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include "quick/stl_utils.hpp"

#include <iostream>
#include <unordered_map>
#include <type_traits>

#include "gtest/gtest.h"

using std::unordered_set;
using std::unordered_map;
using std::set;
using std::vector;

TEST(ContainsKey, Basic) {
  unordered_set<int> v = {10, 11, 12, 13};
  EXPECT_TRUE(qk::ContainsKey(v, 10));
  EXPECT_TRUE(quick::ContainsKey(v, 12));
  EXPECT_FALSE(qk::ContainsKey(v, 130));
  unordered_set<int> v2 = {};
  EXPECT_FALSE(quick::ContainsKey(v2, 124));
}

TEST(STLGetKeys, Basic) {
  unordered_map<int, int> m = {{10, 100}, {20, 200}};
  EXPECT_EQ(qk::STLGetKeys(m), (std::unordered_set<int>{10, 20}));
  EXPECT_EQ(qk::STLGetKeys(std::unordered_map<int, int>{}).size(), 0UL);
  std::set<int> results;
  qk::STLGetKeys(m, &results);
  EXPECT_EQ(results, (std::set<int> {10, 20}));
}

TEST(InsertToVector, Basic) {
  std::vector<int> a = {5, 66};
  std::vector<int> b = {76, 3, 66};
  qk::InsertToVector(b, &a);
  EXPECT_EQ(a.size(), 5UL);
  EXPECT_EQ(a, (vector<int> {5, 66, 76, 3, 66}));
  std::unordered_set<int> c = {11, 22};
  qk::InsertToVector(c, &a);
  std::unordered_set<int> d(a.begin() + 5, a.end());
  EXPECT_EQ(d, c);
}


TEST(InsertToVectorMoving, Basic) {
  std::vector<int> a = {5, 66};
  std::vector<int> b = {76, 3, 66};
  qk::InsertToVectorMoving(b, &a);
  EXPECT_EQ(a.size(), 5UL);
  struct A {
    int data;
    A(int data): data(data) {}  // NOLINT
    A(const A&) = delete;
    A& operator=(const A&) = delete;
    A(A&&) = default;
    A& operator=(A&&) = default;
  };
  std::vector<A> aa;
  aa.emplace_back(11);
  aa.emplace_back(22);
  std::vector<A> bb;
  bb.emplace_back(113);
  bb.emplace_back(114);
  bb.emplace_back(115);
  qk::InsertToVectorMoving(bb, &aa);
  EXPECT_EQ(aa.size(), 5UL);
  EXPECT_EQ(aa[4].data, 115);
}


TEST(SetUnion, Basic) {
  std::unordered_set<int> a = {11, 12, 13, 14};
  auto a2 = a;
  std::unordered_set<int> b = {44, 55, 66, 77};
  auto b2 = b;
  auto union_set = qk::SetUnion(a, b);
  EXPECT_EQ(a, a2);
  EXPECT_EQ(b, b2);
  EXPECT_EQ(union_set.size(), 8UL);
  EXPECT_TRUE(qk::ContainsKey(union_set, 55));
  EXPECT_TRUE(qk::ContainsKey(union_set, 13));
  EXPECT_EQ(qk::SetUnion(std::set<int> {10, 20},
                         std::unordered_set<int> {20, 30}),
            (std::set<int> {10, 20, 30}));
  EXPECT_EQ(qk::SetUnion(std::set<int> {10, 20}),
            (std::set<int> {10, 20}));



  EXPECT_EQ(qk::SetUnion(std::set<int> {10, 20},
                         std::unordered_set<int> {20, 30},
                         std::unordered_set<int> {120, 530},
                         std::unordered_set<int> {320, 330}),
            (std::set<int> {10, 20, 30, 120, 530, 320, 330}));
}


TEST(SetMinus, Basic) {
  unordered_set<int> a = {11, 22, 33, 44};
  unordered_set<int> b = {11, 22, 66, 77};
  unordered_set<int> c = qk::SetMinus(a, b);
  EXPECT_EQ(a, (unordered_set<int> {11, 22, 33, 44}));
  EXPECT_EQ(b, (unordered_set<int> {11, 22, 66, 77}));
  EXPECT_EQ(c, (unordered_set<int> {33, 44}));
}

TEST(ToSet, Basic) {
  unordered_set<int> a = {11, 22, 33, 44};
  set<int> b = {11, 22, 66, 77};
  vector<int> c = {30, 40, 50, 40};
  auto a2 = quick::ToSet(a);
  auto b2 = quick::ToSet(b);
  auto c2 = quick::ToSet(c);
  static_assert(std::is_same<decltype(a2), set<int>>::value, "Unexpected type");
  EXPECT_EQ(a2, (set<int> {11, 22, 33, 44}));
  EXPECT_EQ(b2, (set<int> {11, 22, 66, 77}));
  EXPECT_EQ(c2, (set<int> {30, 40, 50}));
}

TEST(ToUnorderedSet, Basic) {
  unordered_set<int> a = {11, 22, 33, 44};
  set<int> b = {11, 22, 66, 77};
  vector<int> c = {30, 40, 50, 40};
  auto a2 = quick::ToUnorderedSet(a);
  auto b2 = quick::ToUnorderedSet(b);
  auto c2 = quick::ToUnorderedSet(c);
  static_assert(std::is_same<decltype(a2), unordered_set<int>>::value,
                "Unexpected type");
  EXPECT_EQ(a2, (unordered_set<int> {11, 22, 33, 44}));
  EXPECT_EQ(b2, (unordered_set<int> {11, 22, 66, 77}));
  EXPECT_EQ(c2, (unordered_set<int> {30, 40, 50}));
}



TEST(InvertMapTest, Basic) {
  unordered_map<int, int> a = {{11, 22}, {33, 44}};
  std::unordered_map<int, int> b;
  qk::InvertMap(a, &b);
  EXPECT_EQ(b, (std::unordered_map<int, int>{{22, 11}, {44, 33}}));
}


TEST(InsertToSet, Basic) {
  unordered_set<int> x = {11, 12};
  unordered_set<int> y = {13, 12};
  unordered_set<int> z = {13, 12};
  quick::InsertToSet(x, &y);
}


TEST(Get, Basic) {
  std::unordered_map<int, int> m = {{11, 2}, {22, 3}, {33, 4}};
  EXPECT_EQ(qk::Get(m, 11, 120), 2);
  EXPECT_EQ(qk::Get(m, 12, 120), 120);
  std::unordered_map<int, int> m2 = {};
  EXPECT_EQ(qk::Get(m, 12, 120), 120);
}

