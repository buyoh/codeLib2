// %title
// 組合せ計算(前計算)
//
// %overview
// inverse, conbination, permutation, factorial を構築時に計算する\
//
//
// %usage
// Combinatorics<2000100, MD> wakame;
//
// %words
// fact,ncr,npr,nck,npk
// %verified
// https://yukicoder.me/submissions/286997
// %references

template <int MaxN, long long Mod>
class Combinatorics {
  using value_type = long long;
  value_type fact_[MaxN + 1];
  // value_type inv_[MaxN + 1]; // iranai

 public:
  constexpr value_type pow(value_type x, value_type p) const {
    value_type y = 1;
    x = x % Mod;
    for (; 0 < p; p >>= 1) {
      if (p & 1)
        y = y * x % Mod;
      x = x * x % Mod;
    }
    return y;
  }

  constexpr Combinatorics() : fact_() {
    fact_[0] = 1;
    fact_[1] = 1;
    for (value_type n = 2; n <= MaxN; ++n)
      fact_[n] = (fact_[n - 1] * n) % Mod;
    // inv_[Max] := calculate;
    // iterate => inv_[i] = inv_[i+1] * (i+1) % MD;
  }
  constexpr value_type fact(int n) const { return fact_[n]; }

  constexpr value_type inv(value_type n) const { return pow(n, MD - 2); }

  constexpr value_type nPr(value_type n, value_type r) const {
    if (r < 0 || n < r)
      return 0;
    return fact_[n] * inv(fact_[n - r]) % MD;
  }
  constexpr value_type nCr(value_type n, value_type r) const {
    if (n < r)
      return 0;
    return ((fact_[n] * inv(fact_[n - r]) % MD) * inv(fact_[r])) % MD;
  }
  constexpr value_type nHr(value_type n, value_type r) const {
    if (n == 0 && r == 0)
      return 1;
    if (n == 0)
      return 0;
    return nCr(n - 1 + r, n - 1);
  }
};