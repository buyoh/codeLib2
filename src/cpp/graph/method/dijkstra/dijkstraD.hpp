// %=BEGIN
// %title
// ダイクストラ法(有向・重み無し)
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
#include "src/cpp/graph/datastructure/dgraph.hpp"
// ```
// %words
// dijkstra
// %verified
// https://atcoder.jp/contests/abc132/submissions/6175759
// %references
//
// %=END

vector<int> dijkstraD(const DGraph& graph, int startIndex) {
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