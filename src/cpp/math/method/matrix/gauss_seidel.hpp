#ifndef __SRC_CPP_MATH_METHOD_MATRIX_GAUSS_SEIDEL_HPP__
#define __SRC_CPP_MATH_METHOD_MATRIX_GAUSS_SEIDEL_HPP__
// %=BEGIN DOC
// %title
// ガウス・ザイデル法
//
// %overview
// 線形連立方程式を反復法で解く
// 行列が対角優位でない場合は収束する保証は無い
// 動作未検証
//
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
template <typename T>
Matrix<T> gauss_seidel(const Matrix<T>& aa, const Matrix<T>& bb, int iter = 100) {
  assert(aa.height_ == aa.width_);
  assert(aa.height_ == bb.height_);
  assert(bb.width_ == 1);
  auto n = bb.height_;
  auto xx = bb;
  repeat(_, iter) {
    repeat(i, n) {
      T total = 0;
      repeat(j, n) {
        if (j != i)
          total += aa(i, j) * xx(j, 0);
      }
      xx(i, 0) = bb(i, 0) - total;
      xx(i, 0) /= aa(i, i);
    }
  }
  return xx;
}
// %=END CODE
#endif  // __SRC_CPP_MATH_METHOD_MATRIX_GAUSS_SEIDEL_HPP__
