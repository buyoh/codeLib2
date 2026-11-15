#ifndef SRC_CPP_CONTAINER_BALANCING_RBTREE_MULTISET_MONOID_HPP__
#define SRC_CPP_CONTAINER_BALANCING_RBTREE_MULTISET_MONOID_HPP__
// %=BEGIN DOC
// %title
// 赤黒木(multimap, 区間演算)
//
// %overview
// join ベースの赤黒木による順序付き multimap（同じキーを複数持てる）．
// 各要素に値 value_type を持つ．rbtree_set_monoid.hpp の multimap 版．
// rbtree_multiset.hpp の機能に加えて，
// キー区間 [lo, hi) に対する区間取得(prod)・区間更新(apply)
// （モノイド＋遅延評価）を，最悪計算量 O(log N) で処理する．
// 座標圧縮をせずにキー（座標）で直接区間を扱え，途中でキーの追加・削除もできる．
// 同じキーの要素は挿入した順に並ぶ．
// !(lo < hi) のときは空区間．
// 値の参照を返す API は，書き換えると集約値が壊れるため持たない．
// どの要素を指すか決まらないため，キーで値を読み書きする API（set, get）は持たない．
// 代わりにインデックスで value_at / set_at を使う．
// key_at / value_at / set_at / erase_at は 0 <= i < size() が事前条件（assert）．
// Key には operator< のみを要求する．
//
// Policy P は LazyMonoidPolicy<value_type, operator_type>（monoid_policy.hpp）を継承する．
// MonoidPolicy のみでは使えない．
// 継承は is_base_of で検査する（C++17 のため private 継承も通る）．
//
// %usage
// RBTreeMultiSetMonoid<Key, P>()
// int size() const
// bool empty() const
// int insert(const Key& k, const value_type& x)
//   ; 同じキーの要素の末尾に挿入し，挿入した位置（インデックス）を返す
// bool contains(const Key& k) const
// int count(const Key& k) const
// bool erase_one(const Key& k)
//   ; キーが k の最初の要素を 1 つ削除したら true
// int erase_all(const Key& k)
//   ; キーが k の要素をすべて削除し，削除した個数を返す
// void erase_at(int i)
//   ; i 番目の要素を削除する
// int lower_bound(const Key& k) const
//   ; k 以上の最初の要素のインデックス（k 未満の要素数）
// int upper_bound(const Key& k) const
//   ; k より大きい最初の要素のインデックス（k 以下の要素数）
// int index_of(const Key& k) const
//   ; キーが k の最初の要素のインデックス．無ければ -1
// const Key& key_at(int i) const
//   ; i 番目のキー
// value_type value_at(int i)
//   ; i 番目の値
// void set_at(int i, const value_type& x)
//   ; i 番目の値を x にする
// value_type prod(const Key& lo, const Key& hi)
//   ; lo <= key < hi の値の operate（キーの昇順，同じキーは挿入順）．無ければ unit()
// value_type all_prod() const
//   ; 全体の集約値
// void apply(const Key& lo, const Key& hi, const operator_type& f)
//   ; lo <= key < hi の値に f を作用させる
// vector<pair<Key, value_type>> to_vector()
//   ; キーの昇順．同じキーは挿入順
// bool validate() const
//   ; 赤黒木の不変条件の検査（デバッグ用）．O(N)
//
// // 例: 区間加算・区間最小値（monoid_policy_impl_range_add_range_min.hpp を併せて include）
// RBTreeMultiSetMonoid<int, RangeAddRangeMin<long long>> m;
// m.insert(10, 3); m.insert(20, 1); m.insert(20, 5); m.insert(30, 4);
// m.apply(0, 25, 10);  // {10: 13, 20: 11, 20: 15, 30: 4}
// m.prod(15, 25);      // 11
// m.count(20);         // 2
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
// rbtree, red-black, multimap, multiset, lower_bound, upper_bound
// %=END DOC
// %=BEGIN CODE

template <class Key, class P>
class RBTreeMultiSetMonoid {
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
    Key key;
    value_type val, sum;  // lz は適用済み
    operator_type lz;     // 子に未伝播の作用素
    bool has_lz;
    bool red;
    int bh;  // 黒高さ．null は 0
    int cnt;
    Node(const Key& k, const value_type& v)
        : key(k), val(v), sum(v), lz(id_()), has_lz(false), red(false), bh(1), cnt(1) {}
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
  // 前提: L, R は黒か null．L <= m <= R．m は子を持たない．結果の根は黒
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

  // (先頭 k 要素, 残り)
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
  // キーと cnt は遅延作用素の影響を受けないので，push せずに降りてよい
  const Node* node_at_(int i) const {
    assert(0 <= i && i < size());
    const Node* p = root_.get();
    while (true) {
      int c = cnt(p->l);
      if (i < c) {
        p = p->l.get();
      } else if (i == c) {
        return p;
      } else {
        i -= c + 1;
        p = p->r.get();
      }
    }
  }

