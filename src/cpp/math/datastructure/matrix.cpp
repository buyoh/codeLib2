// %title
// 行列の基本操作
//
// %overview
// 行列同士の+-*，スカラーとの+-*/，べき乗を実装
//
// %usage
// ;定数配列を指定する時は，
// Matrix<ll> mat(3, 3, {
//    1, 2, 3,
//    3, 1, 2,
//    2, 3, 1
// });
// 
// %require
// ```
#include <valarray>
#include <ostream>
using namespace std;
// ```
//
// %verified
// https://beta.atcoder.jp/contests/arc027/submissions/3095831
// など
// %references

template <typename T>
// using T = double;
class Matrix {
 public:
  size_t height_, width_;
  valarray<T> data_;
  Matrix(size_t height = 1, size_t width = 1)
      : height_(height), width_(width), data_(height * width) {}
  template <typename V>
  Matrix(size_t height, size_t width, const V& data)
      : height_(height), width_(width), data_(data) {}
  Matrix(size_t height, size_t width, initializer_list<T> init)
      : height_(height), width_(width), data_(init) {}

  inline T& operator()(size_t y, size_t x) { return data_[y * width_ + x]; }
  inline T operator()(size_t y, size_t x) const { return data_[y * width_ + x]; }
  inline T& at(size_t y, size_t x) { return data_[y * width_ + x]; }
  inline T at(size_t y, size_t x) const { return data_[y * width_ + x]; }
  inline void resize(size_t h, size_t w) {
    height_ = h;
    width_ = w;
    data_.resize(h * w);
  }
  inline void resize(size_t h, size_t w, T val) {
    height_ = h;
    width_ = w;
    data_.resize(h * w, val);
  }
  inline void fill(T val) { data_ = val; }
  Matrix<T>& setDiag(T val) {
    for (size_t i = 0, en = min(width_, height_); i < en; ++i)
      at(i, i) = val;
    return *this;
  }

  void print(ostream& os) {
    os << "- - -" << endl;  //  << setprecision(3)
    for (size_t y = 0; y < height_; ++y) {
      for (size_t x = 0; x < width_; ++x) {
        os << setw(7) << at(y, x) << ' ';
      }
      os << endl;
    }
  }
};
template <typename T>
inline ostream& operator<<(ostream& os, Matrix<T> mat) {
  mat.print(os);
  return os;
}

template <typename T>
Matrix<T> multiply(const Matrix<T>& mat1, const Matrix<T>& mat2) {
  assert(mat1.width_ == mat2.height_);
  Matrix<T> result(mat1.height_, mat2.width_);
  for (size_t i = 0; i < mat1.height_; ++i)
    for (size_t j = 0; j < mat2.width_; ++j)
      for (size_t k = 0; k < mat1.width_; ++k)
        result(i, j) += mat1(i, k) * mat2(k, j);
  return result;
}
template <typename T, typename V>
V multiply(const Matrix<T>& mat1, const V& vec2) {
  assert(mat1.width_ == vec2.size());
  V result(mat1.height_);
  for (size_t i = 0, j; i < mat1.height_; ++i)
    for (j = 0; j < mat1.width_; ++j)
      result[i] += mat1(i, j) * vec2[j];
  return result;
}

template <typename T>
inline Matrix<T>& operator+=(Matrix<T>& mat, T val) {
  mat.data_ += val;
  return mat;
}
template <typename T>
inline Matrix<T>& operator-=(Matrix<T>& mat, T val) {
  mat.data_ -= val;
  return mat;
}
template <typename T>
inline Matrix<T>& operator*=(Matrix<T>& mat, T val) {
  mat.data_ *= val;
  return mat;
}
template <typename T>
inline Matrix<T>& operator/=(Matrix<T>& mat, T val) {
  mat.data_ /= val;
  return mat;
}
template <typename T>
inline Matrix<T>& operator^=(Matrix<T>& mat, T val) {
  mat.data_ ^= val;
  return mat;
}
template <typename T>
inline Matrix<T>& operator+=(Matrix<T>& mat1, const Matrix<T>& mat2) {
  mat1.data_ += mat2.data_;
  return mat1;
}
template <typename T>
inline Matrix<T> operator+(Matrix<T>& mat1, const Matrix<T>& mat2) {
  return Matrix<T>(mat1.height_, mat1.width_, mat1.data_ + mat2.data_);
}
template <typename T>
inline Matrix<T>& operator-=(Matrix<T>& mat1, const Matrix<T>& mat2) {
  mat1.data_ -= mat2.data_;
  return mat1;
}
template <typename T>
inline Matrix<T> operator-(Matrix<T>& mat1, const Matrix<T>& mat2) {
  return Matrix<T>(mat1.height_, mat1.width_, mat1.data_ - mat2.data_);
}
template <typename T>
inline Matrix<T>& operator*=(Matrix<T>& mat1, const Matrix<T>& mat2) {
  mat1 = multiply(mat1, mat2);
  return mat1;
}
template <typename T>
inline Matrix<T> operator*(const Matrix<T>& mat1, const Matrix<T>& mat2) {
  return multiply(mat1, mat2);
}
template <typename T, typename V>
inline V operator*(const Matrix<T>& mat1, const V& vec2) {
  return multiply(mat1, vec2);
}

template <typename T>
Matrix<T> pow(Matrix<T> a, long long p) {
  assert(a.height_ == a.width_);
  Matrix<T> b(a.height_, a.height_);
  b.setDiag(1);
  while (0 < p) {
    if (p & 1)
      b *= a;
    a *= a;
    p >>= 1;
  }
  return b;
}
