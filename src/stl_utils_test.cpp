// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include "quick/stl_utils.hpp"

#include <iostream>
#include <unordered_map>

#include "gtest/gtest.h"

using std::unordered_set;
using std::unordered_map;
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
  EXPECT_EQ(qk::STLGetKeys(std::unordered_map<int, int>{}).size(), 0);
  std::set<int> results;
  qk::STLGetKeys(m, &results);
  EXPECT_EQ(results, (std::set<int> {10, 20}));
}

TEST(InsertToVector, Basic) {
  std::vector<int> a = {5, 66};
  std::vector<int> b = {76, 3, 66};
  qk::InsertToVector(b, &a);
  EXPECT_EQ(a.size(), 5);
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
  EXPECT_EQ(a.size(), 5);
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
  EXPECT_EQ(aa.size(), 5);
  EXPECT_EQ(aa[4].data, 115);
}


TEST(InsertToSet, Basic) {
  std::unordered_set<int> a = {11, 12, 13, 14};
  auto a2 = a;
  std::unordered_set<int> b = {44, 55, 66, 77};
  auto b2 = b;
  auto union_set = qk::SetUnion(a, b);
  EXPECT_EQ(a, a2);
  EXPECT_EQ(b, b2);
  EXPECT_EQ(union_set.size(), 8);
  EXPECT_TRUE(qk::ContainsKey(union_set, 55));
  EXPECT_TRUE(qk::ContainsKey(union_set, 13));
  EXPECT_EQ(qk::SetUnion(std::set<int> {10, 20},
                         std::unordered_set<int> {20, 30}),
            (std::set<int> {10, 20, 30}));
}









