#ifndef SRC_CPP_2D_METHOD_CONVEX_HULL_HPP__
#define SRC_CPP_2D_METHOD_CONVEX_HULL_HPP__
// %=BEGIN DOC
// %title
// 凸包
//
// %overview
// 凸包を求める。
//
// %usage
// pair<Pt, int> になっているが、secondのintには何を入れてもok（多分）。
// インデックス情報が入ることを期待している。
//
// %require
// ```
#include <vector>
using namespace std;
#include "src/cpp/2d/datastructure/basic.hpp" // duplicate
#include "src/cpp/2d/method/ccw.hpp"
// ```
// %verified
// atcoder
//
// %references
// http://www.prefield.com/algorithm/geometry/convex_hull.html
// %words
// convex
// %=END DOC
// %=BEGIN CODE

vector<pair<Pt, int>> convex_hull(const vector<pair<Pt, int>>& points) {
  // pointsはソート済みであること
  const int n = points.size();
  vector<pair<Pt, int>> ch(2 * n);
  int k = 0;
  for (int i = 0; i < n; ch[k] = points[i], ++k, ++i)  // lower-hull
    while (k >= 2 && ccw(ch[k - 2].first, ch[k - 1].first, points[i].first) <= 0)
      --k;
  for (int i = n - 2, t = k + 1; i >= 0; ch[k] = points[i], ++k, --i)  // upper-hull
    while (k >= t && ccw(ch[k - 2].first, ch[k - 1].first, points[i].first) <= 0)
      --k;
  ch.resize(k - 1);
  return ch;
}
// %=END CODE
#endif  // SRC_CPP_2D_METHOD_CONVEX_HULL_HPP__
