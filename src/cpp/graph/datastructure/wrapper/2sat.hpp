#ifndef SRC_CPP_GRAPH_DATASTRUCTURE_WRAPPER_2SAT_HPP__
#define SRC_CPP_GRAPH_DATASTRUCTURE_WRAPPER_2SAT_HPP__
// %=BEGIN DOC
// %title
// 2-SAT Solver
//
// %overview
// 2-SATを解く．
// 強連結成分分解は内包済み．DGraphが必要．
//
// %usage
// Sat_2(int n)
// ;
// void Sat_2::push(int a, int b)
// ; 節を追加する．節は2つのリテラルから構成される．
// ; リテラルの番号は1-indexed．
// ; 負のインデックスを指定すると，否定として扱われる．
// vector<int> Sat_2::solve()
// ; 充足不能なら、empty。
// ; result[i]は，i番目のリテラルがTかFか．0-indexed
//
// %require
// ```
#include <cassert>
#include <vector>
using namespace std;
#include "src/cpp/graph/datastructure/dgraph.hpp"
// ```
//
// %verified
// https://yukicoder.me/submissions/496242
//
// %references
// http://www.prefield.com/algorithm/misc/2-sat.html
// プログラミングコンテストチャレンジブック
// %=END DOC

// 考察メモ:
// 強連結を考察する上で、強連結を纏めるとDAGになる、という定義を常にイメージしておくと良さそう
// DAGと書くと難しいが、DAGの例としてPATHをイメージすると、
// 強連結が1つの層で、トポロジカル順が階層になっている、がわかりやすい。
// solve_dfs1
// - postorder(帰りがけ順)を作る
// solve_dfs2
// - グラフの辺を逆にして開始
// - postorderの逆順(最後にpushした頂点から順)にsolve_dfs2を叩いていく
// - 到達済みはreject
// - よってdfsの開始番号から大きいものへ辿ることは無い
// - DAGであれば、開始番号より小さいものへ辿れない
// - 遡れたら、その辺はfeedback辺である
// - 最初の1つ目は、開始番号より小さい頂点
// - そして何故か、番号関係なく逆方向の辺を辿っていくと強連結を洗い出せる
// - ここがよくわからないが、動いているからヨシ！
// - .
// - 真偽を付けるには、トポロジカル順序が必要
// - 既に強連結が分かっているので、3つ目のdfsをすれば（証明は）楽。
// - 2つ目のdfsを使い回すことを考えてみる。これは、あっさり出来る。
// - 2つ目の再帰中に辿る頂点たちは強連結なので、これは同じトポ順である。
// - よって、正しいトポ順かどうか見るには、postorderだけに注目すれば良い
// - これは、辺を逆にしている効果が効いてくる。
// - 辺を逆にしていない方のグラフで、有向先に行くことは無いので、トポ順は正しい…

// ループ1つにしたほうが早そう
// https://yukicoder.me/submissions/496242

// %=BEGIN CODE

class Sat2 {
 public:
  int n;
  DGraph graph;

  Sat2(int n) : n(n), graph(n * 2) {}

 private:
  inline int _cv(int v) { return 0 < v ? (v - 1) * 2 : (-v - 1) * 2 + 1; }

  vector<int> label_, postorder_;

  void solveScc_dfs1(int v) {
    if (label_[v] < 0)
      return;
    label_[v] = -1;
    for (int to : graph.vertex_to[v])
      solveScc_dfs1(to);
    postorder_.push_back(v);
  }

  void solveScc_dfs2(int v, int rank) {
    if (label_[v] > 0)
      return;
    label_[v] = rank;
    for (int to : graph.vertex_from[v])
      solveScc_dfs2(to, rank);
  }

 public:
  // 1 <= a <= n OR -1 >= a >= -n
  // 正ならばx_a，負ならばNOT x_aを表現．
  inline void push(int a, int b) {
    assert(a != 0 && b != 0);
    graph.connect(_cv(-a), _cv(b));
    graph.connect(_cv(-b), _cv(a));
  }

  vector<int> solve() {
    label_.resize(graph.n);
    postorder_.reserve(graph.n);

    for (int i = 0; i < graph.n; ++i)
      solveScc_dfs1(i);
    for (int i = (int)postorder_.size() - 1; 0 <= i; --i)
      solveScc_dfs2(postorder_[i], i + 1);

    vector<int> result(n);
    for (int i = 0; i < n; ++i) {
      if (label_[i * 2] == label_[i * 2 + 1])
        return {};
      result[i] = (label_[i * 2] < label_[i * 2 + 1]);
    }
    return result;
  }
};

// %=END CODE
#endif  // SRC_CPP_GRAPH_DATASTRUCTURE_WRAPPER_2SAT_HPP__
