// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

// If some other defination of std::operator<<(std::ostream&os, const T&) is
// available, (either before `debug.hpp` header or after `debug.hpp` header),
// debug.hpp can still be included without any compile time conflict.
// Additionally, it doesn't override their definations.

#include "quick/debug.hpp"

namespace std {
template<typename T1, typename T2>
ostream& operator<<(ostream& os, const std::pair<T1, T2>& input) {
  os << "CustomPair(" << input.first << ", " << input.second << ")";
  return os;
}
}

#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <set>
#include <string>

#include "gtest/gtest.h"

using std::make_pair;
using std::pair;
using std::cout;
using std::endl;

TEST(OstreamExtensionConflictTest, After) {
  std::ostringstream oss;
  pair<int, int> p(100, 200);
  oss << p;
  EXPECT_EQ(oss.str(), "CustomPair(100, 200)");
}

