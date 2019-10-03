// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include "quick/byte_stream.hpp"

#include <map>
#include <utility>
#include <vector>
#include <set>
#include <string>
#include <tuple>

#include "quick/debug.hpp"

#include "gtest/gtest.h"

using std::make_pair;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;
using std::tuple;
using quick::ByteStream;
using quick::OByteStream;
using quick::IByteStream;
using std::cout;
using std::endl;
using std::make_tuple;


TEST(ByteStream, Basic) {
  int x = 11, y = 555333, z = 3, xx, yy, zz;
  double d = 44.55, dd;
  string s = "byte_stream", ss;
  enum A {AA, BB};
  A q = AA, qq;
  vector<vector<string>> v = {{"1.1", "1.2"}, {"2.1", "2.2"}}, vv;
  vector<set<string>> s1 = {{"1.111", "1.222"}, {"2.111", "2.222"}}, ss1;
  vector<pair<int, tuple<int, float, bool>>> vp1 =
                               {{444, std::make_tuple(33, 44.8f, true)}}, vp2;

  OByteStream obs;
  obs << v  << x  << s  << d  << z <<  y <<  s1  << q <<  vp1;
  IByteStream ibs;
  ibs.str(obs.str());
  ibs >> vv >> xx >> ss >> dd >> zz >> yy >> ss1 >> qq >> vp2;

  EXPECT_EQ(v, vv);
  EXPECT_EQ(x, xx);
  EXPECT_EQ(s, ss);
  EXPECT_EQ(d, dd);
  EXPECT_EQ(z, zz);
  EXPECT_EQ(s1, ss1);
  EXPECT_EQ(q, qq);
  EXPECT_EQ(vp1, vp2);
}


TEST(ByteStream, ClassMethod) {
  struct S {
    int x;
    string s;
    tuple<int, string, vector<int>> p;
    void Serialize(quick::OByteStream& bs) const {
      bs << x << s << p;
    }
    void Deserialize(quick::IByteStream& bs) {
      bs >> x >> s >> p;
    }
    bool operator==(const S& o) const {
      return (x == o.x && s == o.s && p == o.p);
    }
  };
  OByteStream obs;
  S s1, s2 = {100, "Mohit Saini", make_tuple(1000, "Mohit", vector<int> {10, 20, 30})};
  obs << s2 << 200 << "Abc";
  IByteStream ibs;
  ibs.str(obs.str());
  int num;
  string tmp_str;
  ibs >> s1 >> num >> tmp_str;
  EXPECT_EQ(s1, s2);
  EXPECT_EQ(num, 200);
  EXPECT_EQ(tmp_str, "Abc");
}

