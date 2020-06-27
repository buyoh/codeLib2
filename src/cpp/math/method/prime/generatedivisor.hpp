#ifndef SRC_CPP_MATH_METHOD_PRIME_GENERATEDIVISOR_HPP__
#define SRC_CPP_MATH_METHOD_PRIME_GENERATEDIVISOR_HPP__
// %=BEGIN DOC
// %title
// 約数列挙
//
// %overview
// 約数を列挙してvectorに格納する。
// ソートはされていない。
//
// %usage
//
// %require
// ```
#include <vector>
// ```
// %verified
// 10..20
//
// %references
//
// %words
// divisor
// %=END DOC
// %=BEGIN CODE

std::vector<long long> generateDivisor(long long n) {
  std::vector<long long> divs;
  for (long long d = 1; d <= n / d; ++d) {
    if (n / d * d == n) {
      divs.push_back(d);
      divs.push_back(n / d);
    }
  }
  if (divs.size() >= 2 && divs[divs.size() - 1] == divs[divs.size() - 2])
    divs.pop_back();
  return divs;
}
// %=END CODE
#endif  // SRC_CPP_MATH_METHOD_PRIME_GENERATEDIVISOR_HPP__
