#ifndef SRC_CPP_PREDICATOR_DATASTRUCTURE_LINEAR_LEAST_SQUARE_HPP__
#define SRC_CPP_PREDICATOR_DATASTRUCTURE_LINEAR_LEAST_SQUARE_HPP__
// %=BEGIN DOC
// %title
// 線形式の最小二乗法
//
// %overview
// n変数一次方程式で表されるモデルを仮定する。
// 各変数に掛かる係数が不明であるとする。
// q回の計測を行うと、各変数に掛かる係数を変数とする
// q個の方程式からなるn変数連立一次方程式ができる。
// この記事の関数は、上の連立方程式を入力として、
// 係数を推測するための連立一次方程式を出力する。
// %usage
//
//
// %require
// ```
#include "src/cpp/math/datastructure/matrix.hpp"
using namespace std;
// ```
//
// %verified
//
// %references
// %=END DOC
// %=BEGIN CODE
pair<Matrix<double>, Matrix<double>> linearLeastSquare(const Matrix<double>& X,
                                                       const Matrix<double>& Y) {
  assert(Y.width_ == 1);
  assert(Y.height_ == X.width_);
  auto bb = X.transposed();
  bb *= Y;
  auto aa = X.transposed();
  aa *= X;
  return make_pair(move(aa), move(bb));
}
// %=END CODE
#endif  // SRC_CPP_PREDICATOR_DATASTRUCTURE_LINEAR_LEAST_SQUARE_HPP__
