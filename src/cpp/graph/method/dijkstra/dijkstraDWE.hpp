#ifndef SRC_CPP_GRAPH_METHOD_DIJKSTRA_DIJKSTRADWE_HPP__
#define SRC_CPP_GRAPH_METHOD_DIJKSTRA_DIJKSTRADWE_HPP__
// %=BEGIN DOC
// %title
// ダイクストラ法(有向・辺重み)
//
// %overview
//
// %usage
//
// %require
// ```
#include <vector>
#include <queue>
#include <limits>
using namespace std;
#include "src/cpp/graph/datastructure/dgraphe.hpp"
// ```
// %words
// dijkstra
// %verified
//
// %references
//
// %=END DOC
// %=BEGIN CODE

vector<DGraphE::W_T> dijkstraDEdgeWeighted(const DGraphE& graph, int startIndex) {
  using T = DGraphE::W_T;
  vector<T> dist(graph.size(), numeric_limits<T>::max());

  priority_queue<pair<T, int>> que;  // <dist, idx>
  que.emplace(0, startIndex);
  dist[startIndex] = 0;

  while (!que.empty()) {
    T d = -que.top().first;
    int v = que.top().second;
    que.pop();
    if (dist[v] < d)
      continue;

    for (int ei : graph.vertex_to[v]) {
      auto e = graph.arcs[ei];
      int u = e.to;
      if (d + e.value < dist[u]) {
        que.emplace(-(dist[u] = d + e.value), u);
      }
    }
  }
  return dist;
}
// %=END CODE
#endif  // SRC_CPP_GRAPH_METHOD_DIJKSTRA_DIJKSTRADWE_HPP__
