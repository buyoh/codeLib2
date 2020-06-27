#ifndef SRC_CPP_MATH_METHOD_LERP_HPP__
#define SRC_CPP_MATH_METHOD_LERP_HPP__
// %=BEGIN DOC
// %title
// 一次方程式
//
// %overview
// 焼きなましの近似式など．
//
// %require
// ```
// ```
// %usage
// double lerp(double px, double py, double qx, double qy, double x)
// ; (px,py) (qx,qy) を通る直線の関数 y=f(x) の値．
//
// %verified
// %references
// %=END DOC
// %=BEGIN CODE

inline double lerp(double px, double py, double qx, double qy, double x) {
  return py + (qy - py) * (x - px) / (qx - px);
}
// inline double lerp(const d2::Point& p, const d2::Point& q, double x) {
//   return lerp(p.x, p.y, q.x, q.y, x);
// }
// %=END CODE
#endif  // SRC_CPP_MATH_METHOD_LERP_HPP__
