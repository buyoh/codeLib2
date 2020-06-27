#ifndef SRC_CPP_GRAPH_METHOD_DISTANCE_WARSHALLFLOYD_HPP__
#define SRC_CPP_GRAPH_METHOD_DISTANCE_WARSHALLFLOYD_HPP__
// %=BEGIN DOC
// %title
// warshall-floyd
//
// %overview
// 全ペアの最短経路を求める．
// [note] 辺が無い頂点ペアの距離はinfで埋めておくこと．
//
// %usage
// void warshall_floyd(Graph2d& g)
//
// %require
// ```
#include <algorithm>
#include "src/cpp/graph/datastructure/graph2d.hpp"
// ```
// %verified
//
// %references
//
// %=END DOC
// %=BEGIN CODE

void warshall_floyd(Graph2d& g) {
  int i, j, k;
  for (i = 0; i < g.n; i++) {
    for (j = 0; j < g.n; j++) {
      for (k = 0; k < g.n; k++) {
        g(j, k) = std::min(g(j, k), g(j, i) + g(i, k));
      }
    }
  }
}
// %=END CODE
#endif  // SRC_CPP_GRAPH_METHOD_DISTANCE_WARSHALLFLOYD_HPP__
