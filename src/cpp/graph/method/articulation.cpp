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
#include "src/cpp/graph/datastructure/graph.cpp"
// ```
// %verified
//
// %references
// %words
// articulation
// %tag
// unverified

void detect_articulation_point(const Graph& graph, vector<int>& out) {
  out.resize(graph.n);
  fill(out.begin(), out.end(), 0);
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
  visited[0] = 1;
  int start_degree = 0;
  for (int to : graph.vertex_to[0]) {
    if (!visited[to]) {
      ++start_degree;
      visited[to] = ++cnt;
      dfs(to);
    }
  }
  if (2 <= start_degree)
    out[0] = 1;
}