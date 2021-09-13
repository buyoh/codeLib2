#ifndef SRC_CPP_GRAPH_METHOD_ARTICULATION_HPP__
#define SRC_CPP_GRAPH_METHOD_ARTICULATION_HPP__
// %=BEGIN DOC
// %title
// 関節点検出
//
// %overview
// [TODO] validation
//
// %usage
// detect_articulation_point(const GraphE& graph, vector<int>& out)
// out : out[頂点番号] = 関節点 ? 1 : 0
//
// %require
// ```
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;
#include "src/cpp/graph/datastructure/graph.hpp"
// ```
// %verified
//
// %references
// %words
// articulation
// %tag
// unverified
// %=END DOC
// %=BEGIN CODE

vector<int> detectArticulationPoint(const Graph& graph) {
  vector<int> out(graph.n, 0);
  vector<int> visited(graph.n);

  int cnt = 1;
  function<int(int)> dfs = [&](int idx) {
    // printf("%d: called %d\n",idx,visited[idx]);
    int p = visited[idx];
    for (int to : graph.vertex_to[idx]) {
      if (visited[to]) {
        p = min(p, visited[to]);
      } else {
        visited[to] = ++cnt;
        int q = dfs(to);
        if (visited[idx] == q)
          out[idx] = 1;
        p = min(p, q);
      }
    }
    // printf("%d: return %d\n",idx,p);
    return p;
  };
  for (int root = 0; root < graph.n; ++root) {
    if (visited[root])
      continue;
    visited[root] = 1;
    int start_degree = 0;
    for (int to : graph.vertex_to[root]) {
      if (!visited[to]) {
        ++start_degree;
        visited[to] = ++cnt;
        dfs(to);
      }
    }
    if (2 <= start_degree)
      out[root] = 1;
  }
  return out;
}
// %=END CODE
#endif  // SRC_CPP_GRAPH_METHOD_ARTICULATION_HPP__
