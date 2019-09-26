// Copyright: ThoughtSpot Inc. 2019
// Author: Mohit Saini (mohit.saini@thoughtspot.com)

#include "quick/hash.hpp"

#include <map>
#include <utility>
#include <vector>
#include <set>
#include <string>
#include <tuple>

#include "gtest/gtest.h"


using std::make_pair;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;
using std::tuple;
using std::make_tuple;

using std::size_t;

// testing qk::hash for std::pair, enum.
TEST(HashTest, TestEnumAndPair) {
  enum Gender {MALE, FEMALE, OTHER_GENDER};
  qk::hash<pair<int, Gender>> hash_f1;
  std::size_t h1 = hash_f1(make_pair(11, FEMALE));
  std::size_t h2 = hash_f1(make_pair(11, MALE));
  std::size_t h3 = hash_f1(make_pair(12, OTHER_GENDER));
  std::size_t h4 = hash_f1(make_pair(11, FEMALE));
  EXPECT_EQ(h1, h4);
  EXPECT_NE(h1, h2);
  EXPECT_NE(h1, h3);
  EXPECT_NE(h2, h3);
}

// testing qk::hash for std::tuple, std::map.
TEST(HashTest, TestTupleAndMap) {
  using MyMap = std::map<pair<int, string>, string>;
  qk::hash<tuple<int, MyMap>> hash_f2;
  auto h1 = hash_f2(make_tuple(11, MyMap {{{10, "100"}, "10000"},
                                          {{40, "400-"}, "40000--"},
                                          {{30, "-300-"}, "---340000--"}}));
  auto h2 = hash_f2(make_tuple(11, MyMap {{{10, "100"}, "10000"},
                                          {{30, "-300-"}, "---340000--"},
                                          {{40, "400-"}, "40000--"}}));
  auto h3 = hash_f2(make_tuple(11, MyMap {{{10, "100"}, "10000"},
                                          {{30, "-300-"}, "--340000--"},
                                          {{40, "400-"}, "40000--"}}));
  EXPECT_EQ(h1, h2);
  EXPECT_NE(h1, h3);
}

// testing qk::hash for std::list.
TEST(HashTest, TestLinkedList) {
  using MyList = std::list<pair<int, string>>;
  qk::hash<MyList> hash_f2;
  auto h1 = hash_f2({{100, "Mohit"}, {200, "Saini"}, {300, "Mohit Saini"}});
  auto h2 = hash_f2({{100, "Mohit"}, {200, "Mohit"}, {300, "Mohit Saini"}});
  auto h3 = hash_f2({{100, "Mohit"}, {200, "Saini"}, {300, "Mohit Saini"}});
  EXPECT_EQ(h1, h3);
  EXPECT_NE(h1, h2);
}

// testing qk::hash for custom defined types exposing GetHash method.
TEST(HashTest, TestCustomTypes) {
  // Sample custom defined class.
  struct Company {
    struct Release {
      Release() {}
      explicit Release(int id): id(id) {}
      Release(int id, int release_date): id(id),
                                         release_date(release_date) {}
      int id = 1;
      int release_date = 2;
      set<int> bugs;
      std::vector<string> release_tags;
      size_t GetHash() const {
        return qk::HashF(make_tuple(id, release_date, bugs, release_tags));
      }
    };
    int num_members = 0;
    std::vector<Release> releases;
    std::set<pair<int, int>> task_pairs;
    size_t GetHash() const {
      return qk::HashF(make_tuple(num_members, releases, task_pairs));
    }
  };
  qk::hash<Company> hash_f3;
  qk::hash<Company::Release> hash_f4;
  Company::Release r1(11), r2(12), r3(13, 2), r4, r5(11, 2);
  r2.bugs = {11, 11, 134};
  r3.bugs = {134, 134, 11};
  EXPECT_EQ(hash_f4(r1), hash_f4(r5));
  EXPECT_NE(hash_f4(r1), hash_f4(r2));
  EXPECT_NE(hash_f4(r1), hash_f4(r3));
  EXPECT_NE(hash_f4(r1), hash_f4(r4));
  // r2 and r3 differs only at Relase::id.
  EXPECT_NE(hash_f4(r2), hash_f4(r3));
  r2.id = 13;
  EXPECT_EQ(hash_f4(r2), hash_f4(r3));
  // now r2 and r3 differs at release_tags as well.
  r2.release_tags.push_back("AAq");
  EXPECT_NE(hash_f4(r2), hash_f4(r3));

  Company c1, c2;
  EXPECT_EQ(hash_f3(c1), hash_f3(c2));
  c1.releases.push_back(r2);
  c2.releases.push_back(r3);
  EXPECT_NE(hash_f3(c1), hash_f3(c2));
  c2.releases.back().release_tags.push_back("AAq");
  EXPECT_EQ(hash_f3(c1), hash_f3(c2));
  c1.task_pairs = {{110, 110}, {1000, 1000}};
  EXPECT_NE(hash_f3(c1), hash_f3(c2));
  c2.task_pairs = {{1000, 1000}, {1000, 1000}, {1000, 1000}, {110, 110}};
  EXPECT_EQ(hash_f3(c1), hash_f3(c2));
}
