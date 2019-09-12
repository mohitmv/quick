// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include <chrono>  // NOLINT
#include <thread>  // NOLINT

#include "gtest/gtest.h"
#include "quick/time.hpp"


TEST(GetEpochMicroSeconds, Basic) {
  EXPECT_GT(qk::GetEpochMicroSeconds(), 1568297419833LL);
}

TEST(MicroSecondTimer, Basic) {
  qk::MicroSecondTimer timer;
  int64_t t1 = timer.GetStartTime();
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_GT(timer.GetElapsedTime(), 0);
  EXPECT_EQ(t1, timer.GetStartTime());
  timer.Restart();
  int64_t t2 = timer.GetStartTime();
  EXPECT_GT(t2, t1);
}

