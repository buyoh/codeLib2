// %title
// 二分探索
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
// func : ある定数tを持ち，(t<=x)を返す関数func(x)．二分探索はtを探す．
// @ret : 
//
// %verified
// 
// %references
//


typedef ll bsearch_t;
bsearch_t bin_search(bsearch_t low, bsearch_t high, function<bool(bsearch_t)> func) {
    --low;
    bsearch_t c;
    while (low + 1 < high) {
        c = (low + high) / 2;
        (func(c) ? high : low) = c;
    }
    return high;
}

