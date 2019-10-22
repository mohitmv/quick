#include <unistd.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using guard = std::lock_guard<std::mutex>;

std::mutex print_mutex;
std::once_flag flag;


void print_impl(std::ostream&) {}

template<typename T, typename... Ts>
void print_impl(std::ostream& os, const T& a, const Ts& ... b) {
  os << a;
  print_impl(os, b...);
}

template<typename... Ts>
void print(const Ts&... a) {
  guard _(print_mutex);
  print_impl(cout, a...);
  cout << endl;
}

void may_throw_function(bool do_throw, int tid) {
  if (do_throw) {
    print("[", tid, "]: throw: call_once");
    throw std::runtime_error("");
  }
  print("[", tid, "]: Didn't throw, call_once");
}


void do_once(bool do_throw, int tid) {
  if (not do_throw) {
    usleep(1000);
  }
  print("[", tid, "]: Do once started ");
  try {
    print("[", tid, "]: Waiting on std::call_once ");
    std::call_once(flag, may_throw_function, do_throw, tid);
  } catch (...) {
    print("[", tid, "]: exception cought! ");
  }
}

#define LINE(a) print("[Line: ", __LINE__, "]:", a)
 
int main() {
    std::thread t1(do_once, true, 1);
    std::thread t2(do_once, true, 2);
    std::thread t3(do_once, false, 3);
    std::thread t4(do_once, true, 4);
    t1.join();
    LINE("");
    t2.join();
    LINE("");
    t3.join();
    LINE("");
    t4.join();
    cout << "end" << endl;
}

