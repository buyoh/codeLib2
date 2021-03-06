#ifndef SRC_CPP_GRAPH_METHOD_DISTANCE_SPFA_HPP__
#define SRC_CPP_GRAPH_METHOD_DISTANCE_SPFA_HPP__
// %=BEGIN DOC
// %title
// SPFA
//
// %overview
// shortest path faster algorithm
// 負閉路検出可能・負辺動作可能。
// ベルマンフォードより速いらしい。
// ただし閉路検出が遅いような気がする。
//
// %usage
// vector<DGraphE::W_T> shortestPathFasterAlgorithm(const DGraphE& graph, int start = 0, bool*
// detectNegativeCycle = nullptr) detectNegativeCycle = nullptrと置くと負閉路検出を無効化する
// %require
// ```
#include <vector>
#include <queue>
#include <limits>
using namespace std;
#include "src/cpp/graph/datastructure/dgraphe.hpp"
// ```
// %verified
// https://atcoder.jp/contests/abc137/submissions/7173578
//
// %references
// http://hogloid.hatenablog.com/entry/20120409/1333973448
// https://tubo28.me/compprog/algorithm/spfa/
//
// %=END DOC
// %=BEGIN CODE

vector<DGraphE::W_T> shortestPathFasterAlgorithm(const DGraphE& graph,
                                                 int start = 0,
                                                 bool* detectNegativeCycle = nullptr) {
  using T = DGraphE::W_T;
  const int n = graph.n;
  vector<bool> incl(n);
  vector<T> dist(n, numeric_limits<T>::max() / 2);
  vector<int> counter;
  if (detectNegativeCycle)
    *detectNegativeCycle = false, counter.resize(n);

  queue<int> que;
  que.push(start);
  incl[start] = true;
  dist[start] = 0;
  while (!que.empty()) {
    int v = que.front();
    que.pop();
    incl[v] = false;
    for (auto ai : graph.vertex_to[v]) {
      auto a = graph.arcs[ai];

      if (dist[a.to] <= dist[v] + a.value)
        continue;
      dist[a.to] = dist[v] + a.value;

      if (incl[a.to])
        continue;
      incl[a.to] = true;
      que.push(a.to);

      if (detectNegativeCycle && ++counter[a.to] >= n) {
        *detectNegativeCycle = true;
        return move(dist);
      }
    }
  }
  return move(dist);
}
// %=END CODE
#endif  // SRC_CPP_GRAPH_METHOD_DISTANCE_SPFA_HPP__
