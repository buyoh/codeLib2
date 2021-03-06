#ifndef SRC_CPP_GRAPH_DATASTRUCTURE_WRAPPER_PROJECTSELECTION_HPP__
#define SRC_CPP_GRAPH_DATASTRUCTURE_WRAPPER_PROJECTSELECTION_HPP__
// %=BEGIN DOC
// %title
// ProjectSelection
//
// %overview
// project p_1..p_n があり，それぞれのprojectは状態Bである．
// 今，いくつかのpeojectを状態Aに移すことができる．
// - (i, r) p_iが状態Aの時，利益(損失)rを得る．
// - (i, j, q)p_iが状態Aでp_jが状態Bの時，qの損失が発生する．
// 利益を最大化する．
// 燃やす埋めるフローの考察の補助に使うことができる．．
//
// %usage
// void push_revenue(int i, DGraphF::cap_t r)
// ;p_iが状態Aの時，利益(損失)rを得る．
// void push_purchase(int i, int j, DGraphF::cap_t q)
// ;p_iが状態Aでp_jが状態Bの時，qの損失が発生する．
// DGraphF::cap_t solve()
// 解く．
// %require
// ```
#include <vector>
// #include "src/cpp/graph/datastructure/graphflow.hpp" // duplicate
#include "src/cpp/graph/method/flow/dinic.hpp"
// ```
//
// %verified
// verified(ARC)
//
// %references
// https://en.wikipedia.org/wiki/Max-flow_min-cut_theorem#Project_selection_problem
// http://tokoharuland.hateblo.jp/entry/2017/11/12/234636
// %=END DOC
// %=BEGIN CODE

class ProjectSelection {
 public:
  int n_;
  DGraphF graph_;
  const DGraphF::cap_t inf_ = 1e9;
  DGraphF::cap_t potential_ = 0;

  ProjectSelection(int _n) : n_(_n), graph_(_n + 2) {}

  // p_iが状態Aの時，利益(損失)rを得る．
  inline void push_revenue(int i, DGraphF::cap_t r) {
    potential_ += max(DGraphF::cap_t(0), r);
    if (r > 0)
      graph_.connect(n_, i, r);
    else if (r < 0)
      graph_.connect(i, n_ + 1, -r);
  }
  // p_iが状態Aでp_jが状態Bの時，qの損失が発生する．
  inline void push_purchase(int i, int j, DGraphF::cap_t q) { graph_.connect(i, j, q); }

  DGraphF::cap_t solve() {
    std::vector<DGraphF::cap_t> result;
    dinic(graph_, result, n_, n_ + 1);
    return potential_ - result[n_ + 1];
  }
};
// %=END CODE
#endif  // SRC_CPP_GRAPH_DATASTRUCTURE_WRAPPER_PROJECTSELECTION_HPP__
