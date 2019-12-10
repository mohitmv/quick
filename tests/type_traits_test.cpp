// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include <quick/type_traits.hpp>

#include <vector>
#include <utility>
#include <unordered_set>

#include "gtest/gtest.h"


struct TypeTraitsTest: ::testing::Test {
  struct P1 {};
  struct P2 {
      void Member();
  };


  template<typename T, typename R>
  struct S: std::true_type {
    static constexpr int sx = 10;
  };

  struct Sf {
    static constexpr int sx = 20;
  };

  template<typename T>
  using S2 = std::is_same<void, decltype(&T::Member)>;

  template<typename T>
  using S3 = std::is_same<void, decltype(std::declval<T>().Member())>;
};


TEST_F(TypeTraitsTest, Basic) {
  using std::vector;
  using std::unordered_set;
  EXPECT_EQ((quick::test_specialization<S, int, int>::value), 1);
  EXPECT_EQ((quick::test_specialization<S2, P1>::value), 0);
  EXPECT_EQ((quick::test_specialization<S2, P2>::value), 1);

  int e1 = quick::specialize_if_can<Sf, S, int, int>::sx;
  EXPECT_EQ(e1, 10);
  int e2 = quick::specialize_if_can<Sf, S, int, int, int>::sx;
  EXPECT_EQ(e2, 20);
  EXPECT_EQ((quick::specialize_if_can<std::false_type, S2, P1>::value), false);
  EXPECT_EQ((quick::specialize_if_can<std::false_type, S2, P2>::value), false);
  EXPECT_EQ((quick::specialize_if_can<std::false_type, S3, P1>::value), false);
  EXPECT_EQ((quick::specialize_if_can<std::false_type, S3, P2>::value), true);

  EXPECT_EQ((quick::is_specialization<vector<int>, vector>::value), true);
  // Some stupid bug in google test. Figure that out and uncomment these lines.
  {
    bool v = quick::is_specialization<vector<int>,
                                      vector>::value;

    EXPECT_EQ(v, true);
  }
  {
    bool v = quick::is_specialization<vector<int>,
                                      std::set>::value;

    EXPECT_EQ(v, false);
  }
  {
    bool v = (quick::is_specialization<unordered_set<int>,
                                        unordered_set>::value);
    EXPECT_EQ(v, true);
  }
}

