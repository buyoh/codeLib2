// %title
// 2次元グリッド上の距離(マンハッタン距離)
//
// %overview
//
//
// %usage
//
//
// %require
// ```
#include <functional>
#include <limits>
#include <queue>
using namespace std;
#include "src/cpp/grid/datastructure/euclid.cpp"
// ```
// %verified
// [TODO]
//
// %references
//
// %words
// grid, field, point
// %require
//

template <typename T>
F<int> make_distancetable(int height,
                          int width,
                          const vector<P>& start_point,
                          function<bool(P)> ismoveable) {
  constexpr int inf = numeric_limits<int>::max();

  F<int> result(height, width);
  result.fill(inf);

  queue<P> q;
  for (const P& p : start_point)
    q.push(p), result(p) = 0;

  while (!q.empty()) {
    P p = q.front();
    q.pop();
    int d = result(p);
    if (0 < p.y && result(p.y - 1, p.x) == inf && ismoveable(P(p.y - 1, p.x)))
      q.emplace(p.y - 1, p.x), result(p.y - 1, p.x) = d + 1;
    if (p.y < height - 1 && result(p.y + 1, p.x) == inf && ismoveable(P(p.y + 1, p.x)))
      q.emplace(p.y + 1, p.x), result(p.y + 1, p.x) = d + 1;
    if (0 < p.x && result(p.y, p.x - 1) == inf && ismoveable(P(p.y, p.x - 1)))
      q.emplace(p.y, p.x - 1), result(p.y, p.x - 1) = d + 1;
    if (p.x < width - 1 && result(p.y, p.x + 1) == inf && ismoveable(P(p.y, p.x + 1)))
      q.emplace(p.y, p.x + 1), result(p.y, p.x + 1) = d + 1;
  }
  return result;
}
