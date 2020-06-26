#ifndef SRC_CPP_MATH_METHOD_GCDLCM_HPP__
#define SRC_CPP_MATH_METHOD_GCDLCM_HPP__
// %=BEGIN
// %title
// 最大公約数・最小公倍数
//
// %overview
//
//
// %usage
// T gcd(T a, T b)
// ; a,bの最大公約数
// T lcm(T a, T b)
// ; a,bの最小公倍数
//
//
// %verified
//
// %references
//
// %words
// gcd,lcm
// %=END

template <typename T>
inline T gcd(T a, T b) {
  while (b) {
    T t = a % b;
    a = b;
    b = t;
  }
  return a;
}
template <typename T>
inline T lcm(T a, T b) {
  return a * b / gcd(a, b);
}
#endif  // SRC_CPP_MATH_METHOD_GCDLCM_HPP__
