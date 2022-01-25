#ifndef SRC_CPP_GRAPH_METHOD_ROOTED_TREE_HPP__
#define SRC_CPP_GRAPH_METHOD_ROOTED_TREE_HPP__
// %=BEGIN DOC
// %title
// 根付き木解析
//
// %overview
// 根からの距離を計算する
// 機能が単純すぎるので、高機能にしたい
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
//
// %references
// %words
//
// %tag
// unverified
// %=END DOC
// %=BEGIN CODE

class RootedTree {
  struct Node {
    int depth = 0;
  };
  vector<Node> node_;

  void dfs(const Graph& g, int v, int from) {
    for (auto to : g.vertex_to[v]) {
      if (to == from)
        continue;
      node_[to].depth = node_[v].depth + 1;
      dfs(g, to, v);
    }
  }

 public:
  RootedTree() {}
  void build(const Graph& g) {
    node_.resize(g.size());
    node_[0].depth = 0;
    dfs(g, 0, -1);
  }
  const Node& node(int v) const { return node_[v]; }
};
#endif  // SRC_CPP_GRAPH_METHOD_ROOTED_TREE_HPP__
