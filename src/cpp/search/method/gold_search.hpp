// %title
// 黄金探索[TODO]
//
// %overview
// minimize y = func(x)．ただし，func(x)は下に凸な関数．
// 実数限定．整数にも使えなくはないが，多倍長じゃないと不安．
// テンプレートを使うと，ラムダを引数に持てなくなるので，typedefで何とかしてある．[TODO]
// functionでwrapすれば済むだけの話だが，競技では面倒なので．
// 三分探索で十分．
//
// %usage
// tsearch_t tri_search_minimize(tsearch_t left, tsearch_t right, function<fval_t(tsearch_t)> func)
// left : 探索区間．型tsearch_tは整数．区間は low,high を含む
// right:
// func : 下に凸な関数func(x)
// @ret :
//
// %verified
//
// %references
// http://naoyat.hatenablog.jp/entry/2012/01/04/231801