  static void set_at_(Node* p, int i, const value_type& x) {
    push(p);
    int c = cnt(p->l);
    if (i < c)
      set_at_(p->l.get(), i, x);
    else if (i == c)
      p->val = x;
    else
      set_at_(p->r.get(), i - c - 1, x);
    pull(p);
  }
  // インデックス区間 [l, r) の要素を削除する
  void erase_range_(int l, int r) {
    if (l >= r)
      return;
    pair<unique_ptr<Node>, unique_ptr<Node>> s = split_(move(root_), r);
    pair<unique_ptr<Node>, unique_ptr<Node>> t = split_(move(s.first), l);
    root_ = join2(move(t.first), move(s.second));
  }

  // p の部分木内のインデックス区間 [l, r)．前提: 0 <= l < r <= cnt(p)
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

  static void to_vector_(Node* p, vector<pair<Key, value_type>>& out) {
    if (!p)
      return;
    push(p);
    to_vector_(p->l.get(), out);
    out.push_back(make_pair(p->key, p->val));
    to_vector_(p->r.get(), out);
  }

  // 黒高さを返す．不正なら -1．prev は直前のキー（キーの広義単調増加の検査）
  static int validate_(const Node* p, const Key*& prev) {
    if (!p)
      return 0;
    if (p->red && (is_red(p->l) || is_red(p->r)))
      return -1;
    int a = validate_(p->l.get(), prev);
    if (prev && p->key < *prev)
      return -1;
    prev = &p->key;
    int b = validate_(p->r.get(), prev);
    if (a < 0 || a != b)
      return -1;
    int h = a + (p->red ? 0 : 1);
    if (h != p->bh || p->cnt != cnt(p->l) + 1 + cnt(p->r))
      return -1;
    return h;
  }

 public:
  RBTreeMultiSetMonoid() {}

  int size() const { return cnt(root_); }
  bool empty() const { return !root_; }

  // 同じキーの要素の末尾に挿入する．挿入した位置を返す
  int insert(const Key& k, const value_type& x) {
    int i = upper_bound(k);
    pair<unique_ptr<Node>, unique_ptr<Node>> s = split_(move(root_), i);
    root_ = join(move(s.first), unique_ptr<Node>(new Node(k, x)), move(s.second));
    return i;
  }

  bool contains(const Key& k) const { return index_of(k) >= 0; }

  int count(const Key& k) const { return upper_bound(k) - lower_bound(k); }

  bool erase_one(const Key& k) {
    int i = index_of(k);
    if (i < 0)
      return false;
    erase_range_(i, i + 1);
    return true;
  }

  int erase_all(const Key& k) {
    int l = lower_bound(k), r = upper_bound(k);
    erase_range_(l, r);
    return r - l;
  }

  void erase_at(int i) {
    assert(0 <= i && i < size());
    erase_range_(i, i + 1);
  }

  int lower_bound(const Key& k) const {
    int res = 0;
    const Node* p = root_.get();
    while (p) {
      if (p->key < k) {
        res += cnt(p->l) + 1;
        p = p->r.get();
      } else {
        p = p->l.get();
      }
    }
    return res;
  }

  int upper_bound(const Key& k) const {
    int res = 0;
    const Node* p = root_.get();
    while (p) {
      if (!(k < p->key)) {
        res += cnt(p->l) + 1;
        p = p->r.get();
      } else {
        p = p->l.get();
      }
    }
    return res;
  }

  int index_of(const Key& k) const {
    int i = lower_bound(k);
    return i < size() && !(k < node_at_(i)->key) ? i : -1;
  }

  const Key& key_at(int i) const { return node_at_(i)->key; }

  value_type value_at(int i) {
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

  void set_at(int i, const value_type& x) {
    assert(0 <= i && i < size());
    set_at_(root_.get(), i, x);
  }

  value_type prod(const Key& lo, const Key& hi) {
    if (!(lo < hi))
      return unit_();
    int l = lower_bound(lo), r = lower_bound(hi);
    if (l >= r)
      return unit_();
    return prod_(root_.get(), l, r);
  }

  value_type all_prod() const { return sum(root_); }

  void apply(const Key& lo, const Key& hi, const operator_type& f) {
    if (!(lo < hi))
      return;
    int l = lower_bound(lo), r = lower_bound(hi);
    if (l >= r)
      return;
    apply_(root_.get(), l, r, f);
  }

  vector<pair<Key, value_type>> to_vector() {
    vector<pair<Key, value_type>> out;
    out.reserve(size());
    to_vector_(root_.get(), out);
    return out;
  }

  bool validate() const {
    const Key* prev = nullptr;
    return !is_red(root_) && validate_(root_.get(), prev) >= 0;
  }
};

// %=END CODE
#endif  // SRC_CPP_CONTAINER_BALANCING_RBTREE_MULTISET_MONOID_HPP__
