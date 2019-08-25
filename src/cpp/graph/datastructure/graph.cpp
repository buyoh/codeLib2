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
// void resize(size_t _n)
// ;頂点数を変更する．
// ;global変数として宣言した時に用いる．
//
// %verified
//
// %references
//
// %words
// graphstruct

class Graph {
 public:
  size_t n;
  vector<vector<int>> vertex_to;

  Graph(size_t n = 1) : n(n), vertex_to(n) {}

  inline size_t size() const { return n; }
  void resize(size_t _n) { vertex_to.resize(n = _n); }
  void connect(int from, int to) {
    vertex_to[(size_t)from].emplace_back(to);
    vertex_to[(size_t)to].emplace_back(from);
  }
};