// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef QUICK_FILE_UTILS_HPP_
#define QUICK_FILE_UTILS_HPP_

#include <utility>
#include <string>
#include <sstream>
#include <ostream>  // NOLINT
#include <fstream>  // NOLINT

namespace quick {

// Possible use: `throw FileException(FileException::FAILED_TO_OPEN, file_name)`
//                in case of exception during file reading.
struct FileException : public std::exception {
  enum ErrorType {UNKNOWN, FAILED_TO_OPEN, FAILED_TO_WRITE};
  FileException();
  explicit FileException(ErrorType type);
  FileException(ErrorType type, const std::string& file_name);
  virtual const char* what() const throw() {
    return error_message.c_str();
  }
  ErrorType type = UNKNOWN;
  std::string file_name;
  std::string error_message;

 private:
  void BuildErrorMessage();
};

std::string ReadFile(const std::string& file_name);

void WriteFile(const std::string& file_name, const std::string& content);

bool DoesFileExist(const std::string& file_name);

}  // namespace quick

namespace qk = quick;


#endif  // QUICK_FILE_UTILS_HPP_
