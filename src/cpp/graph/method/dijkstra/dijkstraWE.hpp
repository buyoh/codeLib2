// %=BEGIN
// %title
// ダイクストラ法(無向・辺重み)
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
#include "src/cpp/graph/datastructure/graphe.hpp"
// ```
// %words
// dijkstra
// %verified
//
// %references
//
// %=END

vector<GraphE::W_T> dijkstraEdgeWeighted(const GraphE& graph, int startIndex) {
  using T = GraphE::W_T;
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
      auto e = graph.edges[ei];
      int u = e.to(v);
      if (d + e.value < dist[u]) {
        que.emplace(-(dist[u] = d + e.value), u);
      }
    }
  }
  return move(dist);
}
