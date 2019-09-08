#include <iostream>

#include "quick/stl_utils.hpp"
#include "gtest/gtest.h"
#include <unordered_map>

using std::unordered_set;
using std::unordered_map;

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


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

