#ifndef SRC_CPP_GRAPH_METHOD_CONNECTEDCOMPONENT_SCC_HPP__
#define SRC_CPP_GRAPH_METHOD_CONNECTEDCOMPONENT_SCC_HPP__
// %=BEGIN
// %title
// 強連結成分分解
//
// %overview
// [TODO] validation
// よく使っているので問題ないとは思いますが…
//
// %usage
//
// %require
// ```
#include <vector>
#include <stack>
#include <functional>
#include "src/cpp/graph/datastructure/dgraph.hpp"
#include "src/cpp/graph/datastructure/unionfind.hpp"
// ```
//
// %verified
//
// %references
// %words
// scc
// %=END

int strongly_connected_components(const DGraph& graph, Unionfind& result) {
  stack<int> stk;
  vector<int> num(graph.size()), low(graph.size()), flg(graph.size());
  int cnt = 0;
  int n_components = graph.n;

  function<void(int)> dfs = [&](int idx) {
    low[idx] = num[idx] = ++cnt;
    stk.push(idx);
    flg[idx] = true;

    for (int to : graph.vertex_to[idx]) {
      if (num[to] == 0) {
        dfs(to);
        low[idx] = min(low[idx], low[to]);
      } else if (flg[to]) {  // ?
        low[idx] = min(low[idx], num[to]);
      }
    }
    if (low[idx] == num[idx]) {
      while (!stk.empty()) {
        int v = stk.top();
        stk.pop();
        flg[v] = false;
        if (idx == v)
          break;
        n_components -= result.connect(idx, v);
      }
    }
  };
  for (int i = 0; i < graph.size(); ++i) {
    if (num[i] == 0)
      dfs(i);
  }
  return n_components;
}

class SCComps {
 public:
  // sccが1頂点に纏められたグラフ
  DGraph sccg;
  // 元の頂点番号からscc後の頂点番号を求める
  vector<int> ori2scc;
  // scc後の頂点番号から元の頂点番号を求める
  vector<vector<int>> scc2ori;

  SCComps(const DGraph& g) : sccg(1), ori2scc(g.size(), -1) { build(g); }

  void build(const DGraph& orig) {
    Unionfind uf(orig.size());
    strongly_connected_components(orig, uf);

    int n = 0;
    for (int i = 0; i < orig.size(); ++i) {
      int r = uf.root(i);
      if (ori2scc[r] == -1) {
        ori2scc[r] = n++;
        scc2ori.emplace_back();
      }
      ori2scc[i] = ori2scc[r];
      scc2ori[ori2scc[i]].push_back(i);
    }
    sccg.resize(n);

    for (int i = 0; i < orig.size(); ++i) {
      for (int to : orig.vertex_to[i]) {
        if (ori2scc[i] == ori2scc[to])
          continue;
        sccg.connect(ori2scc[i], ori2scc[to]);
      }
    }
  }

  inline const vector<int>& vertex_to(int v) const { return sccg.vertex_to[v]; }
  inline const vector<int>& vertex_from(int v) const { return sccg.vertex_from[v]; }
  inline int size() const { return sccg.n; }
};
#endif  // SRC_CPP_GRAPH_METHOD_CONNECTEDCOMPONENT_SCC_HPP__
