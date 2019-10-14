// %title
// 無向グラフ(辺集合)
//
// %overview
// 辺集合の情報を持つ．
// 辺には1つの値を持たせることができる．
// 必要な空間計算量は |V|+5|E|
//
// %usage
// void connect(int from, int to, W_T val = 0)
// ;つなげる．ループ辺は非推奨．
// void resize(size_t _n)
// ;頂点数を変更する．
// ;global変数として宣言した時に用いる．
// void connect(vector<Edges>&& edges)
// ; edgesを基に構築（グラフは空であること）
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

// Edge構造体を定義する無向グラフ
class GraphE {
 public:
  using W_T = int;
  struct Edge {
    int u, v;
    W_T value;
    Edge(int from = 0, int to = 0, W_T value = 0) : u(from), v(to), value(value) {}
    inline int to(int _v) const { return _v == v ? u : v; }
  };
  size_t n;
  vector<vector<int>> vertex_to;
  vector<Edge> edges;

  explicit GraphE(int n = 1) : n(n), vertex_to(n) {}

  inline size_t size() const noexcept { return n; }
  void resize(size_t _n) { vertex_to.resize(n = _n); }
  void connect(int from, int to, W_T val = 0) {
    vertex_to[(size_t)from].push_back((int)edges.size());
    vertex_to[(size_t)to].push_back((int)edges.size());
    edges.emplace_back(from, to, val);
  }
  void connect(vector<Edge>&& es) {
    edges = move(es);
    for (int i = 0; (size_t)i < edges.size(); ++i) {
      vertex_to[edges[i].u].push_back(i);
      vertex_to[edges[i].v].push_back(i);
    }
  }
};