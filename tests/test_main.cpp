// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include "gtest/gtest.h"

void CrashSignalHandler(int sig) {
  std::cout << "Aborting ..." << std::endl;
  exit(sig);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  for (int sig : {SIGSEGV, SIGILL, SIGFPE, SIGABRT, SIGBUS, SIGTERM}) {
    signal(sig, CrashSignalHandler);
  }
  return RUN_ALL_TESTS();
}
