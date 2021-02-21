#ifndef SRC_CPP_CONTAINER_ARRAY_CYCLEDARRAY_HPP__
#define SRC_CPP_CONTAINER_ARRAY_CYCLEDARRAY_HPP__
// %=BEGIN DOC
// %title
// 巡回する配列
//
// %overview
// 無限に長いが、途中から巡回する規則を見つけられるような
// 列が存在する。A[i+1] = (A[i]*25 + 9)%101 のような。
// このような列を容易に参照するためのデータ構造。
//
// %usage
//
// %require
// ```
#include <vector>
using namespace std;
// ```
// %verified
// rand
//
// %references
//
// %=END DOC
// %=BEGIN CODE

template <typename T>
class CycledVector : std::vector<T> {
  size_t nloop = 0;
  size_t midl = 0;
  size_t cvidx(size_t i) {
    if (i <= midl)
      return i;
    return (i - midl) % nloop + midl;
  }

 public:
  CycledVector() : nloop(0), midl(0) {}
  CycledVector(size_t n) : std::vector<T>(n), nloop(n), midl(0) {}
  CycledVector(const std::vector<T>& v) : std::vector<T>(v), nloop(v.size()), midl(0) {}
  CycledVector(std::vector<T>&& v) : std::vector<T>(v), nloop(v.size()), midl(0) {}
  // 配列上のterm番目の要素とmid番目の要素と共有するような、サイクル状にする
  void set_loop(size_t mid, size_t term) {
    midl = mid;
    nloop = term - mid;
  }
  // warning: midl/nloop is reseted.
  void resize(size_t n) {
    std::vector<T>::resize(n);
    midl = 0;
    nloop = n;
  }
  const T& operator[](size_t i) const { return std::vector<T>::operator[](cvidx(i)); }
  T& operator[](size_t i) { return std::vector<T>::operator[](cvidx(i)); }
  // 累積和の配列とみなして、先頭からiまでの総和を計算する
  // 累積和の配列とは、A[i]がiまでの総和であるようなもので、
  // 通常の累積和とズレている点に注意
  // 別用途との共存の為によるもの
  T sum_as_cum(size_t i) const {
    if (i < midl + nloop)
      return std::vector<T>::operator[](i);
    T cut = T();
    if (0 < midl)
      cut = std::vector<T>::operator[](midl - 1);
    T k = T(i - midl) / T(nloop);
    size_t m = (i - midl) % nloop;
    T block = std::vector<T>::operator[](midl + nloop - 1) - cut;
    T cut2 = std::vector<T>::operator[](midl + m - 1) - cut;
    return cut + block * k + cut2;
  }
};
// %=END CODE
#endif  // SRC_CPP_CONTAINER_ARRAY_CYCLEDARRAY_HPP__
