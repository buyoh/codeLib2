#ifndef SRC_CPP_GRID_DATASTRUCTURE_WALLABLE_FIELD_HPP__
#define SRC_CPP_GRID_DATASTRUCTURE_WALLABLE_FIELD_HPP__
// %=BEGIN DOC
// %title
// 境界に壁のある2次元グリッド
//
// %overview
// ゲームAI用。
//
// %usage
//
// %require
// ```
#include <cassert>
#include "src/cpp/grid/datastructure/euclid.hpp"
using namespace std;
// ```
//
// %verified
// https://atcoder.jp/contests/ahc023/submissions/45438718
//
// %references
//
// %words
// grid, field, point, game
// %=END DOC
// %=BEGIN CODE

class WallableField : public F<int> {
 public:
  WallableField(int h, int w) : F(h, w) {}
  WallableField() : WallableField(1, 1) {}
  // L U R D
  enum { DIR_L = 1, DIR_U = 2, DIR_R = 4, DIR_D = 8 };

  inline bool safe(int y, int x) const { return 0 <= y && y < height && 0 <= x && x < width; }
  inline bool safe(P p) const { return 0 <= p.y && p.y < height && 0 <= p.x && p.x < width; }

  void setWall(int y, int x, int dir) {
    if (!safe(y, x))
      return;
    if (dir == DIR_L) {
      operator()(y, x) |= DIR_L;
    } else if (dir == DIR_U) {
      operator()(y, x) |= DIR_U;
    } else if (dir == DIR_R) {
      setWall(y, x + 1, DIR_L);
    } else if (dir == DIR_D) {
      setWall(y + 1, x, DIR_U);
    }
  }

  bool isWall(int y, int x, int dir) const {
    if (!safe(y, x))
      return true;
    if (dir == DIR_L) {
      return x <= 0 || (operator()(y, x) & DIR_L);
    } else if (dir == DIR_U) {
      return y <= 0 || (operator()(y, x) & DIR_U);
    } else if (dir == DIR_R) {
      return isWall(y + 1, x, DIR_L);
    } else if (dir == DIR_D) {
      return isWall(y, x + 1, DIR_U);
    }
    return true;
  }

  bool isWall(int y, int x, P dir) const {
    if (dir == P(-1, 0)) {
      return isWall(y, x, DIR_U);
    } else if (dir == P(0, -1)) {
      return isWall(y, x, DIR_L);
    } else if (dir == P(1, 0)) {
      return isWall(y + 1, x, DIR_U);
    } else if (dir == P(0, 1)) {
      return isWall(y, x + 1, DIR_L);
    }
    return true;
  }

  inline bool isWall(P p, int dir) const { return isWall(p.y, p.x, dir); }
  inline bool isWall(P p, P dir) const { return isWall(p.y, p.x, dir); }
};
// %=END CODE
#endif  // SRC_CPP_GRID_DATASTRUCTURE_WALLABLE_FIELD_HPP__
