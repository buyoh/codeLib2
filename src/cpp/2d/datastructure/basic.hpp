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

struct Pt {
  using value_t = double;
  value_t x, y;
  Pt(value_t _x = 0, value_t _y = 0) : x(_x), y(_y) {}
  inline void normalize() {
    if (x != 0 || y != 0)
      x /= hypot(), y /= hypot();
  }
  inline value_t hypot() const { return std::hypot(x, y); }
  inline value_t hypot(Pt p) const { return std::hypot(x - p.x, y - p.y); }
  inline value_t dot(Pt p) const { return x * p.x + y * p.y; }
  inline value_t cross(Pt p) const { return x * p.y - y * p.x; }
  inline Pt normalized() const {
    return x != 0 || y != 0 ? Pt(x / hypot(), y / hypot()) : Pt(0, 0);
  }
  inline Pt operator+(Pt p) const { return Pt(x + p.x, y + p.y); }
  inline Pt operator-(Pt p) const { return Pt(x - p.x, y - p.y); }
  inline Pt& operator+=(Pt p) {
    x += p.x;
    y += p.y;
    return *this;
  }
  inline Pt& operator-=(Pt p) {
    x -= p.x;
    y -= p.y;
    return *this;
  }
};
Pt operator*(Pt::value_t a, Pt p) {
  return Pt(a * p.x, a * p.y);
}
// %=END CODE
#endif  // SRC_CPP_2D_DATASTRUCTURE_BASIC_HPP__
