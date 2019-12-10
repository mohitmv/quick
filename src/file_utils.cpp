// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#include "quick/file_utils.hpp"


namespace quick {

using std::string;

FileException::FileException() {
  this->BuildErrorMessage();
}

FileException::FileException(FileException::ErrorType type): type(type) {
  this->BuildErrorMessage();
}

FileException::FileException(FileException::ErrorType type,
                             const string& file_name): type(type),
                                                       file_name(file_name) {
  this->BuildErrorMessage();
}

void FileException::BuildErrorMessage() {
  std::ostringstream oss;
  switch (type) {
    case UNKNOWN:
      oss << "FileException: \"" << file_name << "\"";
      break;
    case FAILED_TO_OPEN:
      oss << "FAILED_TO_OPEN: \"" << file_name << "\"";
      break;
    case FAILED_TO_WRITE:
      oss << "FAILED_TO_WRITE: \"" << file_name << "\"";
      break;
    default: break;
  }
  oss << "\n";
  this-> error_message = oss.str();
}

std::string ReadFile(const std::string& file_name) {
  std::ifstream fd(file_name);
  if (!fd) {
    throw FileException(FileException::FAILED_TO_OPEN, file_name);
  }
  std::stringstream buffer;
  buffer << fd.rdbuf();
  fd.close();
  return buffer.str();
}

void WriteFile(const std::string& file_name, const std::string& content) {
  std::ofstream fd(file_name);
  if (!fd) {
    throw FileException(FileException::FAILED_TO_WRITE, file_name);
  }
  fd << content;
  fd.close();
}

bool DoesFileExist(const std::string& file_name) {
    std::ifstream infile(file_name);
    return infile.good();
}


}  // namespace quick



