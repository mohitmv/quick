// Copyright: ThoughtSpot Inc. 2019
// Author: Mohit Saini (mohit.saini@thoughtspot.com)

#include <map>
#include <utility>
#include <vector>
#include <set>
#include <string>
#include <tuple>

#include "gflags/gflags.h"
#include "glog/logging.h"

#include "gtest/gtest.h"
#include "ts/unordered_map.hpp"

using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;
using std::tuple;
using std::make_tuple;

namespace ts {
namespace test {

using std::size_t;
using std::make_pair;

// testing ts::unordered_map for complex keys ex: tuple, std::vector, std::map, ... etc.
TEST(UnorderedMapTest, Basic) {
  enum Gender {MALE, FEMALE, OTHER_GENDER};
  ts::unordered_map<pair<int, Gender>, int> m;
  m[make_pair(10, MALE)] = 110;
  m[make_pair(13, FEMALE)] = 120;
  EXPECT_EQ(m.at(make_pair(10, MALE)), 110);
  EXPECT_EQ(m.at(make_pair(13, FEMALE)), 120);
}

}  // test
}  // namespace ts
