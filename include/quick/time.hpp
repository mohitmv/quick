// Copyright: 2019 Mohit Saini
// Author: Mohit Saini (mohitsaini1196@gmail.com)

#ifndef QUICK_TIME_HPP_
#define QUICK_TIME_HPP_

#include <chrono>

namespace quick {

long long GetEpochMicroSeconds() {
  using namespace std::chrono;
  auto epoch_time = system_clock::now().time_since_epoch();
  return duration_cast<milliseconds>(epoch_time).count();
}

class MicroSecondTimer {
public:
  MicroSecondTimer() {this->Restart();}
  void Restart() {
    start_time = GetEpochMicroSeconds();
  }
  long long GetStartTime() const {
    return start_time;
  }
  long long GetElapsedTime() const {
    return GetEpochMicroSeconds() - start_time;
  }
  long long start_time;
};

}  // namespace quick

namespace qk = quick;


#endif  // QUICK_TIME_HPP_
