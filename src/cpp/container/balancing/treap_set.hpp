#ifndef SRC_CPP_CONTAINER_BALANCING_TREAP_SET_HPP__
#define SRC_CPP_CONTAINER_BALANCING_TREAP_SET_HPP__
// %=BEGIN DOC
// %title
// Treap(map)
//
// %overview
// Treap による順序付き map（キーは一意）．
// Treap は，キーについて二分探索木であり，乱数で決めた優先度について heap である確率的平衡二分木．
// 各操作の期待計算量は O(log N)．
// 乱数のシードは固定（デバッグ時に再現できるように）．固定シードを狙った入力が心配なら書き換える．
// Key には operator< のみを要求する．
// 返した参照・ポインタは，その要素が削除されるまで有効．
// 二分探索・順位・区間演算が必要なら rbtree_set.hpp / rbtree_set_monoid.hpp を使う．
//
// %usage
// TreapSet<Key, Value>()
// int size() const
// bool empty() const
// Value& operator[](const Key& k)
//   ; k に対応する値の参照．無ければ Value() で作る
// Value* find(const Key& k)
//   ; 無ければ nullptr（const 版あり）
// bool erase(const Key& k)
//   ; 削除したら true
//
// %require
// ```
#include <memory>
#include <random>
#include <utility>
using namespace std;
// ```
// %verified
//
// %references
// https://www.slideshare.net/iwiwi/2-12188757
// http://www.prefield.com/algorithm/container/treap.html
// https://www.lambdanote.com/products/opendatastructures
// %words
// treap, map, set
// %=END DOC
// %=BEGIN CODE

template <class Key, class Value>
class TreapSet {
  struct Node {
    Key key;
    Value value;
    unique_ptr<Node> ch[2];
    unsigned pri;  // 大きいほど根に近い
    Node(const Key& k, unsigned p) : key(k), value(), pri(p) {}
  };

  unique_ptr<Node> root_;
  int size_;

  static mt19937& rng() {
    static mt19937 r(1341398u);  // デバッグのためシードは固定
    return r;
  }

  // base->ch[i] が base の位置に来るように回転する
  static void rotate_(unique_ptr<Node>& base, int i) {
    unique_ptr<Node> c = move(base->ch[i]);
    base->ch[i] = move(c->ch[i ^ 1]);
    c->ch[i ^ 1] = move(base);
    base = move(c);
  }

  static unique_ptr<Node> merge_(unique_ptr<Node> a, unique_ptr<Node> b) {
    if (!a)
      return b;
    if (!b)
      return a;
    if (a->pri > b->pri) {
      a->ch[1] = merge_(move(a->ch[1]), move(b));
      return a;
    }
    b->ch[0] = merge_(move(a), move(b->ch[0]));
    return b;
  }

  // k を持つノードを返す．無ければ作る
  Node* touch_(unique_ptr<Node>& node, const Key& k) {
    if (!node) {
      node.reset(new Node(k, rng()()));
      ++size_;
      return node.get();
    }
    if (!(k < node->key) && !(node->key < k))
      return node.get();
    int i = node->key < k ? 1 : 0;
    Node* res = touch_(node->ch[i], k);
    if (node->ch[i]->pri > node->pri)
      rotate_(node, i);
    return res;
  }

  Node* find_(const Key& k) const {
    Node* p = root_.get();
    while (p) {
      if (k < p->key)
        p = p->ch[0].get();
      else if (p->key < k)
        p = p->ch[1].get();
      else
        return p;
    }
    return nullptr;
  }

 public:
  TreapSet() : size_(0) {}
  TreapSet(const TreapSet&) = delete;
  TreapSet& operator=(const TreapSet&) = delete;
  TreapSet(TreapSet&& o) : root_(move(o.root_)), size_(o.size_) { o.size_ = 0; }
  TreapSet& operator=(TreapSet&& o) {
    if (this != &o) {
      root_ = move(o.root_);
      size_ = o.size_;
      o.size_ = 0;
    }
    return *this;
  }

  int size() const { return size_; }
  bool empty() const { return size_ == 0; }

  Value& operator[](const Key& k) { return touch_(root_, k)->value; }

  Value* find(const Key& k) {
    Node* p = find_(k);
    return p ? &p->value : nullptr;
  }
  const Value* find(const Key& k) const {
    const Node* p = find_(k);
    return p ? &p->value : nullptr;
  }

  bool erase(const Key& k) {
    unique_ptr<Node>* p = &root_;
    while (*p) {
      if (k < (*p)->key) {
        p = &(*p)->ch[0];
      } else if ((*p)->key < k) {
        p = &(*p)->ch[1];
      } else {
        unique_ptr<Node> old = move(*p);
        *p = merge_(move(old->ch[0]), move(old->ch[1]));
        --size_;
        return true;
      }
    }
    return false;
  }
};

// %=END CODE
#endif  // SRC_CPP_CONTAINER_BALANCING_TREAP_SET_HPP__
