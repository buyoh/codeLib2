// %title
// 二分探索（実数）
// 
// %overview
// 境界を探す．func(x)==trueとなるxの中で，最も小さなxを探す．
// テンプレートを使うと，ラムダを引数に持てなくなるので，typedefで何とかしてある．[TODO]
// functionでwrapすれば済むだけの話だが，競技では面倒なので．
//
// %usage
// bsearch_t binarySearch(bsearch_t low, bsearch_t high, function<bool(bsearch_t)> func)
// low  : 探索区間．型bsearch_tは整数．区間はlow,highを含む
// high : 
// func : ある定数tを持ち，(t<=x)を返す関数func(x)
// @ret : 
//
// %verified
// 
// %references
//


const double EPS = 1e-9;
double binarySearchReal(double false_value, double true_value, function<bool(double)> func) {
    for (int i = 0; i < 99; ++i) {
        double mid = (true_value + false_value) / 2;
        (func(mid) ? true_value : false_value) = mid;
    }
    return true_value;
}

