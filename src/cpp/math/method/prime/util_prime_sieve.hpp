#ifndef SRC_CPP_MATH_METHOD_PRIME_UTIL_PRIME_SIEVE_HPP__
#define SRC_CPP_MATH_METHOD_PRIME_UTIL_PRIME_SIEVE_HPP__
// %=BEGIN DOC
// %title
// 素数（エラトステネスの篩）
//
// %overview
// エラトステネスの篩
// 判定する素数の上限を静的に決める。
// 素数判定、素数列挙、素因数分解が可能。
// メモリを (2*Max)*4byte ぐらい消費するが、競技ならこれで十分
//
// %usage
//
// %require
// ```
#include <map>
// ```
// %verified
// https://yukicoder.me/submissions/696483
//
// %references
//
// %=END DOC
// %=BEGIN CODE
// template <int Max = 2000>
template <int Max>
class Prime {
  int d_[Max + 1];
  int n_;
  int li_[std::max(10000, Max / 10)];

 public:
  constexpr Prime() : d_(), n_(), li_() {
    d_[0] = d_[1] = 0;
    for (int i = 2; i <= Max; i += 2) {
      d_[i] = 2;
    }
    n_ = 1;
    li_[0] = 2;
    int p = 3;
    for (p = 3; p * p <= Max; p += 2) {
      if (d_[p] != 0)
        continue;
      d_[p] = p;
      li_[n_++] = p;
      for (int j = p * p; j <= Max; j += p) {  // i*i
        d_[j] = p;
      }
    }
    for (; p <= Max; p += 2) {
      if (d_[p] != 0)
        continue;
      d_[p] = p;
      li_[n_++] = p;
    }
  }
  constexpr inline bool isPrime(int x) const { return (x >= 2) && (x == 2 || d_[x] == x); }
  constexpr inline int operator[](int i) const { return li_[i]; }

  class iterator {
    const Prime& pl;
    int ptr = 0;

   public:
    constexpr iterator(const decltype(pl)& _pl, int _ptr = 0) : pl(_pl), ptr(_ptr) {}
    constexpr int operator*() const { return pl[ptr]; }
    constexpr iterator& operator++() {
      ptr++;
      return *this;
    }  // prefix
    constexpr inline bool operator!=(const iterator& it) const {
      return ptr != it.ptr ? !(pl.n_ <= ptr && pl.n_ <= it.ptr) : false;
    }
    constexpr inline bool operator==(const iterator& it) const {
      return ptr != it.ptr ? (pl.n_ <= ptr && pl.n_ <= it.ptr) : true;
    }
  };
  constexpr Prime::iterator begin() const { return Prime::iterator(*this, 0); }
  constexpr Prime::iterator end() const { return Prime::iterator(*this, n_); }

  std::map<int, int> division(long long number) const {
    std::map<int, int> div;
    // for large number
    for (int i = 0; (long long)Max <= number && i < n_; ++i) {
      long long p = li_[i];
      int c = 0;
      while (number / p * p == number)
        ++c, number /= p;
      if (c > 0)
        div[(int)p] = c;
    }
    if ((long long)Max <= number) {
      // guess it's prime number.
      div[number] += 1;
      return div;
    }
    while (number >= 2) {
      long long p = d_[number];
      int c = 0;
      while (number / p * p == number)
        ++c, number /= p;
      if (c > 0)
        div[(int)p] = c;
    }
    return div;
  }
};

// %=END CODE
#endif  // SRC_CPP_MATH_METHOD_PRIME_UTIL_PRIME_SIEVE_HPP__
