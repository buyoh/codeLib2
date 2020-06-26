// %=BEGIN
// %title
// 回転角
//
// %overview
// 1周をmodで表現するような角度の単位系で，
// 角度xから角度yへ向きを変えるとき，xに加算するべき値
// 例えば，mod=360の時，
// x=45, y=135ならば，return=90
// x=45, y=315ならば，return=-90
// x=45, y=450ならば，return=45
// x=45, y=-45ならば，return=-90
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
// %=END

// 1周をmodで表現するような角度の単位系で，
// 角度xから角度yへ向きを変えるとき，xに加算するべき値
inline double diffangle(double x, double y, double mod) {
  double a1 = fmod(y - x, mod), a2 = fmod(y - x + mod, mod), a3 = fmod(y - x - mod, mod);
  return abs(a2) < abs(a1) ? (abs(a3) < abs(a2) ? a3 : a2) : (abs(a3) < abs(a1) ? a3 : a1);
}
