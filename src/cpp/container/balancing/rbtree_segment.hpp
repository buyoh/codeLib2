#ifndef SRC_CPP_CONTAINER_BALANCING_RBTREE_SEGMENT_HPP__
#define SRC_CPP_CONTAINER_BALANCING_RBTREE_SEGMENT_HPP__
// %=BEGIN DOC
// %title
// 赤黒木(列, 遅延評価)
//
// %overview
// join ベースの赤黒木による列（配列）．treap_segment.hpp と同じ操作を持ち，
// 各操作を最悪計算量 O(log N) で処理する（乱数を使わない）．
// - 任意位置への挿入・削除，先頭からの切り出し(split)，末尾への連結(concat)
// - 区間 [l, r) の集約値の取得(prod)，区間 [l, r) への作用(apply)（モノイド＋遅延評価）
// 0-indexed．区間は半開区間．
// 区間・位置の引数（insert, split, prod, apply）は [0, size()] に丸める．
// erase, set は範囲外なら何もしない．get は 0 <= i < size() が事前条件（assert）．
// 遅延評価のため，取得系の関数の多くは const ではない．
//
// 挿入・削除・分割・連結は，split と join（黒高さの異なる 2 つの木を
// 1 つの要素を挟んで連結する操作）で組み立てる．
//
// Policy P は LazyMonoidPolicy<value_type, operator_type>（monoid_policy.hpp）を継承する．
// MonoidPolicy のみでは使えない．
// 継承は is_base_of で検査する（C++17 のため private 継承も通る）．
//
// %usage
// RBTreeSegment<P>()
// explicit RBTreeSegment<P>(const vector<value_type>& v)
//   ; v から構築する．O(N)
// RBTreeSegment<P>(int n, const value_type& x)
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
// void concat(RBTreeSegment& other)
// void concat(RBTreeSegment&& other)
//   ; other を末尾に連結し，other を空にする．other が自身なら何もしない
// RBTreeSegment split(int k)
//   ; 先頭 k 要素を切り出して返す．*this には残りが残る
// vector<value_type> to_vector()
// bool validate() const
//   ; 赤黒木の不変条件の検査（デバッグ用）．O(N)
//
// // 例: 区間加算・区間最小値（monoid_policy_impl_range_add_range_min.hpp を併せて include）
// RBTreeSegment<RangeAddRangeMin<long long>> t(vector<long long>{3, 1, 4});
// t.apply(0, 2, 10);  // {13, 11, 4}
// t.prod(0, 2);       // 11
//
// %require
// ```
#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>
using namespace std;
#include "src/cpp/container/policy/monoid_policy.hpp"
// ```
// %verified
//
// %references
// Claude Opus 5.5
// %words
// rbtree, red-black, join, split
// %=END DOC
// %=BEGIN CODE

