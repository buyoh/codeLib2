// %title
// Treap(set)
//
// %overview
// Treapとは，次を満たす確率的平衝二分探索木である．
// - keyについてみると，木は二分探索木になっている．
// - priority(randomized)についてみると，木はHeapになっている．
//
// 実装したTreapは，map(set)のような機能を持つだけ．
// split/mergeも出来ない．
//
//
// keyはユニーク＆これ以上Nodeを追加しないという条件を加えれば区間クエリは書けそう．
// 例えば『キーがx以下のNodeにyを加算する』は，treap[x]とtreap[x].childlen[0]に対してLazy実装で実現できる．
//
// %usage
// value_t& Treap::operator[](key_t key)
// ; keyに対応するvalの参照を得る．無いなら作る．
// unique_ptr<TreapNode>& Treap::find(key_t key)
// ; keyに対応するvalのスマートポインタの参照を得る．
// ; keyに対応するvalが存在しないなら，空のスマートポインタの参照を得る．
// void Treap::erase(key_t key)
// ; keyを削除する．
//
// %require
// ```
#include <memory>
#include <algorithm>
#include <random>
#include <functional>
using namespace std;
// ```
// %verified
//
// %references
// https://www.slideshare.net/iwiwi/2-12188757
// http://www.prefield.com/algorithm/container/treap.html

class Treap {
 public:
  using key_type = int;
  using value_type = long long;

 private:
  using randevice_type = mt19937;

  static randevice_type rnd;

 private:
  struct Node {
    key_type key;
    value_type value;
    unique_ptr<Node> childlen[2];
    randevice_type::result_type priority;

    Node(const key_type& k = key_type(), const value_type& v = value_type()) : key(k), value(v), priority(rnd() | 1) {}
    Node(key_type k, value_type v, randevice_type::result_type p) : key(k), value(v), priority(p) {}

    // inline Node& operator[](size_t i) { return *childlen[i & 1]; }
    // inline bool has(size_t i) { return (bool)childlen[i & 1]; }
  };

 private:
  unique_ptr<Node> root;

 public:
  Treap() {}
  Treap(Node* p) : root(p) {}

 private:
  // base.childlen[swap_for]がbaseの位置に来るように回転させる．
  static void _rotate(unique_ptr<Node>& base, size_t swap_for) {
    swap_for &= 1;
    unique_ptr<Node> swf = move(base->childlen[swap_for]);
    base->childlen[swap_for] = move(swf->childlen[swap_for ^ 1]);
    swf->childlen[swap_for ^ 1] = move(base);
    base = move(swf);
  }

  // thisをrootとする部分木にkをkeyとするNodeが無ければ作る，あれば返す．
  static inline Node& _touch(unique_ptr<Node>& node, key_type k) {
    if (k == node->key)
      return *node;
    size_t i = node->key < k;
    if (node->childlen[i]) {
      auto& r = _touch(node->childlen[i], k);
      if (node->priority > node->childlen[i]->priority)
        _rotate(node, i);
      return r;
    } else {
      node->childlen[i].reset(new Node(k));
      return *(node->childlen[i]);
    }
  }

  // keyが等しいnodeを探す(const)
  static const unique_ptr<Node>& _find(const unique_ptr<Node>& ptr, key_type key) {
    return (!ptr || ptr->key == key) ? ptr : _find(ptr->childlen[ptr->key < key], key);
  }
  // keyが等しいnodeを探す
  static unique_ptr<Node>& _find(unique_ptr<Node>& ptr, key_type key) { return (!ptr || ptr->key == key) ? ptr : _find(ptr->childlen[ptr->key < key], key); }

  // ptrを削除する．
  static void _erase(unique_ptr<Node>& ptr) {
    if (!ptr->childlen[0] && !ptr->childlen[1]) {
      ptr.release();
    } else {
      bool i = ptr->childlen[0] ? 0 : ptr->childlen[1] ? 1 : ptr->childlen[0]->priority > ptr->childlen[1]->priority;
      _rotate(ptr, i);
      _erase(ptr->childlen[i ^ 1]);
    }
  }

 public:
  // map[]と同じ
  value_type& operator[](key_type key) {
    if (!(root)) {
      root.reset(new Node());
      return root->value;
    } else {
      return _touch(root, key).value;
    }
  }

  // keyが等しいnodeを探す
  // 存在しないなら空のunique_ptrの参照が返る
  inline const unique_ptr<Node>& find(key_type key) const { return _find(root, key); }

  // keyを持つnodeを削除する
  inline void erase(key_type key) {
    unique_ptr<Node>& node = _find(root, key);
    if (node)
      _erase(node);
  }
};
Treap::randevice_type Treap::rnd = randevice_type();
