#ifndef SRC_CPP_CONTAINER_BALANCING_TREAP_SEGMENT_HPP__
#define SRC_CPP_CONTAINER_BALANCING_TREAP_SEGMENT_HPP__
// %=BEGIN DOC
// %title
// Treap(segment)(plain)
//
// %overview
// Treapとは，次を満たす確率的平衝二分探索木である．
// - keyについてみると，木は二分探索木になっている．
// - priority(randomized)についてみると，木はHeapになっている．
//
// 実装したTreapは，配列っぽいインターフェースを持つ．split/mergeベースで記述．
//
// 遅延評価を想定するので、getter には const 修飾は使われていない。
//
// 区間クエリは書けそう．
// 例えば『キーがx以下のNodeにyを加算する』は，treap[x]とtreap[x].childlen[0]
// に対してLazy実装で実現できる．
//
// 実装ノート:
// Treap の平衡化実装と遅延評価等の区間演算実装を分離したい…
// 遅延評価等の実装が一切無いクラスを TreapPlain として、Treap が TreapPlain を継承して
// 機能追加したクラスとしたい。NodeFactory とか必要そう。vtable がパフォーマンスに影響する
// 問題もある。
// %usage
// void Treap::create(int size, T val)
// ; size要素から成るvalで初期化されたTreapを生成する
// void Treap::concat(Treap& another)
// ; thisの末尾にanotherを結合して，anotherを空にする．
// Treap Treap::split(int size)
// ; thisの前からsize個を切り出す．
// void Treap::insert(int index, value_t val)
// ; index番目の要素の手前にvalを挿入する．
// void Treap::erase(int index)
// ; index番目の要素を削除する．
// value_t& Treap::operator[](int index)
// ; index番目の要素に対応するvalの参照を得る．無いなら爆発する．
//
// %require
// ```
#include <ostream>
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
// %=END DOC
// %=BEGIN CODE

class Treap {
 public:
  using value_t = long long;
  using rng_t = mt19937_64;
  static rng_t rnd;

  struct NodeData {
    value_t value;
    NodeData(value_t val) : value(val) {}
    inline void updateBefore() {}
    inline void updateAfter() {}
  };

 protected:
  struct Node {
    unique_ptr<Node> childlen[2];
    NodeData data;
    rng_t::result_type priority;
    int n_node;

    Node(value_t v, rng_t::result_type p) : data(v), priority(p), n_node(1) {}

    inline void updateBefore() { data.updateBefore(); }
    inline void updateAfter() {
      // recalculate number of nodes
      n_node = 1;
      if (childlen[0])
        n_node += childlen[0]->n_node;
      if (childlen[1])
        n_node += childlen[1]->n_node;
      data.updateAfter();
    }
  };

  static unique_ptr<Node> createNode(value_t val) {
    // NOTE: 乱数の1ビットを潰す理由を忘却
    return unique_ptr<Node>(new Node(val, rnd() | 1));
  }

  unique_ptr<Node> root_;

 public:
  Treap() {}
  Treap(value_t val) : root_(createNode(val)) {}
  Treap(Treap&& p) : root_(move(p.root_)) {}
  void swap(Treap& p) { std::swap(root_, p.root_); }
  int size() const { return root_ ? root_->n_node : 0; }

 private:
  Treap(unique_ptr<Node>&& p) : root_(move(p)) {}

  static void _concat(unique_ptr<Node>& treap1, unique_ptr<Node>& treap2) {
    if (!treap2)
      return;
    if (!treap1) {
      treap1.swap(treap2);
      return;
    }
    if (treap1->priority < treap2->priority) {
      treap1->updateBefore();
      if (!treap1->childlen[1])
        treap1->childlen[1] = move(treap2);
      else
        _concat(treap1->childlen[1], treap2);
    } else {
      treap2->updateBefore();
      if (!treap2->childlen[0]) {
        treap2->childlen[0] = move(treap1);
        treap1.swap(treap2);
      } else {
        treap1.swap(treap2->childlen[0]);
        _concat(treap2->childlen[0], treap1);
        treap1.swap(treap2);
      }
    }
    treap1->updateAfter();
  }

