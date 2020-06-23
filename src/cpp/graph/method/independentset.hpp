// %title
// 最大独立集合 (分岐限定法)
//
// %overview
// グラフの最大独立集合の大きさと選ぶ頂点集合を求める．
// 入力グラフは単純グラフであること．多重辺はNG．
// 最小頂点被覆，最大クリークに転用可能．
// 分岐限定法．
//
// %usage
// int independentSet(const Graph& graph)
//
// %require
// ```
#include <vector>
#include <cstdint>
#include "src/cpp/graph/datastructure/graph.hpp"
// ```
// %verified
// https://atcoder.jp/contests/code-thanks-festival-2017-open/submissions/4080519
//
// %words
// vertexcover, independentSet
//
// %references
// FV Fomin, Exact Exponential Algorithms, Springer.
// %tag
// wip

pair<int, vector<int>> independentSet(const Graph& graph) {
  int bestScore = 0;
  vector<int8_t> bestSelection;

  vector<int8_t> selection(graph.size());
  vector<int> degree(graph.size());

  for (int i = 0; i < graph.size(); ++i)
    degree[i] = graph.vertex_to[i].size();

  auto deleteVtxSel = [&](int vtx, int id) -> int {
    int del = 1;
    selection[vtx] = id;
    for (int to : graph.vertex_to[vtx])
      if (selection[to] == 0) {
        selection[to] = -id;
        ++del;
      }
    for (int to : graph.vertex_to[vtx])
      if (selection[to] == -id)
        for (int to2 : graph.vertex_to[to])
          if (selection[to2] == 0)
            degree[to2]--;
    return del;
  };

  auto undoVtxSel = [&](int vtx, int id) {
    for (int to : graph.vertex_to[vtx])
      if (selection[to] == -id)
        for (int to2 : graph.vertex_to[to])
          if (selection[to2] == 0)
            degree[to2]++;
    for (int to : graph.vertex_to[vtx])
      if (selection[to] == -id)
        selection[to] = 0;
    selection[vtx] = 0;
  };

  auto deleteVtxUnsel = [&](int vtx, int id) -> int {
    selection[vtx] = -id;
    for (int to : graph.vertex_to[vtx])
      if (selection[to] == 0)
        degree[to]--;
    return 1;
  };

  auto undoVtxUnsel = [&](int vtx, int id) {
    for (int to : graph.vertex_to[vtx])
      if (selection[to] == 0)
        degree[to]++;
    selection[vtx] = 0;
  };

  auto dfs = [&](auto& dfs, int depth, int rem, int score) -> void {
    if (rem == 0) {
      if (bestScore < score) {
        bestScore = score;
        bestSelection = selection;
      }
      return;
    }
    if (score + rem < bestScore)
      return;

    int minDeg = 1e9, minDegi = 0;
    int maxDeg = 0, maxDegi = 0;
    int zeroDegCnt = 0;
    for (int i = 0; i < graph.size(); ++i) {
      if (selection[i] != 0)
        continue;
      if (degree[i] == 0) {
        ++zeroDegCnt;
        selection[i] = depth;
      }
      if (minDeg > degree[i]) {
        minDeg = degree[i];
        minDegi = i;
      }
      if (maxDeg < degree[i]) {
        maxDeg = degree[i];
        maxDegi = i;
      }
    }

    if (zeroDegCnt > 0) {
      dfs(dfs, depth + 1, rem - zeroDegCnt, score + zeroDegCnt);
      for (int i = 0; i < graph.size(); ++i) {
        if (selection[i] == depth)
          selection[i] = 0;
      }
      return;
    }

    if (minDeg == 1 || (minDeg == 2 && maxDeg == 2)) {
      int dl = deleteVtxSel(minDegi, depth);
      dfs(dfs, depth + 1, rem - dl, score + 1);
      undoVtxSel(minDegi, depth);
      return;
    }

    {
      deleteVtxUnsel(maxDegi, depth);
      dfs(dfs, depth + 1, rem - 1, score);
      undoVtxUnsel(maxDegi, depth);

      int dl = deleteVtxSel(maxDegi, depth);
      dfs(dfs, depth + 1, rem - dl, score + 1);
      undoVtxSel(maxDegi, depth);
    }
  };

  dfs(dfs, 1, graph.size(), 0);

  vector<int> idxs;
  for (int i = 0; i < graph.size(); ++i)
    if (bestSelection[i] > 0)
      idxs.push_back(i);
  idxs.shrink_to_fit();
  return make_pair(bestScore, idxs);
}
