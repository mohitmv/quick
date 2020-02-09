// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include "quick/variant.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <quick/debug.hpp>
#include "gtest/gtest.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

TEST(QuickVariant, Basic) {
  qk::variant<int, string, vector<int>> v, v3;
  v.at<0>() = 112;
  EXPECT_EQ(v.at<0>(), 112);
  v.at<1>() = "Saini Mohit";
  EXPECT_EQ(v.at<1>(), "Saini Mohit");
  const auto& v1 = v;
  EXPECT_EQ(v1.at<1>(), "Saini Mohit");
  v.at<2>(10, 212);
  EXPECT_EQ(v1.at<2>().size(), 10U);
  EXPECT_EQ(v1.at<2>()[5], 212);
  EXPECT_EQ(v1.selected_type(), 2U);
  EXPECT_TRUE(v1.initialized());
  EXPECT_EQ(v.at<2>().size(), 10U);
  v3 = std::move(v);
  EXPECT_EQ(v3.at<2>().size(), 10U);
  v3.at<2>() = {330, 550, 2000};
  v = std::move(v3);
  EXPECT_EQ(v.at<2>().at(1), 550);
  auto v4 = v1;
  v.at<1>() = std::to_string(v4.at<2>().at(2)) + "_Abc";
  auto v2 = v;
  EXPECT_EQ(v2.at<1>(), "2000_Abc");
  EXPECT_ANY_THROW((v1.at<0>()));
}


TEST(QuickVariant, CustomType) {
  struct Values {
    vector<int> counts;
    Values(): counts(7, 0) {}
    void Reset() { counts.clear(); counts.resize(7, 0); }
    Values& CopyCtr(int x) {counts[0] += x; return *this;}
    Values& CopyAssign(int x) {counts[1] += x; return *this;}
    Values& MoveCtr(int x) {counts[2] += x; return *this;}
    Values& MoveAssign(int x) {counts[3] += x; return *this;}
    Values& SimpleCtr(int x) {counts[4] += x; return *this;}
    Values& DefaultCtr(int x) {counts[5] += x; return *this;}
    Values& Dtr(int x) {counts[6] += x; return *this;}
    bool operator==(const Values& rhs) const {
      return (counts == rhs.counts);
    }
    int Sum() const {
      int o = 0;
      for (auto i : counts)
        o += i;
      return o;
    }
  };
  static Values values;
  values.Reset();
  struct S {
    int x;
    S(): x(10) {values.DefaultCtr(1);}
    S(int x): x(x) { values.SimpleCtr(1); }
    S(const S&) { values.CopyCtr(1); }
    S(S&&) noexcept { values.MoveCtr(1);}
    S& operator=(const S&) { values.CopyAssign(1); return *this;}
    S& operator=(S&&) noexcept { values.MoveAssign(1); return *this;}
    ~S() { values.Dtr(1);}
  };
  quick::variant<int, vector<S>, S, quick::variant<int, S>> v;
  {
    EXPECT_EQ(values.Sum(), 0);
    v.at<0>() = 11;
    EXPECT_EQ(values.Sum(), 0);
    v.at<1>().resize(5);
    EXPECT_EQ(values, Values().DefaultCtr(5));
    values.Reset();
    auto v3 = v;
    values.Reset();
    auto v2 = v;
    EXPECT_EQ(values, Values().CopyCtr(5));
    values.Reset();
    v2.at<2>(104);
    EXPECT_EQ(values, Values().SimpleCtr(1).Dtr(5));
    EXPECT_EQ(v2.at<2>().x, 104);
    values.Reset();
    v = std::move(v2);
    EXPECT_EQ(values, Values().MoveCtr(1).Dtr(5)) << values.counts;
    values.Reset();
    v2 = std::move(v);
    EXPECT_EQ(values, Values().MoveAssign(1));
    values.Reset();
    v = v2;
    EXPECT_EQ(values, Values().CopyAssign(1));
    values.Reset();
    v.at<3>().at<1>(148);
    EXPECT_EQ(values, Values().SimpleCtr(1).Dtr(1));
    EXPECT_EQ(v.at<3>().at<1>().x, 148);
    values.Reset();
    v3.at<1>()[2] = std::move(v.at<3>().at<1>());
    EXPECT_EQ(values, Values().MoveAssign(1)) << values.counts;
    values.Reset();
    auto v4 = std::move(v3);
    EXPECT_EQ(values, Values()) << values.counts;
    v3.at<2>(457);
    EXPECT_EQ(v3.selected_type(), 2U);
    values.Reset();
    auto v5 = std::move(v3);
    EXPECT_EQ(values, Values().MoveCtr(1)) << values.counts;
    values.Reset();
    v4.clear();
    EXPECT_FALSE(v4.initialized());
    EXPECT_EQ(values, Values().Dtr(5)) << values.counts;
    values.Reset();
    v4.at<1>().emplace_back(113);
    EXPECT_EQ(values, Values().SimpleCtr(1)) << values.counts;
    values.Reset();
    v4.at<1>().reserve(100);
    v4.at<1>().emplace_back(114);
    v4.at<1>().emplace_back(115);
    EXPECT_EQ(values, Values().MoveCtr(1).Dtr(1).SimpleCtr(2)) << values.counts;
  }
}











