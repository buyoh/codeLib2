#ifndef SRC_CPP_2D_METHOD_CCW_HPP__
#define SRC_CPP_2D_METHOD_CCW_HPP__
// %=BEGIN DOC
// %title
// ccw(進行方向)
//
// %overview
// 二次元座標上の点を表す構造体
// a -> b -> c と進むときの方向を判定する。
//
// %usage
//
// %require
// ```
#include "src/cpp/2d/datastructure/basic.hpp"
// ```
// %verified
//
// %references
// http://www.prefield.com/algorithm/geometry/ccw.html
// %words
// ccw
// %=END DOC
// %=BEGIN CODE

int ccw(Pt a, Pt b, Pt c) {
  b.x -= a.x;
  b.y -= a.y;
  c.x -= a.x;
  c.y -= a.y;
  if (b.x * c.y - b.y * c.x > 0)
    return +1;  // 半時計周り
  if (b.x * c.y - b.y * c.x < 0)
    return -1;  // 時計回り
  if (b.x * c.x + b.y * c.y < 0)
    return +2;  // c - a - b
  if (b.hypot() < c.hypot())
    return -2;  // a - b - c
  return 0;     // a - c - b
}
// %=END CODE
#endif  // SRC_CPP_2D_METHOD_CCW_HPP__
