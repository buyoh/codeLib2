#ifndef SRC_CPP_CONTAINER_BALANCING_TREAP_SEGMENT_HPP__
#define SRC_CPP_CONTAINER_BALANCING_TREAP_SEGMENT_HPP__
// %=BEGIN DOC
// %title
// Treap(列, 遅延評価)
//
// %overview
// 暗黙キーの Treap．列（配列）を表し，次の操作を期待計算量 O(log N) で処理する．
// - 任意位置への挿入・削除，先頭からの切り出し(split)，末尾への連結(concat)
// - 区間 [l, r) の集約値の取得(prod)，区間 [l, r) への作用(apply)（モノイド＋遅延評価）
// 0-indexed．区間は半開区間．
// 乱数のシードは固定（デバッグ時に再現できるように）．固定シードを狙った入力が心配なら書き換える．
// 区間・位置の引数（insert, split, prod, apply）は [0, size()] に丸める．
// erase, set は範囲外なら何もしない．get は 0 <= i < size() が事前条件（assert）．
// 遅延評価のため，取得系の関数の多くは const ではない．
// 同じ操作を最悪計算量で提供するものに rbtree_segment.hpp がある．
//
// Policy P は LazyMonoidPolicy<value_type, operator_type>（monoid_policy.hpp）を継承する．
// MonoidPolicy のみでは使えない．
// 継承は is_base_of で検査する（C++17 のため private 継承も通る）．
//
// %usage
// TreapSegment<P>()
// explicit TreapSegment<P>(const vector<value_type>& v)
//   ; v から構築する．O(N)
// TreapSegment<P>(int n, const value_type& x)
//   ; x が n 個並んだ列を構築する．O(N)
// int size() const
// bool empty() const
// void insert(int i, const value_type& x)
//   ; i 番目の要素の手前に x を挿入する
// void erase(int i)
//   ; i 番目の要素を削除する
// value_type get(int i)
//   ; i 番目の要素
// void set(int i, const value_type& x)
//   ; i 番目の要素を x にする
// value_type prod(int l, int r)
//   ; operate(a[l], ..., a[r-1])．空なら unit()
// value_type all_prod() const
//   ; 全体の集約値
// void apply(int l, int r, const operator_type& f)
//   ; a[l], ..., a[r-1] に f を作用させる
// void concat(TreapSegment& other)
// void concat(TreapSegment&& other)
//   ; other を末尾に連結し，other を空にする．other が自身なら何もしない
// TreapSegment split(int k)
//   ; 先頭 k 要素を切り出して返す．*this には残りが残る
// vector<value_type> to_vector()
//
// // 例: 区間加算・区間最小値（monoid_policy_impl_range_add_range_min.hpp を併せて include）
// TreapSegment<RangeAddRangeMin<long long>> t(vector<long long>{3, 1, 4});
// t.apply(0, 2, 10);  // {13, 11, 4}
// t.prod(0, 2);       // 11
//
// %require
// ```
#include <cassert>
#include <memory>
#include <random>
#include <type_traits>
#include <utility>
#include <vector>
using namespace std;
#include "src/cpp/container/policy/monoid_policy.hpp"
// ```
// %verified
//
// %references
// https://www.slideshare.net/iwiwi/2-12188757
// http://www.prefield.com/algorithm/container/treap.html
// https://www.lambdanote.com/products/opendatastructures
// %words
// treap, split, merge
// %=END DOC
// %=BEGIN CODE

template <class P>
class TreapSegment {
 public:
  using value_type = typename P::value_type;
  using operator_type = typename P::operator_type;
  static_assert(is_base_of<LazyMonoidPolicy<value_type, operator_type>, P>::value,
                "P must derive from LazyMonoidPolicy<value_type, operator_type>");

 private:
  // 修飾付きの呼び出しで仮想関数呼び出しを避ける（P は状態を持たない）
  static value_type operate_(const value_type& a, const value_type& b) {
    return P().P::operate(a, b);
  }
  static value_type unit_() { return P().P::unit(); }
  static value_type mapping_(const operator_type& f, const value_type& x) {
    return P().P::mapping(f, x);
  }
  static operator_type composition_(const operator_type& f, const operator_type& g) {
    return P().P::composition(f, g);
  }
  static operator_type id_() { return P().P::id(); }

