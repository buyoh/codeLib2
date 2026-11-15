#ifndef SRC_CPP_CONTAINER_LIKERMQ_RMQ_SPARSETABLE_MONOID_HPP__
#define SRC_CPP_CONTAINER_LIKERMQ_RMQ_SPARSETABLE_MONOID_HPP__
// %=BEGIN DOC
// %title
// SparseTable(冪等モノイド)
//
// %overview
// rmq_sparsetable.hpp の演算を一般化したもの．
// 構築済みの配列に対して，区間 [l, r) の集約値を O(1) で求める．構築は O(N log N)．
// 0-indexed．区間は半開区間．区間の引数は [0, size()] に丸める．
// 構築後に要素を更新することはできない．
//
// Policy P は MonoidPolicy<value_type>（monoid_policy.hpp）を継承する．
// operate は冪等（operate(a, a) == a）であること（型では検査できない）．可換は仮定しない．
// 例: min, max, gcd, bitwise and / or
// rmq_sparsetable.hpp と異なり，最小値の位置（インデックス）は求めない．
// 位置が必要なら value_type を (値, 位置) の組にする．
//
// %usage
// SparseTableMonoid<P>()
// explicit SparseTableMonoid<P>(const vector<value_type>& v)
//   ; v から構築する．O(N log N)
// int size() const
// value_type prod(int l, int r) const
//   ; operate(a[l], ..., a[r-1])．空なら unit()
//
// // 例: 区間最小値（monoid_policy_impl_min.hpp を併せて include）
// SparseTableMonoid<MinMonoid<int>> st(vector<int>{3, 1, 4});
// st.prod(0, 2);  // 1
//
// %require
// ```
#include <type_traits>
#include <utility>
#include <vector>
using namespace std;
#include "src/cpp/container/policy/monoid_policy.hpp"
// ```
// %verified
//
// %references
// http://tookunn.hatenablog.com/entry/2016/07/13/211148
// %words
// sparse table, rmq
// %=END DOC
// %=BEGIN CODE

template <class P>
class SparseTableMonoid {
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

  vector<int> log2_;                  // log2_[i] = floor(log2(i))
  vector<vector<value_type>> table_;  // table_[k][i] は [i, i + 2^k) の集約値

  int clip_(int i) const { return i < 0 ? 0 : i > size() ? size() : i; }

 public:
  SparseTableMonoid() : log2_(1, 0) {}
  explicit SparseTableMonoid(const vector<value_type>& v) : log2_(v.size() + 1, 0) {
    const int n = (int)v.size();
    for (int i = 2; i <= n; ++i)
      log2_[i] = log2_[i >> 1] + 1;
    table_.push_back(v);
    for (int k = 1; (1 << k) <= n; ++k) {
      const vector<value_type>& prev = table_[k - 1];
      vector<value_type> cur;
      cur.reserve(n - (1 << k) + 1);
      for (int i = 0; i + (1 << k) <= n; ++i)
        cur.push_back(operate_(prev[i], prev[i + (1 << (k - 1))]));
      table_.push_back(move(cur));
    }
  }

  int size() const { return (int)log2_.size() - 1; }

  value_type prod(int l, int r) const {
    l = clip_(l);
    r = clip_(r);
    if (l >= r)
      return unit_();
    const int k = log2_[r - l];
    return operate_(table_[k][l], table_[k][r - (1 << k)]);
  }
};

// %=END CODE
#endif  // SRC_CPP_CONTAINER_LIKERMQ_RMQ_SPARSETABLE_MONOID_HPP__
