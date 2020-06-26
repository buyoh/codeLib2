#ifndef SRC_CPP_GRAPH_DATASTRUCTURE_GRAPH_HPP__
#define SRC_CPP_GRAPH_DATASTRUCTURE_GRAPH_HPP__
// %=BEGIN
// %title
// 無向グラフ(隣接頂点)
//
// %overview
// 隣接頂点の情報を持つ．
// 必要な空間計算量は |V|+2|E|
//
// %usage
// void connect(int from, int to)
// ;つなげる．ループ辺は非推奨．
// void resize(int _n)
// ;頂点数を変更する．
// ;global変数として宣言した時に用いる．
//
// %require
// ```
#include <vector>
using namespace std;
// ```
// %verified
//
// %references
//
// %words
// graphstruct
// %=END

class Graph {
 public:
  int n;
  vector<vector<int>> vertex_to;

  explicit Graph(int n = 1) : n(n), vertex_to(n) {}

  inline int size() const { return n; }
  void resize(int _n) { vertex_to.resize(n = _n); }
  void connect(int from, int to) {
    vertex_to[(int)from].emplace_back(to);
    vertex_to[(int)to].emplace_back(from);
  }
};
#endif  // SRC_CPP_GRAPH_DATASTRUCTURE_GRAPH_HPP__
