#ifndef SRC_CPP_2D_DATASTRUCTURE_BASIC_HPP__
#define SRC_CPP_2D_DATASTRUCTURE_BASIC_HPP__
// %=BEGIN DOC
// %title
// 幾何頂点
//
// %overview
// 二次元座標上の点を表す構造体
//
// %usage
//
// %require
// ```
#include <cmath>
using namespace std;
// ```
// %verified
//
// %references
//
// %words
//
// %=END DOC
// %=BEGIN CODE

struct P {
  using value_t = double;
  value_t x, y;
  P(value_t _x = 0, value_t _y = 0) : x(_x), y(_y) {}
  inline void normalize() {
    if (x != 0 || y != 0)
      x /= hypot(), y /= hypot();
  }
  inline value_t hypot() const { return std::hypot(x, y); }
  inline value_t hypot(P p) const { return std::hypot(x - p.x, y - p.y); }
  inline value_t dot(P p) const { return x * p.x + y * p.y; }
  inline value_t cross(P p) const { return x * p.y - y * p.x; }
  inline P normalized() const { return x != 0 || y != 0 ? P(x / hypot(), y / hypot()) : P(0, 0); }
  inline P operator+(P p) const { return P(x + p.x, y + p.y); }
  inline P operator-(P p) const { return P(x - p.x, y - p.y); }
  inline P& operator+=(P p) {
    x += p.x;
    y += p.y;
    return *this;
  }
  inline P& operator-=(P p) {
    x -= p.x;
    y -= p.y;
    return *this;
  }
};
P operator*(P::value_t a, P p) {
  return P(a * p.x, a * p.y);
}
// %=END CODE
#endif  // SRC_CPP_2D_DATASTRUCTURE_BASIC_HPP__
