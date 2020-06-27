#ifndef SRC_CPP_MATH_METHOD_PRIME_PRIMEDIVISION_HPP__
#define SRC_CPP_MATH_METHOD_PRIME_PRIMEDIVISION_HPP__
// %=BEGIN DOC
// %title
// 素因数分解
//
// %overview
// 素因数分解やるだけ。
// 素数情報を必要としない。（素数情報を使うと早くなるはず）
// 計算量は O(HlogH),H=sqrt(N)
//
// %usage
// map<ll, int> primeDivision(num);
// ; <素数, 個数> の map として格納
//
// %require
// ```
#include <map>
// ```
// %verified
//
// %references
// %=END DOC
// %=BEGIN CODE

std::map<long long, int> primeDivision(const long long _num) {
  std::map<long long, int> result;
  long long num = _num;
  while ((num & 1) == 0) {
    num /= 2;
    ++result[2];
  }
  for (long long i = 3; 1 < num && i <= _num; i += 2) {
    while (num / i * i == num) {
      num /= i;
      ++result[i];
    }
  }
  return result;
}
// %=END CODE
#endif  // SRC_CPP_MATH_METHOD_PRIME_PRIMEDIVISION_HPP__
