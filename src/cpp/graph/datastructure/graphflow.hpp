// %=BEGIN
// %title
// 有向グラフ(最大流用)
//
// %overview
// 辺集合の情報を持つ．
//
// %usage
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

#ifndef SRC_CPP_GRAPH_DATASTRUCTURE_GRAPHFLOW__  // << prototype implementation!!!
#define SRC_CPP_GRAPH_DATASTRUCTURE_GRAPHFLOW__

class DGraphF {
 public:
  typedef int cap_t;
  int n_;
  struct Arc {
    int from, to;
    // 残量
    cap_t left;
    // 容量
    cap_t cap;

    Arc(int from = 0, int to = 0, cap_t w = 1) : from(from), to(to), left(w), cap(w) {}
    inline bool operator<(const Arc& a) const {
      return (left != a.left) ? left < a.left
                              : (left < a.left) | (cap < a.cap) | (from < a.from) | (to < a.to);
    }
    inline bool operator==(const Arc& a) const {
      return (from == a.from) && (to == a.to) && (left == a.left) && (cap == a.cap);
    }
  };
  vector<vector<int>> vertex_to;
  vector<vector<int>> vertex_from;
  vector<Arc> edges;

  explicit DGraphF(int n = 1) : n_(n), vertex_to(n), vertex_from(n) {}

  void connect(int from, int to, cap_t left) {
    vertex_to[(int)from].push_back((int)edges.size());  // toto
    vertex_from[(int)to].push_back((int)edges.size());  // fromfrom
    edges.emplace_back(from, to, left);
  }

  inline int size() const { return n_; }
};

#endif  // SRC_CPP_GRAPH_DATASTRUCTURE_GRAPHFLOW__
