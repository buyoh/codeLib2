// %=BEGIN
// %title
// 浮動小数点の高速入出力
//
// %overview
// やるだけ
//
// %usage
// output_float<double>(M_PI, 6);
// ; piを小数点以下6桁までoutput
//
// %require
// ```
#include "src/cpp/implements/datastructure/io/stdio.hpp"
// ```
// %verified
// https://yukicoder.me/submissions/285892
// %words
// io
// %=END

template <typename T>
void output_float(T var, int d) noexcept {
  if (var < 0)
    putchar_unlocked('-'), var = -var;
  T i = floor(var);
  printer << (long long)i;
  var -= i;
  putchar_unlocked('.');
  while (d-- > 0 && var > 0) {
    var *= 10.0;
    int i = floor(var);
    putchar_unlocked('0' + i);
    var -= i;
  }
}
