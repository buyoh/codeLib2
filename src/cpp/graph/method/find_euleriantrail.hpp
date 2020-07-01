#ifndef SRC_CPP_GRAPH_METHOD_FIND_EULERIANTRAIL_HPP__
#define SRC_CPP_GRAPH_METHOD_FIND_EULERIANTRAIL_HPP__
// %=BEGIN DOC
// %title
// オイラー路検出
//
// %overview
// 要は一筆書き．
// まだ汚いのでそのうち
// TODO: fix -Wsign-compare
//
// %usage
// vector<int> find_eulerialTrail(const GraphE& graph)
// @ret : 辿り方．頂点番号，辺番号，頂点番号，…と交互に記述される．
//
// %require
// ```
#include <vector>
#include <algorithm>
#include <stack>
#include <map>
using namespace std;
#include "src/cpp/graph/datastructure/unionfind.hpp"
#include "src/cpp/graph/datastructure/dgraphe.hpp"
// ```
// %verified
// [TODO]
// %references
// http://shonen9th.blog.fc2.com/blog-entry-147.html
// %words
// euler
// %tag
// wip, unverified
// %=END DOC
// %=BEGIN CODE

// オイラー路が存在するかどうか
// 存在しなかったら(-1,-1)，存在したら(始点,終点)
// 閉路なら 始点==終点
pair<int, int> has_eulerialTrail(const DGraphE& graph) {
  Unionfind uf(graph.size());

  int vstart = -1, vgoal = -1;

  for (int i = 0; i < graph.size(); ++i) {
    if (graph.vertex_from[i].size() + 1 == graph.vertex_to[i].size()) {
      if (vstart >= 0)
        return make_pair(-1, -1);
      vstart = i;
    } else if (graph.vertex_from[i].size() == graph.vertex_to[i].size() + 1) {
      if (vgoal >= 0)
        return make_pair(-1, -1);
      vgoal = i;
    } else if (graph.vertex_from[i].size() != graph.vertex_to[i].size())
      return make_pair(-1, -1);
  }
  if ((vstart == -1) != (vgoal == -1))
    return make_pair(-1, -1);
  for (const auto& a : graph.arcs) {
    uf.connect(a.from, a.to);
  }
  if (uf.size(0) < graph.size())
    return make_pair(-1, -1);

  if (vstart == -1)
    vgoal = vstart = 0;

  return make_pair(vstart, vgoal);
}

vector<int> find_eulerialTrail(const DGraphE& graph, int startEdge = 0) {
  using boolean = int;

  // 辺集合を複数のサイクルに分割したとき，属するサイクル番号
  vector<int> group(graph.arcs.size(), -1);
  //
  vector<int> prior(graph.arcs.size(), -1);

  int groupCount = 0;

  // ループ辺は独立した1つのサイクルとして分割
  for (int ei = 0; ei < (int)graph.arcs.size(); ++ei)
    if (graph.arcs[ei].from == graph.arcs[ei].to)
      group[ei] = groupCount++;

  // 辺集合を複数のサイクルに分割
  for (int idx_ = -1; idx_ < (int)graph.arcs.size(); ++idx_) {
    int idx = idx_ == -1 ? startEdge : idx_;
    if (group[idx] >= 0)
      continue;

    group[idx] = groupCount++;
    // int tail = graph.arcs[idx].from;
    int v = graph.arcs[idx].to;

    int tim = 0;
    prior[idx] = 0;

    for (bool running = true; running;) {
      running = false;
      for (int ie : graph.vertex_to[v]) {
        if (group[ie] >= 0)
          continue;

        prior[ie] = ++tim;
        group[ie] = group[idx];
        v = graph.arcs[ie].to;
        running = true;
        break;
      }
    }
  }
  vector<int> result;
  stack<int> history;

  int tail = graph.arcs[startEdge].from;
  int curr = graph.arcs[startEdge].to;
  history.push(group[startEdge]);

  result.push_back(tail);
  result.push_back(startEdge);
  result.push_back(curr);

  vector<boolean> encountedGroup(groupCount);
  encountedGroup[group[startEdge]] = true;

  group[startEdge] = -1;

  for (int step = 1; step < (int)graph.arcs.size(); ++step) {
    bool prfound = false;
    map<int, pair<int, int>> connected;  // [group] = prior, edgeid
    for (int ie : graph.vertex_to[curr]) {
      // 通過済みの辺
      if (group[ie] < 0)
        continue;

      int g = group[ie];

      // 通過したことが無いサイクルならば
      if (!encountedGroup[g]) {
        history.push(g);
        encountedGroup[g] = true;

        group[ie] = -1;
        curr = graph.arcs[ie].to;
        result.push_back(ie);
        result.push_back(curr);

        prfound = true;
        break;
      } else {
        if (connected.count(g) == 0)
          connected[g] = make_pair(prior[ie], ie);
        else
          connected[g] = min(connected[g], make_pair(prior[ie], ie));
      }
    }
    if (prfound)
      continue;
    if (connected.empty())
      break;

    {
      // 今通過しているサイクルを一通り巡回したならば，1つ前のサイクルに乗り換える
      while (connected.count(history.top()) == 0) {
        history.pop();
        if (history.empty())
          return result;  // assertion
      }
      // 1つ前のサイクルに乗り換える，辺id
      int ie = connected[history.top()].second;
      group[ie] = -1;
      curr = graph.arcs[ie].to;

      result.push_back(ie);
      result.push_back(curr);
    }
  }

  return result;
}
// %=END CODE
#endif  // SRC_CPP_GRAPH_METHOD_FIND_EULERIANTRAIL_HPP__
