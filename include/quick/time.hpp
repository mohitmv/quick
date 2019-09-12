// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef QUICK_TIME_HPP_
#define QUICK_TIME_HPP_

#include <chrono>

namespace quick {

long long GetEpochMicroSeconds() {
  using std::chrono;
  auto epoch_time = system_clock::now().time_since_epoch();
  return duration_cast<milliseconds>(epoch_time).count();
}




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

}  // namespace quick

namespace qk = quick;


#endif  // QUICK_TIME_HPP_
