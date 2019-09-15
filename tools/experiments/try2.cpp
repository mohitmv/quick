#include <iostream>
#include <vector>
#include "quick/debug.hpp"

using namespace std;



int main() {
  pair<int, int> p(22, 44);
  struct Q {
    string DebugString() const {
      return "My DebugString";
    }
  };
  struct QQ {string DebugString() const {return "QQ";}};
  Q q;
  QQ qq;
  cout << "Mohit" << p << ", " << q << ", " << qq << endl;

  cout << "Mohit" << endl;

  return 0;
}

