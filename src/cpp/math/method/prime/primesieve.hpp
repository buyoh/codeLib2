#ifndef SRC_CPP_MATH_METHOD_PRIME_PRIMESIEVE_HPP__
#define SRC_CPP_MATH_METHOD_PRIME_PRIMESIEVE_HPP__
// %=BEGIN DOC
// %title
// エラトステネスの篩
//
// %overview
// エラトステネスの篩
// 生成する素数の数を静的に決めてしまう．
// コンパイルが異様に遅くなるので(compile TLE)，
// 競技プログラミングではconstexprを付けないほうが無難
//
// PrimeList は IsPrime を引数に取って生成することができる。
// メモリはより消費するが、事前情報なしと比較してずっと速いのでこちらを使うべき。
// PrimeList のサイズも IsPrime のサイズもそれぞれ適切な値を指定する必要があり、面倒。
// 例えば、IsPrime<200117> に対して PrimeList<17994>、
// IsPrime<1000117> に対して PrimeList<78505> など。
//
// %usage
// constexpr IsPrime<9999> ipc;
// ; `ipc[x] == true` なら，xは!!素数!!
//
// constexpr PrimeList<9999> plc;
// ; `plc[i]` i 番目の素数
// ; イテレータあるよ
// map<ll, int> PrimeListC::divison(ll);
// ; 素因数分解する．
//
// %require
// ```
#include <map>
#include <bitset>
// ```
// %verified
// [TODO]
//
// %references
//
// %=END DOC
// %=BEGIN CODE


template <int Max = 2000>
class IsPrime {
 public:  // friend PrimeList
  std::bitset<Max/2 + 1> d_;  // 1,3,5,...

 public:
  // constexpr IsPrime() : d_() {
  IsPrime() : d_() {
    d_[0] = 1;
    for (int i = 3; i*i <= Max; i += 2) {
      if (d_[i/2] == 0) {
        for (int j = i * i; j <= Max; j += i) {  // i*i
          if (j&1) d_.set(j/2);
        }
      }
    }
    for (int i = 0; i <= Max/2; ++i)
      d_[i] = !d_[i];
  }
  constexpr inline bool operator[](int x) const {
    return (x <= 2) ? (x == 2) : (x&1) ? d_[x/2] : false;
  }
};

template <int Cnt = 2000>
class PrimeList {
  int d_[Cnt];

 public:
  // constexpr PrimeListC() : d_() {
  PrimeList() : d_() {
    clog << "we should construct with IsPrimeC" << endl;
    abort();
    int n = 1;
    d_[0] = 2;
    for (int x = 3; n < Cnt; ++x) {
      bool f = true;
      for (int i = 0; d_[i] * d_[i] <= x; ++i)
        if (x / d_[i] * d_[i] == x) {
          f = false;
          break;
        }
      if (f)
        d_[n++] = x;
    }
  }
  template<int M>
  PrimeList(const IsPrime<M>& is_prime) : d_() {
    int p = 0;
    d_[p++] = 2;
    for (int x = 1; x < M/2; ++x) {
      if (is_prime.d_[x]) {
        d_[p++] = x*2 + 1;
      }
    }
  }
  constexpr inline int operator[](int x) const { return d_[x]; }
  template <int _Cnt>
  class iterator {
    const PrimeList<_Cnt>& pl;
    int ptr = 0;

   public:
    constexpr iterator(const decltype(pl)& _pl, int _ptr = 0) : pl(_pl), ptr(_ptr) {}
    constexpr int operator*() const { return pl[ptr]; }
    constexpr iterator<_Cnt>& operator++() {
      ptr++;
      return *this;
    }  // prefix
    constexpr inline bool operator!=(const iterator<_Cnt>& it) const {
      return ptr != it.ptr ? !(_Cnt < ptr && _Cnt < it.ptr) : false;
    }
    constexpr inline bool operator==(const iterator<_Cnt>& it) const {
      return ptr != it.ptr ? (_Cnt < ptr && _Cnt < it.ptr) : true;
    }
  };
  constexpr PrimeList::iterator<Cnt> begin() const { return PrimeList::iterator<Cnt>(*this, 0); }
  constexpr PrimeList::iterator<Cnt> end() const { return PrimeList::iterator<Cnt>(*this, Cnt); }

  std::map<long long, int> division(long long number) const {
    std::map<long long, int> div;
    for (int i = 0; number > 1 && i < Cnt; ++i) {
      long long p = d_[i];
      int c = 0;
      while (number / p * p == number)
        ++c, number /= p;
      if (c > 0)
        div[p] = c;
    }
    return div;
  }
};
// %=END CODE
#endif  // SRC_CPP_MATH_METHOD_PRIME_PRIMESIEVE_HPP__