template <class P>
class RBTreeSegment {
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
    bool red;
    int bh;  // 黒高さ．null は 0
    int cnt;
    explicit Node(const value_type& v)
        : val(v), sum(v), lz(id_()), has_lz(false), red(false), bh(1), cnt(1) {}
  };

  // 不変条件: root_ は黒か null
  unique_ptr<Node> root_;

  static bool is_red(const unique_ptr<Node>& p) { return p && p->red; }
  static int bh(const unique_ptr<Node>& p) { return p ? p->bh : 0; }
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
    p->bh = bh(p->l) + (p->red ? 0 : 1);
    p->sum = operate_(operate_(sum(p->l), p->val), sum(p->r));
  }
  // pull しない（p は未伝播の作用素を持つことがある）
  static void make_black(const unique_ptr<Node>& p) {
    if (p && p->red) {
      p->red = false;
      ++p->bh;
    }
  }

  static void rotate_left(unique_ptr<Node>& t) {
    push(t.get());
    push(t->r.get());
    unique_ptr<Node> c = move(t->r);
    t->r = move(c->l);
    pull(t.get());
    c->l = move(t);
    pull(c.get());
    t = move(c);
  }
  static void rotate_right(unique_ptr<Node>& t) {
    push(t.get());
    push(t->l.get());
    unique_ptr<Node> c = move(t->l);
    t->l = move(c->r);
    pull(t.get());
    c->r = move(t);
    pull(c.get());
    t = move(c);
  }

  // 前提: bh(L) >= bh(R)，R は黒か null，m は子を持たない
  static unique_ptr<Node> join_right(unique_ptr<Node> L, unique_ptr<Node> m, unique_ptr<Node> R) {
    if (!is_red(L) && bh(L) == bh(R)) {
      m->l = move(L);
      m->r = move(R);
      m->red = true;
      pull(m.get());
      return m;
    }
    push(L.get());
    L->r = join_right(move(L->r), move(m), move(R));
    if (!L->red && is_red(L->r) && is_red(L->r->r)) {
      L->r->r->red = false;
      ++L->r->r->bh;
      rotate_left(L);
    } else {
      pull(L.get());
    }
    return L;
  }
  // 前提: bh(L) <= bh(R)，L は黒か null，m は子を持たない
  static unique_ptr<Node> join_left(unique_ptr<Node> L, unique_ptr<Node> m, unique_ptr<Node> R) {
    if (!is_red(R) && bh(L) == bh(R)) {
      m->l = move(L);
      m->r = move(R);
      m->red = true;
      pull(m.get());
      return m;
    }
    push(R.get());
    R->l = join_left(move(L), move(m), move(R->l));
    if (!R->red && is_red(R->l) && is_red(R->l->l)) {
      R->l->l->red = false;
      ++R->l->l->bh;
      rotate_right(R);
    } else {
      pull(R.get());
    }
    return R;
  }
  // 前提: L, R は黒か null．m は子を持たない．結果の根は黒
  static unique_ptr<Node> join(unique_ptr<Node> L, unique_ptr<Node> m, unique_ptr<Node> R) {
    unique_ptr<Node> t;
    if (bh(L) > bh(R)) {
      t = join_right(move(L), move(m), move(R));
    } else if (bh(L) < bh(R)) {
      t = join_left(move(L), move(m), move(R));
    } else {
      m->l = move(L);
      m->r = move(R);
      m->red = false;
      pull(m.get());
      t = move(m);
    }
    make_black(t);
    return t;
  }

  // (先頭 k 要素, 残り)．どちらも根は黒か null
  static pair<unique_ptr<Node>, unique_ptr<Node>> split_(unique_ptr<Node> t, int k) {
    if (!t)
      return pair<unique_ptr<Node>, unique_ptr<Node>>();
    push(t.get());
    unique_ptr<Node> L = move(t->l), R = move(t->r);
    make_black(L);
    make_black(R);
    int c = cnt(L);
    if (k <= c) {
      pair<unique_ptr<Node>, unique_ptr<Node>> s = split_(move(L), k);
      return make_pair(move(s.first), join(move(s.second), move(t), move(R)));
    }
    pair<unique_ptr<Node>, unique_ptr<Node>> s = split_(move(R), k - c - 1);
    return make_pair(join(move(L), move(t), move(s.first)), move(s.second));
  }

  static unique_ptr<Node> join2(unique_ptr<Node> L, unique_ptr<Node> R) {
    if (!L)
      return R;
    if (!R)
      return L;
    int n = cnt(L);
    pair<unique_ptr<Node>, unique_ptr<Node>> s = split_(move(L), n - 1);
    return join(move(s.first), move(s.second), move(R));
  }

  // 中点で分割した平衡木．深さ D のノードのみ赤
  static unique_ptr<Node> build_(const vector<value_type>& v, int l, int r, int depth, int D) {
    if (l >= r)
      return unique_ptr<Node>();
    int mid = (l + r) / 2;
    unique_ptr<Node> t(new Node(v[mid]));
    t->l = build_(v, l, mid, depth + 1, D);
    t->r = build_(v, mid + 1, r, depth + 1, D);
    t->red = (depth == D);
    pull(t.get());
    return t;
  }
  static unique_ptr<Node> build_(const vector<value_type>& v) {
    int n = (int)v.size();
    int D = 0;  // floor(log2(n + 1))
    while ((2LL << D) <= (long long)n + 1)
      ++D;
    return build_(v, 0, n, 0, D);
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

  // 黒高さを返す．不正なら -1
  static int validate_(const Node* p) {
    if (!p)
      return 0;
    if (p->red && (is_red(p->l) || is_red(p->r)))
      return -1;
    int a = validate_(p->l.get());
    int b = validate_(p->r.get());
    if (a < 0 || a != b)
      return -1;
    int h = a + (p->red ? 0 : 1);
    if (h != p->bh || p->cnt != cnt(p->l) + 1 + cnt(p->r))
      return -1;
    return h;
  }

  int clip_(int i) const { return i < 0 ? 0 : i > size() ? size() : i; }

 public:
  RBTreeSegment() {}
  explicit RBTreeSegment(const vector<value_type>& v) : root_(build_(v)) {}
  RBTreeSegment(int n, const value_type& x) : root_(build_(vector<value_type>(n < 0 ? 0 : n, x))) {}

  int size() const { return cnt(root_); }
  bool empty() const { return !root_; }

  void insert(int i, const value_type& x) {
    i = clip_(i);  // root_ をムーブする前に計算する
    pair<unique_ptr<Node>, unique_ptr<Node>> s = split_(move(root_), i);
    root_ = join(move(s.first), unique_ptr<Node>(new Node(x)), move(s.second));
  }

  void erase(int i) {
    if (i < 0 || size() <= i)
      return;
    pair<unique_ptr<Node>, unique_ptr<Node>> s = split_(move(root_), i);
    pair<unique_ptr<Node>, unique_ptr<Node>> t = split_(move(s.second), 1);
    root_ = join2(move(s.first), move(t.second));
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

  void concat(RBTreeSegment& other) {
    if (&other == this)
      return;
    root_ = join2(move(root_), move(other.root_));
  }
  void concat(RBTreeSegment&& other) { concat(other); }

  RBTreeSegment split(int k) {
    k = clip_(k);  // root_ をムーブする前に計算する
    pair<unique_ptr<Node>, unique_ptr<Node>> s = split_(move(root_), k);
    root_ = move(s.second);
    RBTreeSegment res;
    res.root_ = move(s.first);
    return res;
  }

  vector<value_type> to_vector() {
    vector<value_type> out;
    out.reserve(size());
    to_vector_(root_.get(), out);
    return out;
  }

  bool validate() const { return !is_red(root_) && validate_(root_.get()) >= 0; }
};

// %=END CODE
#endif  // SRC_CPP_CONTAINER_BALANCING_RBTREE_SEGMENT_HPP__
