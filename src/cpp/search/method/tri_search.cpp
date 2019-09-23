// %title
// 三分探索
//
// %overview
// minimize y = func(x)．ただし，func(x)は下に凸な関数．
// テンプレートを使うと，ラムダを引数に持てなくなるので，typedefで何とかしてある．[TODO]
// functionでwrapすれば済むだけの話だが，競技では面倒なので．
//
// %usage
// tsearch_t ternarySearchMinimize(tsearch_t left, tsearch_t right, function<fval_t(tsearch_t)>
// func) left : 探索区間．型tsearch_tは整数．区間は low,high を含む right: func :
// 下に凸な関数func(x)
// @ret :
//
// %require
// ```
#include <functional>
using namespace std;
// ```
// %verified
//
// %references
//

using tsearch_t = long long;
using fval_t = long long;
tsearch_t ternarySearchMinimize(tsearch_t left, tsearch_t right, function<fval_t(tsearch_t)> func) {
  for (int lop = 0; lop < 60; ++lop) {
    tsearch_t l = right - (2 * right - 2 * left) / 3;
    tsearch_t r = right - (right - left) / 3;
    fval_t lval = func(l);
    fval_t rval = func(r);

    if (lval <= rval)
      right = r;
    else
      left = l;

    if (right - left <= 2)
      break;
  }
  fval_t best = func(left);
  tsearch_t bestidx = left;
  for (tsearch_t t = left + 1; t <= right; ++t) {
    tsearch_t f = func(t);
    if (best > f) {
      best = f;
      bestidx = t;
    }
  }
  return bestidx;
}