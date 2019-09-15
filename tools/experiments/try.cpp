#include <iostream>
#include <vector>

using namespace std;


template<typename...> using void_t = void;
template<typename...> using int_t = int;
template<typename T1, typename T2> using GetFirst = T1;

// template<typename T, std::enable_if_t<std::is_integral<T>::value> = 0>
// GetFirst<void, std::enable_if_t<std::is_integral<T>::value>> F(T x) {
//   cout << "int Type" << x << endl;
// }

// template<typename T, std::enable_if_t<std::is_floating_point<T>::value> = 0>
// GetFirst<void, std::enable_if_t<std::is_floating_point<T>::value>> F(T x) {
//   cout << "float Type" << x << endl;
// }

// template<typename T>
// void F(T x) {
//   cout << "other Type" << x << endl;
// }

// template<typename T>
// void F(T x, std::enable_if_t<std::is_integral<T>::value, int> = 0) {
//   cout << "Integer Type " << x << endl;
// }


// template<typename T>
// void F(T x, std::enable_if_t<std::is_floating_point<T>::value, int> = 0) {
//   cout << "float Type " << x  << endl;
// }

template<typename T>
void F(T x, int = 0) {
  cout << "other Type " << x << endl;
}

template<typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
void F(std::enable_if_t<std::is_integral<T>::value, T> x) {
  cout << "int type " << x << endl;
}


void F2(int x) {
  cout << "F2 int" << endl;
}

template<typename T>
void F2(std::enable_if_t<std::is_floating_point<T>::value, T> x) {
  cout << "F2 Floating" << endl;
}




void F3(int x) {
  cout << "F3 int" << endl;
}

template<typename T>
void F3(T y, std::enable_if_t<std::is_floating_point<T>::value, int> x = 0) {
  cout << "F3 Floating" << endl;
}

template<typename T>
void F3(T y, int_t<decltype(&T::DebugString)> x = 0) {
  cout << "F3 DebugString" << endl;
}


void F4(int x) {
  cout << "F4 int" << endl;
}

template<typename T>
std::enable_if_t<std::is_floating_point<T>::value, void> F4(T y) {
  cout << "F4 Floating" << endl;
}

template<typename T>
void_t<decltype(&T::DebugString)> F4(T y) {
  cout << "F4 DebugString" << endl;
}



// template<class T, typename DummyType = void>
// void G(T) {
//   cout << "Generic G" << endl;
// }

// template <>
// void G<int, void>(int) {
//   cout << "Integer G" << endl;
// }

// template <typename T>
// void G<T, T>(T) {
//   cout << "Floating G" << endl;
// }


template<typename T, typename DummyType = void>
struct H {
  void operator()(T) {
    cout << "Generic Type T" << endl;
  }
};

template<typename T>
struct H<T, std::enable_if_t<std::is_integral<T>::value>> {
  void operator()(T) {
    cout << "Int Type T" << endl;
  }
};


// template<class T>
// std::enable_if_t<std::is_integral<T>::value, void> G() {
//   cout << "Int G" << endl;
// }





int main() {
  // cout << "Mohit" << endl;
  // F(222);
  // F(22.33);
  // F('c');
  H<int>()(11);
  H<float>()(44.55);
  cout << "---------------------------------" << endl;
  vector<int> v1 = {11, 22,44};
  F2<float>(333.44);
  F2(112);
  F2<float>(333);
  cout << "---------------------------------" << endl;
  struct S {string DebugString();};
  S s;
  F3(333.44);
  F3(112);
  F3(s);
  cout << "---------------------------------" << endl;
  F4(333.44);
  F4(112);
  F4(s);


  // G<char>();
  // G<char>('c');
  return 0;
}