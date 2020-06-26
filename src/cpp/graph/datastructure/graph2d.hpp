#ifndef SRC_CPP_GRAPH_DATASTRUCTURE_GRAPH2D_HPP__
#define SRC_CPP_GRAPH_DATASTRUCTURE_GRAPH2D_HPP__
// %=BEGIN
// %title
// 無向グラフ(隣接行列)
//
// %overview
// 隣接行列を持つ．
// 必要な空間計算量は |V|^2
//
// %usage
// void connect(int from, int to, W_T dist = 1)
// ;つなげる．ループ辺は非推奨．
// void resize(int _n)
// ;頂点数を変更する．
// ;global変数として宣言した時に用いる．
// inline W_T& operator()(int y, int x);
// ;行列の値の参照
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

class Graph2d {
 public:
  using W_T = int;
  int n;
  vector<W_T> matrix;

  explicit Graph2d(int size) : n(size), matrix(size * size){};

  inline int size() const { return n; }
  void resize(int s) {
    n = s;
    matrix.resize(n * n);
  }
  void resize(int s, W_T val) {
    n = s;
    matrix.resize(n * n, val);
  }

  inline W_T& at(int y, int x) { return matrix[y * n + x]; }
  inline W_T& operator()(int y, int x) { return matrix[y * n + x]; }
  inline W_T at(int y, int x) const { return matrix[y * n + x]; }
  inline W_T operator()(int y, int x) const { return matrix[y * n + x]; }

  inline void connect(int u, int v, W_T dist = 1) { at(u, v) = at(v, u) = dist; }
  inline void connect_d(int from, int to, W_T dist = 1) {  // directedEdge u->v
    at(from, to) = dist;
  }
};
#endif  // SRC_CPP_GRAPH_DATASTRUCTURE_GRAPH2D_HPP__
