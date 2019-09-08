// Copyright: ThoughtSpot Inc. 2019
// Author: Mohit Saini (mohit.saini@thoughtspot.com)

#include "ts/file_utils.hpp"

#include <string>

using std::string;

namespace ts {

std::string ReadFile(const std::string& file_name) {
  std::ifstream fd(file_name);
  if (!fd) {
    throw FileException(FileException::FAILED_TO_OPEN, file_name);
  }
  std::stringstream buffer;
  buffer << fd.rdbuf();
  fd.close();
  return buffer.str();
};

void WriteFile(const std::string& file_name, const std::string& content) {
  std::ofstream fd(file_name);
  if (!fd) {
    throw FileException(FileException::FAILED_TO_WRITE, file_name);
  }
  fd << content;
  fd.close();
};



}  // namespace ts
