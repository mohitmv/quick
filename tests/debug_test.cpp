// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include "quick/debug.hpp"

#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <set>
#include <string>

#include "gtest/gtest.h"

using std::make_pair;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::unordered_set;
using std::unordered_map;
using std::vector;
using std::cout;
using std::endl;

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
  EXPECT_EQ(oss.str(), expected_output);
  struct S {
    string s;
    S(): s("My Value") {}
    string DebugString() const {return s; }
  };
  pair<int, vector<S>> p5;
  p5.first = 100;
  p5.second.resize(3);
  oss.str("");
  oss << p5;
  EXPECT_EQ(oss.str(), "(100, [My Value, My Value, My Value])");
  auto p = std::make_tuple(11, 22, false, vector<int>{10, 20, 30});
  EXPECT_EQ(qk::ToString(p), "(11, 22, 0, [10, 20, 30])");
  std::list<pair<int, int>> p2 = {{11, 22}, {44, 55}, {44, 55}};
  std::list<pair<int, int>> p4 = {};
  EXPECT_EQ(qk::ToString(p2), "[(11, 22), (44, 55), (44, 55)]");
  EXPECT_EQ(qk::ToString(p4), "[]");
}


TEST(OstreamExtensionTest, DebugStream) {
  struct S {
    string s = "Default Value";
    vector<S> children;
    S() {}
    S(const string& s): s(s) {}
    void DebugStream(quick::DebugStream& ds) const {
      ds << "s = " << s << "\n"
         << "children = " << children;
    }
  };
  pair<string, vector<S>> p5;
  p5.first = "A";
  p5.second.resize(3);
  p5.second.emplace_back(S("11"));
  p5.second.emplace_back(S("12"));
  p5.second.back().children.emplace_back(S("11.11"));
  p5.second.back().children.emplace_back(S("11.12"));
  std::ostringstream oss;
  oss << p5;
  // Use this python code to generate this C++ string to expect.
  // a = """<output>"""
  // print("\n".join('"'+i+"\\n"+'"' for i in a.split("\n")))
  string expected_output =
    "(A, [{\n"
    "  s = Default Value\n"
    "  children = []\n"
    "}, {\n"
    "  s = Default Value\n"
    "  children = []\n"
    "}, {\n"
    "  s = Default Value\n"
    "  children = []\n"
    "}, {\n"
    "  s = 11\n"
    "  children = []\n"
    "}, {\n"
    "  s = 12\n"
    "  children = [\n"
    "    {\n"
    "      s = 11.11\n"
    "      children = []\n"
    "    }, {\n"
    "      s = 11.12\n"
    "      children = []\n"
    "    }\n"
    "  ]\n"
    "}])";
  EXPECT_EQ(expected_output, oss.str());
}

TEST(ToString, Basic) {
  pair<int, pair<int, int>> p(110, {10, 44});
  EXPECT_EQ(qk::ToString(p), "(110, (10, 44))");
}



