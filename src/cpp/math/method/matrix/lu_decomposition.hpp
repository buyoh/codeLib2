// %=BEGIN DOC
// %title
// LU decomposition
//
// %overview
// LU 分解をする
// determinant (行列式)も計算する。
// 0除算により失敗するときは空を返す。
// det が 0 になるとき必ず decomposition が失敗するとは限らない。
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
class LUDecomposition {
 public:
  const size_t n_;
  vector<int> pivot_;
  vector<T> elem_;
  T det_;
  LUDecomposition(size_t n) : n_(n), det_(1){};

 public:
  bool valid() const { return n_ > 0; }
  static LUDecomposition decompose(Matrix<T> mat) {
    constexpr double eps = 1e-9;
    // 引数matはコピー
    assert(mat.height_ == mat.width_);
    LUDecomposition lu(mat.height_);
    const int n = mat.height_;

    vector<int> pv;
    pv.reserve(n);  // TODO: pivot_だけにする
    lu.pivot_.resize(n);
    for (int i = 0; i < n; ++i)
      lu.pivot_[i] = i;

    for (int i = 0; i < n; ++i) {
      int p = i;
      for (int j = i + 1; j < n; ++j)
        if (abs(mat(lu.pivot_[j], i)) > abs(mat(lu.pivot_[p], i)))
          p = j;
      if (p != i) {
        swap(lu.pivot_[p], lu.pivot_[i]);
        lu.det_ *= -1;
      }
      pv.push_back(p);

      for (int j = i + 1; j < n; ++j) {
        if (abs(mat(lu.pivot_[i], i)) <= eps)
          return LUDecomposition(0);
        mat(lu.pivot_[j], i) /= mat(lu.pivot_[i], i);
        for (int k = i + 1; k < n; ++k) {
          mat(lu.pivot_[j], k) -= mat(lu.pivot_[i], k) * mat(lu.pivot_[j], i);
        }
        lu.elem_.push_back(mat(lu.pivot_[j], i));
      }
    }
    for (int i = n - 1; i >= 0; --i) {
      for (int j = i + 1; j < n; ++j)
        lu.elem_.push_back(mat(lu.pivot_[i], j));
      auto t = mat(lu.pivot_[i], i);
      lu.elem_.push_back(t);
      lu.det_ *= t;
    }
    lu.pivot_ = move(pv);
    return lu;
  }
  template <typename V>
  V solve(V v) const {
    assert(valid());
    int c = 0;
    const int n = n_;
    for (int i = 0; i < n; ++i) {
      swap(v[pivot_[i]], v[i]);
      for (int j = i + 1; j < n; ++j) {
        v[j] -= v[i] * elem_[c++];
      }
    }
    for (int i = n - 1; i >= 0; --i) {
      for (int j = i + 1; j < n; ++j)
        v[i] -= v[j] * elem_[c++];
      v[i] /= elem_[c++];
    }
    return v;
  }
  T det() const { return det_; }
};