// Copyright: ThoughtSpot Inc. 2019
// Author: Mohit Saini (mohit.saini@thoughtspot.com)

#ifndef TS_FILE_UTILS_HPP_
#define TS_FILE_UTILS_HPP_

#include <utility>
#include <string>
#include <sstream>
#include <ostream>  // NOLINT
#include <fstream>  // NOLINT

namespace ts {

// Possible use: `throw FileException(FileException::FAILED_TO_OPEN, file_name)`
//                in case of exception during file reading.
// ToDo(Mohit): Create a TSException base class, exposing rich interface for
// custom exceptions to be built on top of it without requiring boilerplate
// code.
// ToDo(Mohit)-2: Move the implementations of exception helpers to *.cpp file.
// ToDo(Mohit)-3: Add support for __FILE__, __LINE__ macros in exception
// message.
struct FileException : public std::exception {
  enum ErrorType {UNKNOWN, FAILED_TO_OPEN, FAILED_TO_WRITE};
  FileException() {
    this->BuildErrorMessage();
  };
  explicit FileException(ErrorType type): type(type) {
    this->BuildErrorMessage();
  }
  FileException(ErrorType type, std::string file_name): type(type),
                                                        file_name(file_name) {
    this->BuildErrorMessage();
  }
  ErrorType type = UNKNOWN;
  std::string file_name;
  std::string error_message;
  virtual const char* what() const throw() {
    return error_message.c_str();
  }

 private:
  void BuildErrorMessage() {
    std::ostringstream oss;
    switch (type) {
      case UNKNOWN:
        oss << "Unknown Error: " << file_name;
        break;
      case FAILED_TO_OPEN:
        oss << "FAILED_TO_OPEN: " << file_name;
        break;
      case FAILED_TO_WRITE:
        oss << "FAILED_TO_WRITE: " << file_name;
        break;
      default: break;
    }
    oss << "\n";
    this-> error_message = oss.str();
  }
};

std::string ReadFile(const std::string& file_name);

void WriteFile(const std::string& file_name, const std::string& content);

}  // namespace ts

#endif  // TS_FILE_UTILS_HPP_
