// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include "quick/debug_stream.hpp"

#include <map>
#include <utility>
#include <vector>
#include <set>
#include <string>
#include <iostream>

#include <quick/unordered_map.hpp>

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


// To generate C++ string to enforce a EXPECT_EQ, follow these step
// 1. Do `cout << ds.str() << endl` and print the output. Copy paste the output
//    in python string using `"""`.
// 2. Run this print command and paste in this test file.
//
// a = """<output>"""
// print("\n".join('"'+i+"\\n"+'"' for i in a.split("\n")))

TEST(DebugStreamTest, Fundamental) {
  using quick::DebugStream;
  DebugStream ds;
  uint32_t a = 1;
  int8_t b = 43;
  int x = 4;
  int16_t y = -45;
  uint8_t q = 12;
  ds << a << ", " << b << ", " << x << ", " << y << ", " << q;
  EXPECT_EQ(ds.str(), "1, 43, 4, -45, 12");
}


TEST(DebugStreamTest, Basic) {
  using quick::DebugStream;
  string expected_output;
  {
    quick::DebugStream ds;
    enum {UU, PP};
    ds.BranchStart('[');
    ds << make_pair(44, 55) << "\n\n";
    {
      ds << "mohit" << make_pair(44, 55);
      ds << "saini\n\n";
      {
        ds << "mohit\nsaini\nand mohit\nsaini\n";
        ds << UU << ", " << PP;
      }
      ds << "\n\nsaini\n\n";
    }
    ds.BranchEnd(']');
    ds << "\n\n" << make_pair(44, 55);
    expected_output =
        "[\n"
        "  (\n"
        "    44, 55\n"
        "  )\n"
        "  \n"
        "  mohit(\n"
        "    44, 55\n"
        "  )saini\n"
        "  \n"
        "  mohit\n"
        "  saini\n"
        "  and mohit\n"
        "  saini\n"
        "  ENUM-0, ENUM-1\n"
        "  \n"
        "  saini\n"
        "  \n"
        "  \n"
        "]\n\n"
        "(\n"
        "  44, 55\n"
        ")";
    EXPECT_EQ(ds.str(), expected_output);
  }
  {
    quick::DebugStream ds;
    ds << "mohit\n\n";
    ds << 'e';
    ds << "\n\n";
    ds << 11198996464646446;
    ds.BranchStart('{');
    ds << "Mohit\n\nSaini\n\n";
    string s = "Mohit\n\nSaini";
    ds << s;
    ds.BranchEnd('}');
    ds << s;
    ds << "\n\n";
    ds << make_pair(make_pair(44, 55), make_pair(7744, 5599));
    expected_output =
          "mohit\n\n"
          "e\n\n"
          "11198996464646446{\n"
          "  Mohit\n"
          "  \n"
          "  Saini\n"
          "  \n"
          "  Mohit\n"
          "  \n"
          "  Saini\n"
          "}Mohit\n\n"
          "Saini\n"
          "\n"
          "(\n"
          "  (\n"
          "    44, 55\n"
          "  ), (\n"
          "    7744, 5599\n"
          "  )\n"
          ")";
    EXPECT_EQ(ds.str(), expected_output);
  }
  {
    quick::DebugStream ds;
    vector<vector<int>> v1 = {{11, 22}, {44, 55}};
    map<string, set<int>> map1 = {{"age", {1300}}, {"sage", {33, 55, 66}}};
    pair<int, pair<int, vector<string>>> p3;
    p3 = make_pair(121, make_pair(133, vector<string> {"A1", "B1", "C1"}));
    unordered_map<string, unordered_set<int>> map4;
    map4 = {{"age", {1000}}};
    ds << v1 << "\n" << map1 << "\n" << p3 << "\n" << map4 << "\n";
    expected_output =
          "[\n"
          "  [\n"
          "    11, 22\n"
          "  ], [\n"
          "    44, 55\n"
          "  ]\n"
          "]\n"
          "{\n"
          "  age: [\n"
          "    1300\n"
          "  ],\n"
          "  sage: [\n"
          "    33, 55, 66\n"
          "  ]\n"
          "}\n"
          "(\n"
          "  121, (\n"
          "    133, [\n"
          "      A1, B1, C1\n"
          "    ]\n"
          "  )\n"
          ")\n"
          "{\n"
          "  age: [\n"
          "    1000\n"
          "  ]\n"
          "}\n";
    EXPECT_EQ(ds.str(), expected_output);
  }

  {
    struct S {
      string s;
      vector<S> children;
      S(): s("Default") {}
      S(const string& s): s(s) {}  // NOLINT
      S(const string& s, vector<S>&& children): s(s),
                                                children(std::move(children)) {}
      void DebugStream(qk::DebugStream& ds) const {  // NOLINT
        ds << "name = " << s << "\nchildren = " << children;
      }
    };
    {
      quick::DebugStream ds;
      S s1 = {"Root"};
      ds << s1;
      expected_output =
          "{\n"
          "  name = Root\n"
          "  children = []\n"
          "}";
      EXPECT_EQ(expected_output, ds.str());
    }
    {
      quick::DebugStream ds;
      S s2 = {"Root", {S {"R.1"}, S {"R.2"}, S {"R.3"}, S {"R.4"}}};
      ds << s2;
      expected_output =
            "{\n"
            "  name = Root\n"
            "  children = [\n"
            "    {\n"
            "      name = R.1\n"
            "      children = []\n"
            "    }, {\n"
            "      name = R.2\n"
            "      children = []\n"
            "    }, {\n"
            "      name = R.3\n"
            "      children = []\n"
            "    }, {\n"
            "      name = R.4\n"
            "      children = []\n"
            "    }\n"
            "  ]\n"
            "}";
      EXPECT_EQ(expected_output, ds.str());
    }
    {
      quick::DebugStream ds;
      S s3 = {"Root", {S {"R.1"},
                       S {"R.2",
                          {S {"R.2.1",
                              {S {"R.2.1.1"}}
                             },
                           S {"R.2.2"},
                           S {"R.2.3",
                              {S {"R.2.3.1"}}}}}}};
      ds << s3;
      expected_output =
          "{\n"
          "  name = Root\n"
          "  children = [\n"
          "    {\n"
          "      name = R.1\n"
          "      children = []\n"
          "    }, {\n"
          "      name = R.2\n"
          "      children = [\n"
          "        {\n"
          "          name = R.2.1\n"
          "          children = [\n"
          "            {\n"
          "              name = R.2.1.1\n"
          "              children = []\n"
          "            }\n"
          "          ]\n"
          "        }, {\n"
          "          name = R.2.2\n"
          "          children = []\n"
          "        }, {\n"
          "          name = R.2.3\n"
          "          children = [\n"
          "            {\n"
          "              name = R.2.3.1\n"
          "              children = []\n"
          "            }\n"
          "          ]\n"
          "        }\n"
          "      ]\n"
          "    }\n"
          "  ]\n"
          "}";
      EXPECT_EQ(expected_output, ds.str());
    }
  }
  {
    quick::DebugStream ds;
    std::list<pair<int, int>> p2 = {{11, 22}, {44, 55}, {44, 55}};
    std::list<pair<int, int>> p4 = {};
    quick::unordered_map<pair<int, string>, int> m1, m2;
    m1 = {{make_pair(500, "name"), 11}};
    ds << p2 << "\n" << p4 << "\n" << m1 << "\n" << m2;
    expected_output =
      "[\n"
      "  (\n"
      "    11, 22\n"
      "  ), (\n"
      "    44, 55\n"
      "  ), (\n"
      "    44, 55\n"
      "  )\n"
      "]\n"
      "[]\n"
      "{\n"
      "  (500, name): 11\n"
      "}\n"
      "{}";
    EXPECT_EQ(expected_output, ds.str());
  }
}


TEST(DebugStreamTest, Branch) {
  quick::DebugStream ds;
  ds << "mohit" << "{" << 33 << "\n\n" << "saini" << "}" << "\n";
  string expected_output =
      "mohit{\n"
      "  33\n"
      "  \n"
      "  saini\n"
      "}\n";
  EXPECT_EQ(ds.str(), expected_output);
}



TEST(DebugStreamTest, ConstructorAndConsume) {
  int x = 100;
  vector<int> v = {11, 22, 33};
  EXPECT_EQ(quick::DebugStream(x).SetInline(true).Consume(v).str(),
            "100[11, 22, 33]");
}


