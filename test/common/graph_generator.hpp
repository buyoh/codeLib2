#ifndef TEST_COMMON_GRAPH_GENERATOR_HPP__
#define TEST_COMMON_GRAPH_GENERATOR_HPP__

#include "testutil.hpp"

namespace GraphGenerator {

struct UnionFind {
  vector<int> meta_;
  UnionFind(int n) : meta_(n, -1) {}
  int root(int v) { return meta_[v] < 0 ? v : (meta_[v] = root(meta_[v])); }
  bool connect(int u, int v) {
    u = root(u);
    v = root(v);
    if (u == v)
      return false;
    if (-meta_[u] > -meta_[v])
      swap(u, v);
    meta_[u] += meta_[v];
    meta_[v] = u;
    return true;
  }
  bool same(int u, int v) { return root(u) == root(v); }
};

template <typename G, typename F>
void connectedGraph(int n, F callback) {
  int m = n * (n - 1) / 2;
  repeat(b, (1 << m)) {
    bitset<32> bit(b);
    G g(n);
    UnionFind uf(n);
    {
      int i = 0;
      repeat(x, n - 1) {
        iterate(y, x + 1, n) {
          if (bit[i])
            g.connect(x, y), uf.connect(x, y);
          ++i;
        }
      }
    }
    bool connected = true;
    repeat(i, n - 1) connected &= uf.same(i, i + 1);
    if (connected)
      callback(move(g));
  }
}

}  // namespace GraphGenerator

#endif  // TEST_COMMON_GRAPH_GENERATOR_HPP__
