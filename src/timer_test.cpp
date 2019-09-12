// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include "quick/timer.hpp"

#include "gtest/gtest.h"

TEST(GetEpochMicroSeconds, Basic) {
  EXPECT_GT(qk::GetEpochMicroSeconds(), 10000000000000000LL);
}


TEST(MicroSecondTimer, Basic) {
  qk::MicroSecondTimer timer;
  volatile int k = 0;
  for (int i = 0; i < 100; i++) {
    k += i;
  }
  long long t1 = timer.GetStartTime();
  EXPECT_GT(timer.GetElapsedTime(), 0);
  timer.Restart();
  long long t2 = timer.GetStartTime();
  EXPECT_GT();
}

