#include "test/common/testutil.hpp"
#include "src/cpp/math/method/matrix/lu_decomposition.hpp"

int main() {
  repeat(_, 1000) {
    Matrix<double> m(3, 3);
    Matrix<double> v(3, 1);
    Matrix<double> a(3, 1);
    repeat(i, 3) repeat(j, 3) m(i, j) = Rand::i<int>(-10, 10);
    repeat(i, 3) v[i] = Rand::i<int>(-10, 10);
    repeat(i, 3) repeat(j, 3) a[i] += m(i, j) * v[j];
    double det = m(0, 0) * m(1, 1) * m(2, 2) + m(0, 1) * m(1, 2) * m(2, 0) +
                 m(0, 2) * m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1) * m(0, 2) -
                 m(2, 1) * m(1, 2) * m(0, 0) - m(2, 2) * m(1, 0) * m(0, 1);
    auto lu = LUDecomposition<double>::decompose(m);
    if (!lu.valid()) {
      CHK(abs(det) < 1e-6);
      continue;
    }
    CHK(abs(lu.det() - det) < 1e-6);
    if (abs(det) < 1e-6) {
      continue;
    };
    auto v2 = lu.solve(a);
    // cout << m << endl;
    // cout << a << endl;
    // cout << v  << endl;
    // cout << v2 << endl;
    repeat(i, 3) {
      //
      assert(abs(v[i] - v2[i]) < 1e-6);
    }
  }
  return 0;
}
