// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include <map>
#include <utility>
#include <vector>
#include <set>
#include <string>

#include "gtest/gtest.h"
#include "quick/debug.hpp"

using std::make_pair;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::unordered_set;
using std::unordered_map;
using std::vector;

TEST(OstreamExtensionTest, Basic) {
  std::ostringstream oss;
  vector<vector<int>> v1 = {{11, 22}, {44, 55}};
  map<string, set<int>> map1 = {{"age", {1300}}};
  pair<int, pair<int, vector<string>>> p3;
  p3 = make_pair(121, make_pair(133, vector<string> {"A1", "B1", "C1"}));
  unordered_map<string, unordered_set<int>> map4;
  map4 = {{"age", {1000}}};
  oss << v1 << "\n" << map1 << "\n" << p3 << "\n" << map4 << "\n";
  string expected_output = "[[11, 22], [44, 55]]\n"
                           "{age: [1300]}\n"
                           "(121, (133, [A1, B1, C1]))\n"
                           "{age: [1000]}\n";
  ASSERT_EQ(oss.str(), expected_output);
}

