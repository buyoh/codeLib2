#ifndef SRC_CPP_GRAPH_METHOD_DIJKSTRA_DIJKSTRAWV_HPP__
#define SRC_CPP_GRAPH_METHOD_DIJKSTRA_DIJKSTRAWV_HPP__
// %=BEGIN DOC
// %title
// ダイクストラ法(無向・頂点重み)
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
#include "src/cpp/graph/datastructure/graph.hpp"
// ```
// %words
// dijkstra
// %verified
//
// %references
//
// %=END DOC
// %=BEGIN CODE

template <typename T, typename VECTOR>
vector<T> dijkstraVertexWeighted(const Graph& graph, int startIndex, const VECTOR& weight) {
  vector<T> dist(graph.size(), numeric_limits<int>::max());

  priority_queue<pair<T, int>> que;  // <dist, idx>
  que.emplace(0, startIndex);
  dist[startIndex] = 0;

  while (!que.empty()) {
    T d = -que.top().first;
    int v = que.top().second;
    que.pop();
    if (dist[v] < d)
      continue;

    for (int u : graph.vertex_to[v])
      if (d + weight[u] < dist[u])
        que.emplace(-(dist[u] = d + weight[u]), u);
  }
  return move(dist);
}
// %=END CODE
#endif  // SRC_CPP_GRAPH_METHOD_DIJKSTRA_DIJKSTRAWV_HPP__
