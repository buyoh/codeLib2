#ifndef SRC_CPP_GRAPH_METHOD_DIJKSTRA_DIJKSTRA_HPP__
#define SRC_CPP_GRAPH_METHOD_DIJKSTRA_DIJKSTRA_HPP__
// %=BEGIN DOC
// %title
// ダイクストラ法(無向・重み無し)
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

vector<int> dijkstra(const Graph& graph, int startIndex) {
  vector<int> dist(graph.size(), numeric_limits<int>::max());

  queue<pair<int, int>> que;  // <dist, idx>
  que.emplace(0, startIndex);
  dist[startIndex] = 0;

  while (!que.empty()) {
    int d = que.front().first;
    int v = que.front().second;
    que.pop();
    if (dist[v] < d)
      continue;

    for (int u : graph.vertex_to[v])
      if (d + 1 < dist[u])
        que.emplace(dist[u] = d + 1, u);
  }
  return move(dist);
}
// %=END CODE
#endif  // SRC_CPP_GRAPH_METHOD_DIJKSTRA_DIJKSTRA_HPP__
