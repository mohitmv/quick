// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include "quick/unordered_set.hpp"

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


// testing ts::unordered_map for complex keys ex: tuple, std::vector,
// std::map, ... etc.
TEST(UnorderedSetTest, Basic) {
  enum Gender {MALE, FEMALE, OTHER_GENDER};
  quick::unordered_set<pair<int, Gender>> m;
  m.insert(make_pair(10, MALE));
  m.insert(make_pair(13, FEMALE));
  EXPECT_EQ(m.size(), 2);
}

