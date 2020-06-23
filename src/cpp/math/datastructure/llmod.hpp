// %title
// 剰余演算付き整数型(llmod)
//
// %overview
// MODを法として計算する．
// MODを超える値の引き算を行う際に値を拾うと，負になることがある．
// justifyを呼ぶことで，llmod::val_を[0,MOD) の範囲に収める．
//
// %usage
// [TODO]
// %require
// ```
#include <ostream>
// ```
//
// %verified
// verify: https://yukicoder.me/submissions/183521
// verify: https://yukicoder.me/submissions/183523
//
// %references

class llmod {
 private:
  using value_type = long long;
  value_type val_;
  // inline ll cut(ll v) const { return ((v%MOD) + MOD) % MOD; } // safe
 public:
  static const value_type MOD = 1000000007;  // <=

  llmod() : val_(0) {}
  llmod(value_type num) : val_(((num % MOD) + MOD) % MOD) {}

  inline operator value_type() const { return val_; }
  inline value_type operator*() const { return val_; }
  inline llmod& operator=(const llmod& lm) {
    val_ = lm.val_;
    return *this;
  }
  inline llmod& operator=(value_type v) {
    val_ = (v) % MOD;
    return *this;
  }

  inline llmod& operator+=(value_type v) {
    val_ = (val_ + v) % MOD;
    return *this;
  }
  inline llmod& operator+=(const llmod& l) {
    val_ = (val_ + l.val_) % MOD;
    return *this;
  }
  inline llmod& operator-=(value_type v) {
    val_ = (val_ - v + MOD) % MOD;
    return *this;
  }
  inline llmod& operator-=(const llmod& l) {
    val_ = (val_ - l.val_ + MOD) % MOD;
    return *this;
  }
  inline llmod& operator*=(value_type v) {
    val_ = (val_ * v) % MOD;
    return *this;
  }
  inline llmod& operator*=(const llmod& l) {
    val_ = (val_ * l.val_) % MOD;
    return *this;
  }
  inline llmod& operator++() {
    val_ = (val_ + 1) % MOD;
    return *this;
  }
  inline llmod operator++(int) {
    llmod t = *this;
    val_ = (val_ + 1) % MOD;
    return t;
  }
  inline llmod& justify() {
    val_ = ((val_ % MOD) + MOD) % MOD;
    return *this;
  }
  friend llmod pow(llmod, long long);
};
inline std::ostream& operator<<(std::ostream& os, const llmod& l) {
  os << *l;
  return os;
}

inline llmod operator+(llmod t, const llmod& r) {
  return t += r;
}
inline llmod operator-(llmod t, const llmod& r) {
  return t -= r;
}
inline llmod operator*(llmod t, const llmod& r) {
  return t *= r;
}

// MEMO : 逆元...pow(n,MD-2)
llmod pow(llmod x, long long p) {
  llmod::value_type y = 1;
  llmod::value_type xval = x.justify();
  while (0 < p) {
    if (p & 1)
      y = (xval * y) % llmod::MOD;
    xval = (xval * xval) % llmod::MOD;
    p >>= 1;
  }
  return llmod(y);
}

inline llmod& operator/=(llmod& l, const llmod& r) {
  return l *= pow(r, llmod::MOD - 2);
}