// %title
// オイラー路検出(テスト用)
//
// %overview
//
// %usage
//
// %require
// ```
#include "src/cpp/implements/compe.cpp"
#include "src/cpp/graph/datastructure/dgraphe.cpp"
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;
// ```
// %verified
//
// %references
//
// %words
// euler
// %tag
// wiip

DGraphE gen_random_eulerialTrail(int N, int M) {
  vector<pair<int, int>> route;
  route.reserve(M);
  int la = 0;
  repeat(i, M - 1) {
    int e = rand(0, N - 1);
    route.emplace_back(la, e);
    la = e;
  }
  shuffle(route.begin(), route.end(), randdev);

  vector<int> shuffler(N);
  iota(shuffler.begin(), shuffler.end(), 0);
  shuffle(shuffler.begin(), shuffler.end(), randdev);

  DGraphE graph(N);
  for (auto p : route)
    graph.connect(shuffler[p.first], shuffler[p.second]);

  return graph;
}

bool check_eulerialTrail(const DGraphE& graph, vector<int> trail) {
  vector<int> chkvertex(graph.size());
  vector<int> chkedge(graph.arcs.size());

  int v = trail[0];

  for (int i = 1; i < trail.size(); ++i) {
    int t = trail[i];
    if (i % 2 == 1) {
      auto e = graph.arcs[t];
      if (chkedge[t])
        return false;
      if (e.from != v)
        return false;
      v = e.to;
    } else {
      if (v != t)
        return false;
    }
  }
  return true;
}