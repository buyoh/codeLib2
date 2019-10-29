// %title
// 最小流量制限付き最大フロー
//
// %overview
// 最小流量制限付き最大フローは，普通の最大フローに置き換えることができる．
//
// %usage
// solve_dinic_edge(map<pair<int, int>, int>& result_edge, int i_source, int i_sink)
// ; i_source から i_sink に流す
// ; 解が存在するならばtrueを返す
// ; result_edge[make_pair(u,v)] で辺の流量
//
// %require
// ```
#include <vector>
#include <map>
using namespace std;
#include "src/cpp/graph/datastructure/graphflow.cpp"
#include "src/cpp/graph/method/flow/dinic.cpp"
// ```
// %verified
// http://yukicoder.me/submissions/137248
// http://yukicoder.me/submissions/143696
// %references
// http://snuke.hatenablog.com/entry/2016/07/10/043918
// %words
// maxflow,dinic
// #### (昔書いた)解説
// 最小流量制限付き最大フローは，普通の最大フローに置き換えることができる．
//
// 面倒なので，最小流l,最大流hで頂点uから頂点vへ流れる有向辺を(u,v)[l,h]と表記する．
//
// + s→tな最大最小流量制限付きフローG=(V,E)を考える．最大流量制限付きフローG'を作りたい．
// + 新たに頂点S,Tを作る．
// + (u,v)[c,c+d]がGに存在するとき，G'に(u,v)[0,d],(u,T)[0,c],(S,v)[0,c]を与える．
// + G'に多重辺が出来ることがある．
// + S→T,S→t,s→T,s→tの順に最大流を求める．S,Tに隣接する辺に優先して流すため．
// + S,Tに隣接する辺が全てemptyになっていれば，条件を満たすフローが存在
// + 流量は(u,v)+(u,T)
//
// 事前に全体の流量が把握出来るならば， #137248のように,S→s,t→Tの辺を作ってS→Tを流せばよい

class FlowMinMax {
 public:
  DGraphF graph;
  const int v_source;  // vertex of new source
  FlowMinMax(int n) : graph(n + 2), v_source(n) {}

 private:
  bool _solve_maxflow_edge(map<pair<int, int>, int>& result_edge, int i_source, int i_sink) {
    vector<int> resflow(graph.size(), 0);

    dinic(graph, resflow, v_source, v_source + 1);
    dinic(graph, resflow, v_source, i_sink);
    dinic(graph, resflow, i_source, v_source + 1);
    dinic(graph, resflow, i_source, i_sink);

    for (int e : graph.vertex_from[v_source + 1]) {
      const DGraphF::Arc& a = graph.edges[e];
      if (0 < a.left)
        return false;
    }
    for (int u = 0; u < graph.size() - 2; u++) {
      for (int ei : graph.vertex_to[u]) {            // TODO:最適化の余地あり(らしい)
        const DGraphF::Arc& a_uv = graph.edges[ei];  // u -> v
        if (a_uv.to >= graph.size() - 2) {
          if (0 < a_uv.left)
            return false;
          continue;
        }

        const DGraphF::Arc& a_sv = graph.edges[ei + 1];  // S -> v
        int flow;
        if (a_uv.to != a_sv.to) {
          flow = a_uv.cap - a_uv.left;
        } else {
          if (0 < a_sv.left)
            return false;
          flow = a_sv.cap + a_uv.cap - a_sv.left - a_uv.left;
        }
        if (0 < flow)
          result_edge[make_pair(u, a_uv.to)] += flow;
      }
    }
    return true;
  }

 public:
  void connect(int from, int to, int w_min, int w_max) {
    if (w_max == w_min) {
      graph.connect(v_source, to, w_min);
      graph.connect(from, v_source + 1, w_min);
    } else if (w_min == 0) {
      graph.connect(from, to, w_max - w_min);
    } else {
      graph.connect(from, v_source + 1, w_min);
      graph.connect(from, to, w_max - w_min);
      graph.connect(v_source, to, w_min);
    }
  }

  inline bool solve_maxflow_edge(map<pair<int, int>, int>& result_edge, int i_source, int i_sink) { return _solve_maxflow_edge(result_edge, i_source, i_sink); }
};
