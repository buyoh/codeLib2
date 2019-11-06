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
//
// 区間クエリは書けそう．
// 例えば『キーがx以下のNodeにyを加算する』は，treap[x]とtreap[x].childlen[0]に対してLazy実装で実現できる．
//
// %usage
// void Treap::concat(Treap& another)
// ; thisの末尾にanotherを結合して，anotherを空にする．
// Treap Treap::split(int size)
// ; thisの前からsize個を切り出す．
// void Treap::insert(int index, value_t val)
// ; index番目の要素の手前にvalを挿入する．
// void Treap::erase(int index)
// ; index番目の要素を削除する．
// unique_ptr<TreapNode>& Treap::find(int index)
// ; index番目の要素のスマートポインタの参照を得る．
// ; index番目の要素が存在しないなら，空のスマートポインタの参照を得る．
// value_t& Treap::operator[](int index)
// ; index番目の要素に対応するvalの参照を得る．無いなら爆発する．
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

struct TreapNode {
  typedef long long value_t;
  typedef mt19937_64 randomizer_t;
  static const value_t init_value_ = 0;
  static randomizer_t rnd;

  unique_ptr<TreapNode> childlen[2];
  value_t value;
  randomizer_t::result_type priority;
  int n_node;

  TreapNode(value_t v = init_value_) : value(v), priority(rnd() | 1), n_node(1) {}
  TreapNode(value_t v, randomizer_t::result_type p) : value(v), priority(p), n_node(1) {}

  // 子の操作前に必ず実行
  // todo: 十分に呼ばれているかどうかvalidate
  inline void update_before() {}

  // 子の操作後に必ず実行
  inline void update_after() {
    n_node = 1;
    if (childlen[0])
      n_node += childlen[0]->n_node;
    if (childlen[1])
      n_node += childlen[1]->n_node;
  }

  // inline Node& operator[](size_t i) { return *childlen[i & 1]; }
  // inline bool has(size_t i) { return (bool)childlen[i & 1]; }
};
TreapNode::randomizer_t TreapNode::rnd = TreapNode::randomizer_t();

class Treap : public unique_ptr<TreapNode> {
 public:
  using value_t = TreapNode::value_t;
  static const value_t init_value_ = TreapNode::init_value_;

  Treap() {}
  Treap(value_t val) : unique_ptr<TreapNode>(new TreapNode(val)) {}
  Treap(unique_ptr<TreapNode>& p) { swap(p); }

  static void _concat(unique_ptr<TreapNode>& treap1, unique_ptr<TreapNode>& treap2) {
    if (!treap2)
      return;
    if (!treap1) {
      treap1.swap(treap2);
      return;
    }
    if (treap1->priority < treap2->priority) {
      treap1->update_before();
      if (!treap1->childlen[1])
        treap1->childlen[1] = move(treap2);
      else
        _concat(treap1->childlen[1], treap2);
    } else {
      treap2->update_before();
      if (!treap2->childlen[0]) {
        treap2->childlen[0] = move(treap1);
        treap1.swap(treap2);
      } else {
        treap1.swap(treap2->childlen[0]);
        _concat(treap2->childlen[0], treap1);
        treap1.swap(treap2);
      }
    }
    treap1->update_after();
  }

  // anotherをthisの後ろにmergeする
  inline void concat(Treap& another) { _concat(*this, another); }
  inline void concat(Treap&& another) {
    auto a = move(another);
    _concat(*this, a);
  }

  // TODO: あまりにも下手
  static unique_ptr<TreapNode> _split(unique_ptr<TreapNode>& node, int size) {
    if (size <= 0)
      return unique_ptr<TreapNode>();
    if (!node->childlen[0]) {
      if (!node->childlen[1])
        return move(node);
      node->update_before();
      unique_ptr<TreapNode> cutted = move(node);
      node = move(cutted->childlen[1]);
      cutted->childlen[1] = move(_split(node, size - 1));
      cutted->update_after();
      node->update_after();
      return cutted;
    }
    if (node->childlen[0]->n_node > size) {
      unique_ptr<TreapNode> res = _split(node->childlen[0], size);
      node->update_after();
      return res;
    } else if (node->childlen[0]->n_node < size) {
      node->update_before();
      unique_ptr<TreapNode> cutted = move(node);
      node = move(cutted->childlen[1]);
      cutted->childlen[1] = move(_split(node, size - cutted->childlen[0]->n_node - 1));
      cutted->update_after();
      node->update_after();
      return cutted;
    } else {
      unique_ptr<TreapNode> res = move(node->childlen[0]);
      node->update_after();
      return res;
    }
  }

  // 左からsize個切り取る
  inline Treap split(int size) {
    if (!(*this))
      return Treap();
    if ((*this)->n_node <= size) {
      return move(*this);
    }
    unique_ptr<TreapNode> res = _split(*this, size);
    (*this)->update_after();
    return res;
  }

  static unique_ptr<TreapNode>& _find(unique_ptr<TreapNode>& node, int index) {
    if (!node)
      return node;
    node->update_before();
    if (!node->childlen[0])
      return 0 < index ? _find(node->childlen[1], index - 1)
                       : index == 0 ? node : node->childlen[0];
    if (node->childlen[0]->n_node < index)
      return _find(node->childlen[1], index - node->childlen[0]->n_node - 1);
    if (node->childlen[0]->n_node > index)
      return _find(node->childlen[0], index);
    return node;
  }

  // 左からi番目のnodeを削除する
  // 存在しないなら空のunique_ptrの参照が返る
  inline unique_ptr<TreapNode>& find(int index) { return _find(*this, index); }
  // 安全でない
  inline value_t& operator[](int index) { return find(index)->value; }

  // 左からidx番目のnodeを削除する
  inline void erase(int index) {
    if (index < 0 && (*this)->n_node <= index)
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

  // size要素から成るvalで初期化されたTreapを生成する
  // 空でないTreapでgenerateを呼び出したら何もしない．
  // 乱数生成がボトルネックで，O(NlogN)時間
  void generate(int size, value_t val = init_value_) {
    if (size <= 0 || *this)
      return;
    vector<TreapNode::randomizer_t::result_type> rr(size);
    for (auto& x : rr)
      x = TreapNode::rnd();
    sort(rr.begin(), rr.end());

    function<void(unique_ptr<TreapNode>&, int)> dfs = [&size, &rr, &val, &dfs](
                                                          unique_ptr<TreapNode>& node, int idx1) {
      if (idx1 > size)
        return;
      node.reset(new TreapNode(val, rr[idx1 - 1]));
      node->update_before();
      dfs(node->childlen[0], idx1 * 2);
      dfs(node->childlen[1], idx1 * 2 + 1);
      node->update_after();
    };
    dfs(*this, 1);
  }

  // void print_tour(unique_ptr<TreapNode>& node) {
  //   if (!node) {
  //     cout << "NIL ";
  //     return;
  //   }
  //   cout << "L[" << node->value << "] ";
  //   print_tour(node->childlen[0]);
  //   cout << "C[" << node->value << "] ";
  //   print_tour(node->childlen[1]);
  //   cout << "R[" << node->value << "] ";
  // }
  // inline void print_tour() { print_tour(*this); }
};
