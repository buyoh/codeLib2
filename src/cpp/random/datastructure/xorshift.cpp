// %title
// XorShift
//
// %overview
// 高速な乱数生成器．64bit版を実装した．
// 一部のrandomヘッダの関数(uniform_int_distribution等)にも使える．
//
// %usage
// XorShift randdev;
// int motivation = uniform_int_distribution<int>(-9, 9)(randdev);
//
// %require
// ```
#include <limits>
#include <cstdint>
using namespace std;
// ```
// %verified
// 目視
//
// %references
// https://ja.wikipedia.org/wiki/Xorshift

struct XorShift {
  using result_type = uint64_t;
  result_type x_;
  XorShift(result_type x = 88172645463325252ull) : x_(x){};
  static constexpr inline result_type min() { return 0ull; }
  static constexpr inline result_type max() { return numeric_limits<result_type>::max(); }
  inline result_type operator()() {
    x_ ^= x_ << 7;
    return x_ ^= x_ >> 9;
  }
  inline void discard(unsigned long long z) {
    while (z--)
      operator()();
  }
};
XorShift randdev;