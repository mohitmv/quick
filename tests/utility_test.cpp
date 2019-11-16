// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include <quick/utility.hpp>

#include <vector>
#include <utility>
#include <unordered_set>

#include "gtest/gtest.h"



TEST(StdApply, Basic) {
  auto lF = [](int x, int y, char c) {
    return x + y + static_cast<int>(c);
  };
  EXPECT_EQ(quick::apply(lF, std::make_tuple(10, 20, 'a')), 127);
}

