#ifndef SRC_CPP_MATH_METHOD_COMBINATORICS_PRECALC_HPP__
#define SRC_CPP_MATH_METHOD_COMBINATORICS_PRECALC_HPP__
// %=BEGIN DOC
// %title
// 組合せ計算(完全前計算)
//
// %overview
// 範囲内の conbination を全て事前に計算しておく。
// 取得は O(1) 時間。
//
// %usage
// Combinatorics<2000100, Mod> wakame;
//
// %words
// ncr, nck
// %verified
// https://atcoder.jp/contests/abc234/submissions/28419041
// %references
// %=END DOC
// %=BEGIN CODE

template <int MaxN, long long Mod>
class nCrTable {
  using value_type = long long;
  value_type table_[MaxN][MaxN];  // nCr[r-1][n-1]; only r <= n

  constexpr void build() {
    for (int n = 0; n < MaxN; ++n)
      table_[0][n] = (n + 1) % Mod;
    for (int n = 1; n < MaxN; ++n) {
      for (int r = 1; r <= n; ++r) {
        table_[r][n] = (table_[r][n - 1] + table_[r - 1][n - 1]) % Mod;
      }
    }
  }

 public:
  constexpr nCrTable() { build(); }

  constexpr value_type nCr(int n, int r) const {
    return n < r ? 0
                 : r > n ? nCr(n, n - r)
                         : n == 0 && r == 0 ? 1 : n <= 0 ? 0 : r <= 0 ? 1 : table_[r - 1][n - 1];
  }

  constexpr value_type nHr(value_type n, value_type r) const {
    if (n == 0 && r == 0)
      return 1;
    if (n == 0)
      return 0;
    return nCr(n - 1 + r, n - 1);
  }
};
// %=END CODE
#endif  // SRC_CPP_MATH_METHOD_COMBINATORICS_PRECALC_HPP__
