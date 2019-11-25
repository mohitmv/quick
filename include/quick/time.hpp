// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef QUICK_TIME_HPP_
#define QUICK_TIME_HPP_

#include <chrono>  // NOLINT

namespace quick {

inline int64_t GetEpochMicroSeconds() {
  using namespace std::chrono;  // NOLINT
  auto epoch_time = system_clock::now().time_since_epoch();
  return duration_cast<microseconds>(epoch_time).count();
}

class MicroSecondTimer {
 public:
  MicroSecondTimer() {this->Restart();}
  void Restart() {
    start_time = GetEpochMicroSeconds();
  }
  int64_t GetStartTime() const {
    return start_time;
  }
  int64_t GetElapsedTime() const {
    return GetEpochMicroSeconds() - start_time;
  }
 private:
  int64_t start_time;
};

}  // namespace quick

namespace qk = quick;


#endif  // QUICK_TIME_HPP_