  struct Node {
    unique_ptr<Node> l, r;
    value_type val, sum;  // lz は適用済み
    operator_type lz;     // 子に未伝播の作用素
    bool has_lz;
    unsigned pri;  // 大きいほど根に近い
    int cnt;
    Node(const value_type& v, unsigned p)
        : val(v), sum(v), lz(id_()), has_lz(false), pri(p), cnt(1) {}
  };

  unique_ptr<Node> root_;

  static mt19937& rng() {
    static mt19937 r(1341398u);  // デバッグのためシードは固定
    return r;
  }

  static int cnt(const unique_ptr<Node>& p) { return p ? p->cnt : 0; }
  static value_type sum(const unique_ptr<Node>& p) { return p ? p->sum : unit_(); }

  static void apply_node(Node* p, const operator_type& f) {
    p->val = mapping_(f, p->val);
    p->sum = mapping_(f, p->sum);
    p->lz = p->has_lz ? composition_(f, p->lz) : f;
    p->has_lz = true;
  }
  static void push(Node* p) {
    if (!p->has_lz)
      return;
    if (p->l)
      apply_node(p->l.get(), p->lz);
    if (p->r)
      apply_node(p->r.get(), p->lz);
    p->lz = id_();
    p->has_lz = false;
  }
  // 前提: p は push 済み
  static void pull(Node* p) {
    p->cnt = cnt(p->l) + 1 + cnt(p->r);
    p->sum = operate_(operate_(sum(p->l), p->val), sum(p->r));
  }

  static unique_ptr<Node> merge_(unique_ptr<Node> a, unique_ptr<Node> b) {
    if (!a)
      return b;
    if (!b)
      return a;
    if (a->pri > b->pri) {
      push(a.get());
      a->r = merge_(move(a->r), move(b));
      pull(a.get());
      return a;
    }
    push(b.get());
    b->l = merge_(move(a), move(b->l));
    pull(b.get());
    return b;
  }

  // (先頭 k 要素, 残り)
  static pair<unique_ptr<Node>, unique_ptr<Node>> split_(unique_ptr<Node> t, int k) {
    if (!t)
      return pair<unique_ptr<Node>, unique_ptr<Node>>();
    push(t.get());
    if (k <= cnt(t->l)) {
      pair<unique_ptr<Node>, unique_ptr<Node>> s = split_(move(t->l), k);
      t->l = move(s.second);
      pull(t.get());
      return make_pair(move(s.first), move(t));
    }
    pair<unique_ptr<Node>, unique_ptr<Node>> s = split_(move(t->r), k - cnt(t->l) - 1);
    t->r = move(s.first);
    pull(t.get());
    return make_pair(move(t), move(s.second));
  }

  static unique_ptr<Node> link_(vector<unique_ptr<Node>>& nodes,
                                const vector<int>& lc,
                                const vector<int>& rc,
                                int i) {
    unique_ptr<Node> t = move(nodes[i]);
    if (lc[i] >= 0)
      t->l = link_(nodes, lc, rc, lc[i]);
    if (rc[i] >= 0)
      t->r = link_(nodes, lc, rc, rc[i]);
    pull(t.get());
    return t;
  }
  // Cartesian tree の構築．O(N)
  static unique_ptr<Node> build_(const vector<value_type>& v) {
    int n = (int)v.size();
    if (n == 0)
      return unique_ptr<Node>();
    vector<unique_ptr<Node>> nodes(n);
    vector<int> lc(n, -1), rc(n, -1), st;
    for (int i = 0; i < n; ++i) {
      nodes[i].reset(new Node(v[i], rng()()));
      int last = -1;
      while (!st.empty() && nodes[st.back()]->pri < nodes[i]->pri) {
        last = st.back();
        st.pop_back();
      }
      lc[i] = last;
      if (!st.empty())
        rc[st.back()] = i;
      st.push_back(i);
    }
    return link_(nodes, lc, rc, st[0]);
  }

  static void set_(Node* p, int i, const value_type& x) {
    push(p);
    int c = cnt(p->l);
    if (i < c)
      set_(p->l.get(), i, x);
    else if (i == c)
      p->val = x;
    else
      set_(p->r.get(), i - c - 1, x);
    pull(p);
  }

