#ifndef SRC_CPP_CONTAINER_LIKERMQ_BIT_MONOID_HPP__
#define SRC_CPP_CONTAINER_LIKERMQ_BIT_MONOID_HPP__
// %=BEGIN DOC
// %title
// Binary Indexed Tree(モノイド)
//
// %overview
// bit.hpp の演算を一般化したもの．次のクエリを O(log N) で処理する．
// - 1 つの要素 a[i] を operate(a[i], x) にする(add)
// - 前置区間 [0, r) の集約値を求める(prefix)
// - P が群（GroupPolicy）のとき，区間 [l, r) の集約値(prod)・要素の取得(get)・更新(set)
// - 前置区間の集約値に対する二分探索(max_right)
// 0-indexed．区間は半開区間．bit.hpp（1-indexed）とは添字の扱いが異なる．
// 区間の引数（prefix, prod）は [0, size()] に丸める．
// add, get, set は 0 <= i < size() が事前条件（assert）．
//
// Policy P は MonoidPolicy<value_type>（monoid_policy.hpp）を継承する．
// operate は可換であること（型では検査できない）．
// prod, get, set は P が GroupPolicy<value_type> を継承するときのみ使える．
//
// %usage
// explicit BitreeMonoid<P>(int n = 0)
//   ; 長さ n．全要素は unit()
// explicit BitreeMonoid<P>(const vector<value_type>& v)
//   ; v から構築する．O(N)
//   ; 波括弧 1 要素（b({5})）は int n のコンストラクタになる（vector と同様）
// int size() const
// void add(int i, const value_type& x)
//   ; a[i] = operate(a[i], x)
// value_type prefix(int r) const
//   ; operate(a[0], ..., a[r-1])．空なら unit()
// value_type prod(int l, int r) const
//   ; operate(a[l], ..., a[r-1])．空なら unit()．群のみ
// value_type get(int i) const
//   ; a[i]．群のみ
// void set(int i, const value_type& x)
//   ; a[i] = x．群のみ
// int max_right(Pred pred) const
//   ; pred(prefix(r)) が true となる最大の r．
//   ; pred(unit()) == true で，pred(prefix(r)) が r について単調（true の後に false）であること
//
// // 例: 1 点加算・区間和（monoid_policy_impl_sum.hpp を併せて include）
// BitreeMonoid<SumGroup<long long>> b(vector<long long>{3, 1, 4});
// b.add(1, 10);                                     // {3, 11, 4}
// b.prod(1, 3);                                     // 15
// b.max_right([](long long s) { return s <= 14; });  // 2
//
// %require
// ```
#include <cassert>
#include <type_traits>
#include <vector>
using namespace std;
#include "src/cpp/container/policy/monoid_policy.hpp"
// ```
// %verified
//
// %references
// プログラミングコンテストチャレンジブック
// %words
// bit, binary indexed tree, fenwick tree
// %=END DOC
// %=BEGIN CODE

template <class P>
class BitreeMonoid {
 public:
  using value_type = typename P::value_type;
  static_assert(is_base_of<MonoidPolicy<value_type>, P>::value,
                "P must derive from MonoidPolicy<value_type>");

 private:
  // 修飾付きの呼び出しで仮想関数呼び出しを避ける（P は状態を持たない）
  static value_type operate_(const value_type& a, const value_type& b) {
    return P().P::operate(a, b);
  }
  static value_type unit_() { return P().P::unit(); }
  template <class Q = P>
  static value_type inverse_(const value_type& a) {
    return Q().Q::inverse(a);
  }

  int n_;
  vector<value_type> data_;  // 1-indexed．data_[i] は (i - (i & -i), i] の集約値

  int clip_(int i) const { return i < 0 ? 0 : i > n_ ? n_ : i; }

 public:
  explicit BitreeMonoid(int n = 0) : n_(n < 0 ? 0 : n), data_(n_ + 1, unit_()) {}
  explicit BitreeMonoid(const vector<value_type>& v) : n_((int)v.size()), data_(n_ + 1, unit_()) {
    for (int i = 1; i <= n_; ++i) {
      data_[i] = operate_(data_[i], v[i - 1]);
      int j = i + (i & -i);
      if (j <= n_)
        data_[j] = operate_(data_[j], data_[i]);
    }
  }

  int size() const { return n_; }

  void add(int i, const value_type& x) {
    assert(0 <= i && i < n_);
    for (++i; i <= n_; i += i & -i)
      data_[i] = operate_(data_[i], x);
  }

  value_type prefix(int r) const {
    value_type res = unit_();
    for (r = clip_(r); r > 0; r -= r & -r)
      res = operate_(data_[r], res);
    return res;
  }

  template <
      class Q = P,
      typename enable_if<is_same<Q, P>::value && is_base_of<GroupPolicy<value_type>, Q>::value,
                         int>::type = 0>
  value_type prod(int l, int r) const {
    l = clip_(l);
    r = clip_(r);
    if (l >= r)
      return unit_();
    return operate_(inverse_<Q>(prefix(l)), prefix(r));
  }

  template <
      class Q = P,
      typename enable_if<is_same<Q, P>::value && is_base_of<GroupPolicy<value_type>, Q>::value,
                         int>::type = 0>
  value_type get(int i) const {
    assert(0 <= i && i < n_);
    return prod<Q>(i, i + 1);
  }

  template <
      class Q = P,
      typename enable_if<is_same<Q, P>::value && is_base_of<GroupPolicy<value_type>, Q>::value,
                         int>::type = 0>
  void set(int i, const value_type& x) {
    assert(0 <= i && i < n_);
    add(i, operate_(inverse_<Q>(get<Q>(i)), x));
  }

  template <class Pred>
  int max_right(Pred pred) const {
    int k = 1;
    while ((k << 1) <= n_)
      k <<= 1;
    int r = 0;
    value_type acc = unit_();
    for (; k > 0; k >>= 1) {
      if (r + k <= n_) {
        value_type nx = operate_(acc, data_[r + k]);
        if (pred(nx)) {
          r += k;
          acc = nx;
        }
      }
    }
    return r;
  }
};

// %=END CODE
#endif  // SRC_CPP_CONTAINER_LIKERMQ_BIT_MONOID_HPP__
