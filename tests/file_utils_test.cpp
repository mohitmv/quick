// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include "quick/file_utils.hpp"

#include <string>

#include "gtest/gtest.h"

using std::string;

TEST(ReadWriteFile, Basic) {
  string str = "This is the content of this file";
  qk::WriteFile("/tmp/abx887.txt", str);
  EXPECT_EQ(qk::ReadFile("/tmp/abx887.txt"), str);
}

TEST(FileException, Basic) {
  bool exception_cought = false;
  string random_file_name = "/aa/bb/cc/dd/rr/tt/tt/ww/www/rrr/ww/33/rr";
  try {
    string s = qk::ReadFile(random_file_name);
  } catch (const qk::FileException& e) {
    exception_cought = true;
    EXPECT_EQ(e.file_name, random_file_name);
    EXPECT_TRUE(string(e.what()).find(random_file_name) != string::npos);
  }
  EXPECT_TRUE(exception_cought);
}

