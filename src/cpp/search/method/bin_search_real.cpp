// %title
// 二分探索（実数）
// 
// %overview
// 境界を探す．func(x)==trueとなるxの中で，最も小さなxを探す．
// テンプレートを使うと，ラムダを引数に持てなくなるので，typedefで何とかしてある．[TODO]
// functionでwrapすれば済むだけの話だが，競技では面倒なので．
//
// %usage
// bsearch_t bin_search(bsearch_t low, bsearch_t high, function<bool(bsearch_t)> func)
// low  : 探索区間．型bsearch_tは整数．区間はlow,highを含む
// high : 
// func : ある定数tを持ち，(t<=x)を返す関数func(x)
// @ret : 
//
// %verified
// 
// %references
//

double bin_search_real(double l, double h, function<bool(double)> func) {
    double mid = 1.0;
    double last = 0.0;

    while (abs(last - mid) < EPS) {
        last = mid;
        mid = (l + h) / 2.0;
        (func(mid) ? h : l) = mid;
    }
    return mid;
}