  // p の部分木内の区間 [l, r)．前提: 0 <= l < r <= cnt(p)
  static value_type prod_(Node* p, int l, int r) {
    if (l == 0 && r == p->cnt)
      return p->sum;
    push(p);
    int c = cnt(p->l);
    value_type res = unit_();
    if (l < c)
      res = prod_(p->l.get(), l, r < c ? r : c);
    if (l <= c && c < r)
      res = operate_(res, p->val);
    if (c + 1 < r)
      res = operate_(res, prod_(p->r.get(), l > c + 1 ? l - c - 1 : 0, r - c - 1));
    return res;
  }
  static void apply_(Node* p, int l, int r, const operator_type& f) {
    if (l == 0 && r == p->cnt) {
      apply_node(p, f);
      return;
    }
    push(p);
    int c = cnt(p->l);
    if (l < c)
      apply_(p->l.get(), l, r < c ? r : c, f);
    if (l <= c && c < r)
      p->val = mapping_(f, p->val);
    if (c + 1 < r)
      apply_(p->r.get(), l > c + 1 ? l - c - 1 : 0, r - c - 1, f);
    pull(p);
  }

  static void to_vector_(Node* p, vector<value_type>& out) {
    if (!p)
      return;
    push(p);
    to_vector_(p->l.get(), out);
    out.push_back(p->val);
    to_vector_(p->r.get(), out);
  }

  int clip_(int i) const { return i < 0 ? 0 : i > size() ? size() : i; }

 public:
  TreapSegment() {}
  explicit TreapSegment(const vector<value_type>& v) : root_(build_(v)) {}
  TreapSegment(int n, const value_type& x) : root_(build_(vector<value_type>(n < 0 ? 0 : n, x))) {}

  int size() const { return cnt(root_); }
  bool empty() const { return !root_; }

  void insert(int i, const value_type& x) {
    i = clip_(i);  // root_ をムーブする前に計算する
    pair<unique_ptr<Node>, unique_ptr<Node>> s = split_(move(root_), i);
    root_ = merge_(merge_(move(s.first), unique_ptr<Node>(new Node(x, rng()()))), move(s.second));
  }

  void erase(int i) {
    if (i < 0 || size() <= i)
      return;
    pair<unique_ptr<Node>, unique_ptr<Node>> s = split_(move(root_), i);
    pair<unique_ptr<Node>, unique_ptr<Node>> t = split_(move(s.second), 1);
    root_ = merge_(move(s.first), move(t.second));
  }

  value_type get(int i) {
    assert(0 <= i && i < size());
    Node* p = root_.get();
    while (true) {
      push(p);
      int c = cnt(p->l);
      if (i < c) {
        p = p->l.get();
      } else if (i == c) {
        return p->val;
      } else {
        i -= c + 1;
        p = p->r.get();
      }
    }
  }

  void set(int i, const value_type& x) {
    if (i < 0 || size() <= i)
      return;
    set_(root_.get(), i, x);
  }

  value_type prod(int l, int r) {
    l = clip_(l);
    r = clip_(r);
    if (l >= r)
      return unit_();
    return prod_(root_.get(), l, r);
  }

  value_type all_prod() const { return sum(root_); }

  void apply(int l, int r, const operator_type& f) {
    l = clip_(l);
    r = clip_(r);
    if (l >= r)
      return;
    apply_(root_.get(), l, r, f);
  }

  void concat(TreapSegment& other) {
    if (&other == this)
      return;
    root_ = merge_(move(root_), move(other.root_));
  }
  void concat(TreapSegment&& other) { concat(other); }

  TreapSegment split(int k) {
    k = clip_(k);  // root_ をムーブする前に計算する
    pair<unique_ptr<Node>, unique_ptr<Node>> s = split_(move(root_), k);
    root_ = move(s.second);
    TreapSegment res;
    res.root_ = move(s.first);
    return res;
  }

  vector<value_type> to_vector() {
    vector<value_type> out;
    out.reserve(size());
    to_vector_(root_.get(), out);
    return out;
  }
};

// %=END CODE
#endif  // SRC_CPP_CONTAINER_BALANCING_TREAP_SEGMENT_HPP__
