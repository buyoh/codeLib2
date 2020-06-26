// %=BEGIN
// %title
// セグメント木(単独書き換え，区間演算)
//
// %overview
// 次のクエリを処理できる．
// - １つの要素を書き換える．
// - 区間の要素全てにfunc_を適応した値を求める．
// 十分な高速化が施されている．と思われる．
//
// %usage
// ; 非負整数，区間最大値の場合．
// SegmentTreeQ<int> s(10, [](int x, int y) {return max(x, y); }, 0);
//
// %require
// ```
#include <vector>
#include <functional>
using namespace std;
// ```
// %verified
// https://beta.atcoder.jp/contests/dwacon2018-final-open/submissions/2050384
//
// %references
// http://d.hatena.ne.jp/komiyam/20131202/1385992406
// http://proc-cpuinfo.fixstars.com/2017/07/optimize-segment-tree/
// https://beta.atcoder.jp/contests/dwacon2018-final-open/submissions/2049865
// %=END

template <typename T>
// typedef int T;
class SegmentTreeQ {
  int size_;
  vector<T> data_;
  const function<T(T, T)> func_;
  const T zero_;

 public:
  SegmentTreeQ(int n, function<T(T, T)> f, T z) : func_(f), zero_(z) {
    size_ = 8;
    while (size_ < n)
      size_ <<= 1;
    data_.resize(size_ * 2, zero_);
  }

  void fill(T val) { std::fill(data_.begin(), data_.end(), val); }

  inline T get_val(int index) const { return data_[index + size_]; }

  void set_val(int index, const T e) {
    index += size_;
    data_[index] = e;
    while (1 < index) {
      data_[index >> 1] =
          func_(data_[index], data_[index ^ 1]);  // TODO : この部分の計算順序は正確か？
      index >>= 1;
    }
  }

  inline int get_range(int begin, int end) const {
    T rl = zero_, rr = zero_;
    begin += size_;
    end += size_;
    for (; begin < end; begin >>= 1, end >>= 1) {
      if (begin & 1)
        rl = func_(data_[begin++], rl);
      if (end & 1)
        rr = func_(rr, data_[--end]);
    }
    return func_(rl, rr);
  }
};