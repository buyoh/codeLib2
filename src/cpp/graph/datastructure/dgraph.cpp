// %title
// 有向グラフ(隣接頂点)
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

class DGraph {
 public:
  int n;
  vector<vector<int>> vertex_to;
  vector<vector<int>> vertex_from;

  DGraph(int n = 1) : n(n), vertex_to(n), vertex_from(n) {}

  inline int size() const { return n; }
  void resize(int _n) {
    n = _n;
    vertex_to.resize(_n);
    vertex_from.resize(_n);
  }
  void connect(int from, int to) {
    vertex_to[(int)from].emplace_back(to);
    vertex_from[(int)to].emplace_back(from);
  }
};