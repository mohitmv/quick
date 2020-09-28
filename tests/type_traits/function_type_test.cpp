// Copyright: 2020 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include "gtest/gtest.h"
#include "quick/type_traits/function_type.hpp"

int add1(int x, int y) {
    return x+y;
}

struct S {
  int add2(int x, int y) {
      return x+y;
  }
  static int add3(int x, int y) {
      return x+y;
  }
};

TEST(FunctionType, Basic) {
  auto add4 = [](int x, int y) {
      return x+y;
  };
  int xx = 1000;
  auto add5 = [xx](int x, int y) {
      return x+y+xx;
  };
  std::function<int(int, int)> add6 = [](int x, int y) {
      return x+y;
  };
  struct R {
    int operator()(int x, int y) {
      return x+y;
    }
  };
  R add7;
  {
    using F1 = quick::function_type<decltype(&add1)>;
    F1 f1 = &add1;
    EXPECT_EQ(7, f1(3, 4));
    static_assert(std::is_same<F1, std::function<int(int, int)>>::value, "");
  }
  {
    using F2 = quick::function_type<decltype(&S::add2)>;
    F2 f2 = &add1;
    EXPECT_EQ(7, f2(3, 4));
    static_assert(std::is_same<F2, std::function<int(int, int)>>::value, "");
  }
  {
    using F3 = quick::function_type<decltype(&S::add3)>;
    F3 f3 = &S::add3;
    EXPECT_EQ(7, f3(3, 4));
    static_assert(std::is_same<F3, std::function<int(int, int)>>::value, "");
  }
  {
    using F4 = quick::function_type<decltype(add4)>;
    F4 f4 = add4;
    EXPECT_EQ(7, f4(3, 4));
    static_assert(std::is_same<F4, std::function<int(int, int)>>::value, "");
  }
  {
    using F5 = quick::function_type<decltype(add5)>;
    F5 f5 = add5;
    EXPECT_EQ(1007, f5(3, 4));
    static_assert(std::is_same<F5, std::function<int(int, int)>>::value, "");
  }
  {
    using F6 = quick::function_type<decltype(add6)>;
    F6 f6 = add6;
    EXPECT_EQ(7, f6(3, 4));
    static_assert(std::is_same<F6, std::function<int(int, int)>>::value, "");
  }
  {
    using F7 = quick::function_type<decltype(add7)>;
    F7 f7 = add7;
    EXPECT_EQ(7, f7(3, 4));
    static_assert(std::is_same<F7, std::function<int(int, int)>>::value, "");
  }
}


