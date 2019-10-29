// %title
// bellmanFord
//
// %overview
// bellmanFord法。
// 頂点数と辺情報を引数とする。
// ループする場合、空のvectorを返す。
//
// %usage
//
// %require
// ```
#include <vector>
#include <limits>
using namespace std;
#include "src/cpp/graph/datastructure/dgraphe.cpp"
// ```
// %verified
// https://atcoder.jp/contests/abc137/submissions/6908127
//
// %references
// %words
// bellmanford

vector<DGraphE::W_T> bellmanFord(int n, const vector<DGraphE::Arc>& arcs, int start = 0) {
  using T = DGraphE::W_T;
  vector<T> dist(n, numeric_limits<T>::min() / 2);
  dist[start] = 0;
  bool running = true;
  while (running && --n >= 0) {
    running = false;
    for (auto& arc : arcs) {
      if (dist[arc.to] < dist[arc.from] + arc.value) {
        dist[arc.to] = dist[arc.from] + arc.value;
        running = true;
        if (n <= 0)
          return vector<T>();
      }
    }
  }
  return dist;
}
