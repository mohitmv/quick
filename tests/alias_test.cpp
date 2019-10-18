// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include "quick/alias.hpp"

#include <string>
#include <vector>
#include "gtest/gtest.h"

using std::string;
using std::vector;

struct S {
  static int num_copy;
  static int num_move;
  int value = 10;
  S() {}
  S(const S&) {num_copy++;}
  S& operator=(const S&) {num_copy++; return *this;}
  S(S&&) noexcept {num_move++;}
  S& operator=(S&&) noexcept {num_move++; return *this;}
  static void ResetCounter() {num_move = num_copy = 0;}
};

int S::num_copy = 0;
int S::num_move = 0;

int F1(int x) {
  return x*100;
}

int F2(vector<int> x) {
  return x.size()*100;
}

int F3(S x) {
  return x.value;
}

QUICK_FUNCTION_ALIAS(G1, F1)
QUICK_FUNCTION_ALIAS(G2, F2)
QUICK_FUNCTION_ALIAS(G3, F3)

TEST(AliasTest, Basic) {
  EXPECT_EQ(G1(10), 1000);
  EXPECT_EQ(G2((vector<int>{10, 20, 30})), 300);
  {
    S s1;
    int o1 = F3(s1);
    EXPECT_EQ(S::num_move, 0);
    EXPECT_EQ(S::num_copy, 1);
    S::ResetCounter();
    int o2 = G3(s1);
    EXPECT_EQ(S::num_move, 0);
    EXPECT_EQ(S::num_copy, 1);
    S::ResetCounter();
    EXPECT_EQ(o1 + o2, 20);
  }
  {
    int o1 = F3(S());
    EXPECT_EQ(S::num_move, 0);
    EXPECT_EQ(S::num_copy, 0);
    S::ResetCounter();
    int o2 = G3(S());
    EXPECT_EQ(S::num_move, 1);
    EXPECT_EQ(S::num_copy, 0);
    S::ResetCounter();
    EXPECT_EQ(o1 + o2, 20);
  }
}

