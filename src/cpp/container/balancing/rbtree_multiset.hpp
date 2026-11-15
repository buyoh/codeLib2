#ifndef SRC_CPP_CONTAINER_BALANCING_RBTREE_MULTISET_HPP__
#define SRC_CPP_CONTAINER_BALANCING_RBTREE_MULTISET_HPP__
// %=BEGIN DOC
// %title
// 赤黒木(multimap, 順位)
//
// %overview
// join ベースの赤黒木による順序付き multimap（同じキーを複数持てる）．
// rbtree_set.hpp の multimap 版．各操作を最悪計算量 O(log N) で処理する．
// 二分探索(lower_bound, upper_bound)，順位（左から何番目か），k 番目の要素の取得ができる．
// インデックスはキーの昇順で 0 から数える．同じキーの要素は挿入した順に並ぶ．
// lower_bound / upper_bound はイテレータではなくインデックスを返す．
// key_at / value_at と組み合わせて使う．
// key_at / value_at / erase_at は 0 <= i < size() が事前条件（assert）．
// Key には operator< のみを要求する．
// 返した参照・ポインタは，その要素が削除されるまで有効．
// どの要素を指すか決まらないため，operator[] は持たない．
//
// %usage
// RBTreeMultiSet<Key, Value>()
// int size() const
// bool empty() const
// int insert(const Key& k, const Value& v)
//   ; 同じキーの要素の末尾に挿入し，挿入した位置（インデックス）を返す
// Value* find(const Key& k)
//   ; キーが k の最初の要素．無ければ nullptr（const 版あり）
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
// Value& value_at(int i)
//   ; i 番目の値（const 版あり）
// vector<pair<Key, Value>> to_vector() const
//   ; キーの昇順．同じキーは挿入順
// bool validate() const
//   ; 赤黒木の不変条件の検査（デバッグ用）．O(N)
//
// // 例: キーが k の要素をすべて走査する
// for (int i = s.lower_bound(k), r = s.upper_bound(k); i < r; ++i) { s.value_at(i); }
//
// %require
// ```
#include <cassert>
#include <memory>
#include <utility>
#include <vector>
using namespace std;
// ```
// %verified
// https://atcoder.jp/contests/abc484/submissions/80876457
// %references
// Claude Opus 5.5
// %words
// rbtree, red-black, multimap, multiset, lower_bound, upper_bound
// %=END DOC
// %=BEGIN CODE

template <class Key, class Value>
class RBTreeMultiSet {
  struct Node {
    unique_ptr<Node> l, r;
    Key key;
    Value value;
    bool red;
    int bh;  // 黒高さ．null は 0
    int cnt;
    Node(const Key& k, const Value& v) : key(k), value(v), red(false), bh(1), cnt(1) {}
  };

  // 不変条件: root_ は黒か null
  unique_ptr<Node> root_;

  static bool is_red(const unique_ptr<Node>& p) { return p && p->red; }
  static int bh(const unique_ptr<Node>& p) { return p ? p->bh : 0; }
  static int cnt(const unique_ptr<Node>& p) { return p ? p->cnt : 0; }

  static void pull(Node* p) {
    p->cnt = cnt(p->l) + 1 + cnt(p->r);
    p->bh = bh(p->l) + (p->red ? 0 : 1);
  }
  static void make_black(const unique_ptr<Node>& p) {
    if (p && p->red) {
      p->red = false;
      ++p->bh;
    }
  }

  static void rotate_left(unique_ptr<Node>& t) {
    unique_ptr<Node> c = move(t->r);
    t->r = move(c->l);
    pull(t.get());
    c->l = move(t);
    pull(c.get());
    t = move(c);
  }
  static void rotate_right(unique_ptr<Node>& t) {
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

  Node* node_at_(int i) const {
    assert(0 <= i && i < size());
    Node* p = root_.get();
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

  // インデックス区間 [l, r) の要素を削除する
  void erase_range_(int l, int r) {
    if (l >= r)
      return;
    pair<unique_ptr<Node>, unique_ptr<Node>> s = split_(move(root_), r);
    pair<unique_ptr<Node>, unique_ptr<Node>> t = split_(move(s.first), l);
    root_ = join2(move(t.first), move(s.second));
  }

  static void to_vector_(const Node* p, vector<pair<Key, Value>>& out) {
    if (!p)
      return;
    to_vector_(p->l.get(), out);
    out.push_back(make_pair(p->key, p->value));
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
  RBTreeMultiSet() {}

  int size() const { return cnt(root_); }
  bool empty() const { return !root_; }

  // 同じキーの要素の末尾に挿入する．挿入した位置を返す
  int insert(const Key& k, const Value& v) {
    int i = upper_bound(k);
    pair<unique_ptr<Node>, unique_ptr<Node>> s = split_(move(root_), i);
    root_ = join(move(s.first), unique_ptr<Node>(new Node(k, v)), move(s.second));
    return i;
  }

  Value* find(const Key& k) {
    int i = index_of(k);
    return i < 0 ? nullptr : &node_at_(i)->value;
  }
  const Value* find(const Key& k) const {
    int i = index_of(k);
    return i < 0 ? nullptr : &node_at_(i)->value;
  }

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
  Value& value_at(int i) { return node_at_(i)->value; }
  const Value& value_at(int i) const { return node_at_(i)->value; }

  vector<pair<Key, Value>> to_vector() const {
    vector<pair<Key, Value>> out;
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
#endif  // SRC_CPP_CONTAINER_BALANCING_RBTREE_MULTISET_HPP__
