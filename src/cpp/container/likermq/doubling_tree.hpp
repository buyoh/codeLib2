#ifndef SRC_CPP_CONTAINER_LIKERMQ_DOUBLING_TREE_HPP__
#define SRC_CPP_CONTAINER_LIKERMQ_DOUBLING_TREE_HPP__
// %=BEGIN DOC
// %title
// Doubling (根付き木)
//
// %overview
// 根付き木上でダブリングをする
// 例えば、ある頂点から根に向かって k 頂点の要素の総和が計算できる
// 真面目に汎化すれば、出次数が高々1のグラフに対しても出来るはずので拡張したい
//
// %usage
//
// %require
// ```
#include <vector>
using namespace std;
#include "src/cpp/graph/datastructure/graph.hpp"
// ```
// %verified
// https://atcoder.jp/contests/abc235/submissions/28783483
//
// %references
//
// %=END DOC
// %=BEGIN CODE

template <typename Trait>
class DoublingRootedTree {
  Trait trait;
  using T = typename Trait::T;

  int n_, m_;
  // データ構造のダブリング
  vector<T> dp_;
  // 親要素へのダブリング
  vector<int> parent_db_;

  inline T& dpAt(int i, int h) { return dp_[i * m_ + h]; }
  inline T& parentAt(int i, int h) { return parent_db_[i * m_ + h]; }

  void buildTableVertex(const vector<int>& stk) {
    const int n = stk.size();  // TODO:
    // set dp_;
    for (int k = 1; k < m_; ++k) {
      int h = 1 << (k - 1);
      if (h < n) {
        dpAt(stk[n - 1], k) = trait.mul(dpAt(stk[n - 1], k - 1), dpAt(stk[n - 1 - h], k - 1));
      }
    }
    // set parent_db_
    for (int k = 0; k < m_; ++k) {
      int h = 1 << k;
      if (h < n) {
        parentAt(stk[n - 1], k) = stk[n - 1 - h];
      }
    }
  }
  void buildTable(const Graph& g, vector<int>& stk, int vtx, int from) {
    stk.push_back(vtx);
    buildTableVertex(stk);
    for (auto to : g.vertex_to[vtx]) {
      if (to == from)
        continue;
      buildTable(g, stk, to, vtx);
    }
    stk.pop_back();
  }
  void buildTable(const Graph& g, int root) {
    vector<int> stk;
    buildTable(g, stk, root, -1);
  }

  void init() {
    dp_.resize(n_ * m_, trait.elem());
    parent_db_.resize(n_ * m_);
  }

 public:
  DoublingRootedTree(int n) : n_(n) {
    for (m_ = 0; n_ > (1 << m_); ++m_)
      ;
    --m_;
    if (m_ <= 0)
      m_ = 1;
  }

  void build(const Graph& g, int root = 0) {
    init();
    buildTable(g, root);
  }

  template <typename IT>
  void build(const Graph& g, IT begin, IT end, int root = 0) {
    init();
    IT it = begin;
    for (int i = 0; i < n_ && it != end; ++i, ++it) {
      dpAt(i, 0) = *it;
    }
    buildTable(g, root);
  }

  template <typename F>
  void build(const Graph& g, F init, int root = 0) {
    init();
    for (int i = 0; i < n_; ++i) {
      dpAt(i, 0) = init(i);
    }
    buildTable(g, root);
  }

  // p1 から親方向へ d 個の要素の総和を取る。
  T query(int p1, int d) {
    T s = trait.elem();
    // for (int k = m_ - 1; 0 <= k; --k) {
    //   int h = 1 << k;
    //   while (p1 + h <= p2) {
    //     s = trait.mul(s, dpAt(p1, k));
    //     p1 += h;
    //   }
    // }
    return s;
  }
};

struct DoublingTrait {
  using T = long long;
  // 元を返す
  T elem() { return 0; }
  T mul(T s, T t) { return s + t; }
};

//

//
// %=END CODE
#endif  // SRC_CPP_CONTAINER_LIKERMQ_DOUBLING_TREE_HPP__