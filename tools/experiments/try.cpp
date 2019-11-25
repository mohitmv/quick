#include <bits/stdc++.h>

namespace std {
template<typename T1, typename T2>
ostream& operator<<(ostream& os, const std::pair<T1, T2>& input) {
  os << "MyTuple(" << input.first << ", " << input.second << ")";
  return os;
}
}


#include <quick/debug.hpp>



using namespace std;




int main() {
  pair<int, int> x = {11, 12};
  cout << x << endl;

  // using T = quick::detail::IsOstreamDefinedForPair<int, int>;

  // cout << quick::test_specialization<quick::detail::IsOstreamDefinedForPair,
  //                                               int,
  //                                               int>::value << endl;

  // cout << test_specialization<IsOstreamDefinedForPair,
  //                                               int,
  //                                               int>::value << endl;


  cout << "pp" << endl;

  return 0;
}

