#ifndef SRC_CPP_CONTAINER_BALANCING_RBTREE_SET_HPP__
#define SRC_CPP_CONTAINER_BALANCING_RBTREE_SET_HPP__
// %=BEGIN DOC
// %title
// 赤黒木(map, 順位)
//
// %overview
// join ベースの赤黒木による順序付き map（キーは一意）．
// 各操作を最悪計算量 O(log N) で処理する．treap_set.hpp の機能に加えて，
// 二分探索(lower_bound, upper_bound)，順位（左から何番目か），k 番目の要素の取得ができる．
// インデックスはキーの昇順で 0 から数える．
// lower_bound / upper_bound はイテレータではなくインデックスを返す．
// key_at / value_at と組み合わせて使う．
// key_at / value_at は 0 <= i < size() が事前条件（assert）．
// Key には operator< のみを要求する．
// 返した参照・ポインタは，その要素が削除されるまで有効．
// 同じキーを複数持つ multimap 版は rbtree_multiset.hpp．
//
// %usage
// RBTreeSet<Key, Value>()
// int size() const
// bool empty() const
// Value& operator[](const Key& k)
//   ; k に対応する値の参照．無ければ Value() で作る
// bool insert(const Key& k, const Value& v)
//   ; k が無ければ挿入して true．あれば何もせず false
// Value* find(const Key& k)
//   ; 無ければ nullptr（const 版あり）
// bool erase(const Key& k)
//   ; 削除したら true
// int lower_bound(const Key& k) const
//   ; k 以上の最初の要素のインデックス（k 未満の要素数）
// int upper_bound(const Key& k) const
//   ; k より大きい最初の要素のインデックス（k 以下の要素数）
// int index_of(const Key& k) const
//   ; k のインデックス．無ければ -1
// const Key& key_at(int i) const
//   ; i 番目のキー
// Value& value_at(int i)
//   ; i 番目の値（const 版あり）
// vector<pair<Key, Value>> to_vector() const
//   ; キーの昇順
// bool validate() const
//   ; 赤黒木の不変条件の検査（デバッグ用）．O(N)
//
// // 例: x 以上で最小のキー
// int i = s.lower_bound(x);
// if (i < s.size()) { const Key& k = s.key_at(i); }
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
//
// %references
// Claude Opus 5.5
// %words
// rbtree, red-black, map, set, lower_bound, upper_bound
// %=END DOC
// %=BEGIN CODE

template <class Key, class Value>
class RBTreeSet {
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
  // 前提: L, R は黒か null．L < m < R．m は子を持たない．結果の根は黒
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
  // (k 未満, k 以上)
  static pair<unique_ptr<Node>, unique_ptr<Node>> split_key_(unique_ptr<Node> t, const Key& k) {
    if (!t)
      return pair<unique_ptr<Node>, unique_ptr<Node>>();
    unique_ptr<Node> L = move(t->l), R = move(t->r);
    make_black(L);
    make_black(R);
    if (!(t->key < k)) {
      pair<unique_ptr<Node>, unique_ptr<Node>> s = split_key_(move(L), k);
      return make_pair(move(s.first), join(move(s.second), move(t), move(R)));
    }
    pair<unique_ptr<Node>, unique_ptr<Node>> s = split_key_(move(R), k);
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

  Node* find_(const Key& k) const {
    Node* p = root_.get();
    while (p) {
      if (k < p->key)
        p = p->l.get();
      else if (p->key < k)
        p = p->r.get();
      else
        return p;
    }
    return nullptr;
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
  // 新しいノードを挿入する．前提: k は存在しない
  Node* insert_new_(const Key& k, const Value& v) {
    unique_ptr<Node> n(new Node(k, v));
    Node* raw = n.get();
    pair<unique_ptr<Node>, unique_ptr<Node>> s = split_key_(move(root_), k);
    root_ = join(move(s.first), move(n), move(s.second));
    return raw;
  }

  static void to_vector_(const Node* p, vector<pair<Key, Value>>& out) {
    if (!p)
      return;
    to_vector_(p->l.get(), out);
    out.push_back(make_pair(p->key, p->value));
    to_vector_(p->r.get(), out);
  }

  // 黒高さを返す．不正なら -1．prev は直前のキー（キーの狭義単調増加の検査）
  static int validate_(const Node* p, const Key*& prev) {
    if (!p)
      return 0;
    if (p->red && (is_red(p->l) || is_red(p->r)))
      return -1;
    int a = validate_(p->l.get(), prev);
    if (prev && !(*prev < p->key))
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
  RBTreeSet() {}

  int size() const { return cnt(root_); }
  bool empty() const { return !root_; }

  Value& operator[](const Key& k) {
    Node* p = find_(k);
    if (!p)
      p = insert_new_(k, Value());
    return p->value;
  }

  bool insert(const Key& k, const Value& v) {
    if (find_(k))
      return false;
    insert_new_(k, v);
    return true;
  }

  Value* find(const Key& k) {
    Node* p = find_(k);
    return p ? &p->value : nullptr;
  }
  const Value* find(const Key& k) const {
    const Node* p = find_(k);
    return p ? &p->value : nullptr;
  }

  bool erase(const Key& k) {
    if (!find_(k))
      return false;
    pair<unique_ptr<Node>, unique_ptr<Node>> s = split_key_(move(root_), k);
    pair<unique_ptr<Node>, unique_ptr<Node>> t =
        split_(move(s.second), 1);  // t.first は k を持つノード
    root_ = join2(move(s.first), move(t.second));
    return true;
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
    int res = 0;
    const Node* p = root_.get();
    while (p) {
      if (k < p->key) {
        p = p->l.get();
      } else if (p->key < k) {
        res += cnt(p->l) + 1;
        p = p->r.get();
      } else {
        return res + cnt(p->l);
      }
    }
    return -1;
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
#endif  // SRC_CPP_CONTAINER_BALANCING_RBTREE_SET_HPP__
