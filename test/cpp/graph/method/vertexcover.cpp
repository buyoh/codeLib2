#include "test/common/testutil.hpp"
#include "test/common/graph_generator.hpp"
#include "src/cpp/graph/method/vertexcover.cpp"
#include "src/cpp/graph/method/independentset.cpp"

// mode = 1 : vertex cover
// mode = 2 : independent set
bool satisfiedVCID(const Graph& g, const vector<int>& choice, int mode) {
  int n = g.size();
  bitset<30> cb;
  for (int c : choice)
    cb[c] = true;
  repeat(i, n) {
    for (auto v : g.vertex_to[i]) {
      if (mode == 1 && !cb[i] && !cb[v])
        return false;
      if (mode == 2 && cb[i] && cb[v])
        return false;
    }
  }
  return true;
}

int main() {
  const int N = 5;
  GraphGenerator::connectedGraph<Graph>(N, [](Graph g) {
    auto vc = vertexCover(g);
    auto id = independentSet(g);
    CHKEQ(N - vc.first, id.first);
    CHK(satisfiedVCID(g, vc.second, 1));
    CHK(satisfiedVCID(g, id.second, 2));
  });
  return 0;
}