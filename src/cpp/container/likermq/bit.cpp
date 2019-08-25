// %title
// Binary Indexed Tree(単独加算，区間総和)
//
// %overview
// 次のクエリを処理できる．
// - １つの要素を加算・減算する．
// - 区間の要素の総和を計算する．
// 1-indexedの実装に注意．
// 何度も加減算を繰り返すと内部でオーバーフローを起こす可能性．
//
// %usage
// Bitree<typename T>(int n)
// ; [1..n] の配列を確保する．
// T Bitree::sum(int r)
// ; [1..r]の範囲の値の和を求める．
// T Bitree::add(int idx, T val)
// ; idx の要素の値をval増やす
// ; 1-indexed．
//
// %verified
// 二分探索が未だ出来ていない
//
// %references
// プログラミングコンテストチャレンジブック

template <typename T = ll>
class Bitree {
  const int size_;
  vector<T> data_;
  static int nlz(uint32_t x) {
    union {
      uint32_t i;
      float f;
    } data;
    data.f = (float)x + 0.5;
    return 158 - (data.i >> 23);
  }

 public:
  Bitree(int size_) : size_(size_), data_(size_ + 1) {}

  // 1..rの範囲の値の和を求める．
  T sum(int r) const {
    T s = 0;
    while (r) {
      s += data_[r];
      r -= r & -r;  //  r&-r と書くことで最下のビットを得る
    }
    return s;
  }
  // l..rの範囲の値の和を求める
  inline T sum(int l, int r) const { return sum(r) - sum(l - 1); }
  // i番目の値を求める(1index)
  T get(int i) const {
    int j = i - 1;
    T s = 0;
    while (i != j) {
      s += data_[i] - data_[j];
      i -= i & -i;
      j -= j & -j;
    }
    return s;
  }
  // idxの要素の値をval増やす
  void add(int idx, T val) {
    while (idx <= size_) {
      data_[idx] += val;
      idx += idx & -idx;
    }
  }
  // sum(i)がvalを超える最初のiを返す
  // get(j)<0となるjが存在すると正しく動作しない
  int upper_bound(T val) const {
    int li = 1 << (31 - nlz(size_));
    int p = 0;
    T d = 0;
    while (li && p < size_) {
      T k = data_[p | li];
      if (!(val < d + k))
        p |= li, d += k;
      li >>= 1;
    }
    if (p > size_)
      p = size_;
    return p + 1;
  }
  // sum(i)がval以上である最初のiを返す
  // get(j)<0となるjが存在すると正しく動作しない
  int lower_bound(T val) const {
    int li = 1 << (31 - nlz(size_));
    int p = 0;
    T d = 0;
    while (li && p < size_) {
      T k = data_[p | li];
      if (d + k < val)
        p |= li, d += k;
      li >>= 1;
    }
    if (p > size_)
      p = size_;
    return p + 1;
  }
};
