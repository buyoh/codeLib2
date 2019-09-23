// %title
// 橋検出
//
// %overview
//
// %usage
// detect_bridge(const GraphE& graph, vector<int>& out)
// out : out[辺番号] = 橋 ? 1 : 0
// 
// %require
// ```
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;
#include "src/cpp/graph/datastructure/graphe.cpp"
// ```
// %verified
// contests/abc075/submissions/1688087
//
// %references
// %words
// bridge

// 橋を探す
//
void detect_bridge(const GraphE& graph, vector<int>& out) {
  out.resize(graph.edges.size());
  fill(out.begin(), out.end(), 0);
  vector<int> visited(graph.n);

  int cnt = 1;
  function<int(int, int)> dfs = [&](int idx, int from) {
    // printf("%d: called %d\n",idx,visited[idx]);
    int p = visited[idx];
    for (int ei : graph.vertex_to[idx]) {
      if (ei == from)
        continue;
      auto& edge = graph.edges[ei];
      int to = edge.to(idx);
      if (visited[to]) {
        p = min(p, visited[to]);
      } else {
        visited[to] = ++cnt;
        int q = dfs(to, ei);
        if (visited[idx] < q)
          out[ei] = 1;
        p = min(p, q);
      }
    }
    // printf("%d: return %d\n",idx,p);
    return p;
  };
  visited[0] = 1;
  dfs(0, -1);
}
