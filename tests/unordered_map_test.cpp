// Copyright: ThoughtSpot Inc. 2019
// Author: Mohit Saini (mohit.saini@thoughtspot.com)

#include "quick/unordered_map.hpp"

#include <map>
#include <utility>
#include <vector>
#include <set>
#include <string>
#include <tuple>

#include "gtest/gtest.h"

using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;
using std::tuple;
using std::make_tuple;


using std::size_t;
using std::make_pair;

// testing ts::unordered_map for complex keys ex: tuple, std::vector,
// std::map, ... etc.
TEST(UnorderedMapTest, Basic) {
  enum Gender {MALE, FEMALE, OTHER_GENDER};
  qk::unordered_map<pair<int, Gender>, int> m;
  m[make_pair(10, MALE)] = 110;
  m[make_pair(13, FEMALE)] = 120;
  EXPECT_EQ(m.at(make_pair(10, MALE)), 110);
  EXPECT_EQ(m.at(make_pair(13, FEMALE)), 120);
}