  // TODO: refactoring?
  static unique_ptr<Node> _split(unique_ptr<Node>& node, int size) {
    if (size <= 0)
      return unique_ptr<Node>();
    if (!node->childlen[0]) {
      if (!node->childlen[1])
        return move(node);
      node->updateBefore();
      unique_ptr<Node> cutted = move(node);
      node = move(cutted->childlen[1]);
      cutted->childlen[1] = move(_split(node, size - 1));
      cutted->updateAfter();
      node->updateAfter();
      return cutted;
    }
    if (node->childlen[0]->n_node > size) {
      unique_ptr<Node> res = _split(node->childlen[0], size);
      node->updateAfter();
      return res;
    } else if (node->childlen[0]->n_node < size) {
      node->updateBefore();
      unique_ptr<Node> cutted = move(node);
      node = move(cutted->childlen[1]);
      cutted->childlen[1] = move(_split(node, size - cutted->childlen[0]->n_node - 1));
      cutted->updateAfter();
      node->updateAfter();
      return cutted;
    } else {
      unique_ptr<Node> res = move(node->childlen[0]);
      node->updateAfter();
      return res;
    }
  }

  static unique_ptr<Node>& _find(unique_ptr<Node>& node, int index) {
    if (!node)
      return node;
    node->updateBefore();
    if (!node->childlen[0])
      return 0 < index    ? _find(node->childlen[1], index - 1)
             : index == 0 ? node
                          : node->childlen[0];
    if (node->childlen[0]->n_node < index)
      return _find(node->childlen[1], index - node->childlen[0]->n_node - 1);
    if (node->childlen[0]->n_node > index)
      return _find(node->childlen[0], index);
    return node;
  }

 public:
  // anotherをthisの後ろにmergeする
  inline void concat(Treap& another) { _concat(root_, another.root_); }
  inline void concat(Treap&& another) { _concat(root_, another.root_); }

  // 左からsize個切り取る
  inline Treap split(int size) {
    if (!root_)
      return Treap();
    if (root_->n_node <= size) {
      return move(root_);
    }
    unique_ptr<Node> res = _split(root_, size);
    root_->updateAfter();
    return res;
  }

  // 左からi番目のnodeを取得する
  // 存在しないなら空のunique_ptrの参照が返る
  // inline unique_ptr<Node>& find(int index) { return _find(root_, index); }
  // 安全でない
  inline value_t& operator[](int index) { return _find(root_, index)->data.value; }

  // 左からidx番目のnodeを削除する
  inline void erase(int index) {
    if (index < 0 && root_->n_node <= index)
      return;
    Treap tmp = split(index);
    split(1);
    swap(tmp);
    concat(tmp);
  }

  // i番目の要素の前にvalを挿入する
  inline void insert(int index, value_t val) {
    Treap tmp = split(index);
    tmp.concat(Treap(val));
    tmp.concat(*this);
    swap(tmp);
  }

 private:
  static unique_ptr<Node> create_dfs(int req_size,
                                     const value_t& val,
                                     const vector<rng_t::result_type>& rr,
                                     int idx1) {
    if (idx1 > req_size)
      return unique_ptr<Node>();
    unique_ptr<Node> node(new Node(val, rr[idx1 - 1]));
    node->updateBefore();
    node->childlen[0] = create_dfs(req_size, val, rr, idx1 * 2);
    node->childlen[1] = create_dfs(req_size, val, rr, idx1 * 2 + 1);
    node->updateAfter();
    return node;
  }

  // size要素から成るvalで初期化されたTreapを生成する
  // 乱数生成時のソートがボトルネックで，O(NlogN)時間
 public:
  static Treap create(int size, value_t val) {
    vector<rng_t::result_type> rr(size);
    for (auto& x : rr)
      x = rnd();
    sort(rr.begin(), rr.end());

    return create_dfs(size, val, rr, 1);
  }

 private:
  void printTour_dfs(ostream& o, unique_ptr<Node>& node) {
    if (!node) {
      o << "NIL ";
      return;
    }
    o << "L[" << node->data.value << "] ";
    printTour_dfs(o, node->childlen[0]);
    o << "C[" << node->data.value << "] ";
    printTour_dfs(o, node->childlen[1]);
    o << "R[" << node->data.value << "] ";
  }

 public:
  inline void printTour(ostream& o) { printTour_dfs(o, root_); }

 private:
  void toVector_dfs(unique_ptr<Node>& node, vector<value_t>& out) {
    if (!node)
      return;
    toVector_dfs(node->childlen[0], out);
    out.push_back(node->data.value);
    toVector_dfs(node->childlen[1], out);
  }

 public:
  inline vector<value_t> toVector() {
    vector<value_t> out;
    toVector_dfs(root_, out);
    return out;
  }

  //

  void setValue(int index, value_t value) {
    auto& node = _find(root_, index);
    if (!node)
      return;
    // TODO: updateAfter is not fired
    node->data.value = value;
  }
};
Treap::rng_t Treap::rnd = Treap::rng_t();

// %=END CODE
#endif  // SRC_CPP_CONTAINER_BALANCING_TREAP_SEGMENT_HPP__
