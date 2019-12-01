// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include "quick/experiments/variant.hpp"

#include <iostream>
#include <vector>
#include <string>

#include "gtest/gtest.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

TEST(QuickVariant, Basic) {

  qk::variant<int, string, vector<int>> v, v3;
  v.at<0>() = 112;
  cout << v.at<0>() << endl;
  v.at<1>() = "Saini Mohit";
  cout << v.at<1>() << endl;
  const auto& v1 = v;
  cout << v1.at<1>() << endl;
  // cout << v1.at<0>() << endl;
  v.at<2>(10, 2);
  cout << v1.at<2>().size() << endl;
  cout << v1.selected_type() << endl;
  cout << v1.initialized() << endl;
  // v3 = std::move(v1);
  {
    using namespace quick::variant_impl;
    using TL = TypeList<int, string, int, int, vector<int>>;
    using A = GetNthType<0, int, string, int>;
    using B = GetNthType<1, int, string, int>;

    A x = 4;
    (void)x;
    B y = "Saini Mohit";
    cout << y << endl;

    // TL tl_e;
    // // GetNthTypeFromTypeList<0>(tl_e) = 44;
    // using T0 = TL::At<0>;
    // T0 x = 44;
    // cout << x << endl;

  }
}


