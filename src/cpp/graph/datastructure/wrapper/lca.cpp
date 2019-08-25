// %title
// LCA
//
// %overview
// 根付き木が与えられたとき，準備O(NlogN)，次のクエリにO(1)で答えるアルゴリズム・データ構造．
// - 頂点u,vに共通する最も若い祖先
// LCA問題は，構築処理さえ済めば各質問をRMQの時間で解くことが出来る．
//
// ループを含むグラフを与えた場合，再帰処理が終わらない可能性がある．
//
// %usage
// LCATable(const Graph& graph, int root = 0)
// ; rootを根としたgraphに対してLCAを構築する
// int LCATable::operator()(int u, int v)
// ; 頂点u,vに共通する最も若い祖先の頂点番号を取得する
//
// %verified
// problem.
//
// %references
// thanks.
// %words
// lca
// %require
// cpp/graph/datastructure/graph.cpp
// cpp/container/likermq/rmq_sparsetable.cpp

class LCATable {
  vector<int> visited_;
  vector<int> visited_inv_;
  SparseTable<int> depth_;

 public:
  LCATable(const Graph& g, int root = 0) : visited_(g.n * 2), visited_inv_(g.n), depth_(g.n * 2) {
    build(g, root);
  }

  int _tour_dfs(const Graph& g, int idx, int from = -1, int step = 0, int dep = 0) {
    depth_[step] = dep;
    visited_inv_[idx] = step;
    visited_[step] = idx;

    for (int to : g.vertex_to[idx]) {
      if (to == from)
        continue;
      step = _tour_dfs(g, to, idx, ++step, dep + 1);
      depth_[step] = dep;
      visited_[step] = idx;
    }
    return ++step;
  }

  inline void build(const Graph& g, int root = 0) {
    _tour_dfs(g, root);
    depth_.build();
  }

  inline int operator()(int u, int v) {
    return visited_inv_[u] <= visited_inv_[v]
               ? visited_[depth_.getminrangeIdx(visited_inv_[u], visited_inv_[v])]
               :
               operator()(v, u);
  }
};
