// %title
// セグメント木(区間加算，区間最大，単独書換)[deprecated]
//
// %overview
// 次のクエリを処理できる．
// - １つの要素を書き換える．
// - 区間の要素すべてに加算する．
// - 区間の要素の最大値を計算する．
// 0-indexedで，[begin,end)．beginを含み，endを含まない．
// 何度も加減算を繰り返すと内部でオーバーフローを起こす可能性．
// インデックスは size_t ではなく int で扱う．
// コピーを繰り返すため，T に指定する型はプリミティブ型か小さな構造体だと嬉しい
//
// %usage
// SegmentTree<typename T>(int n)
// ; [0,n) の配列を確保する．
// T SegmentTree::get_val(int idx)
// ; idx の要素の値を取得する．
// void SegmentTree::set_val(int idx, T e)
// ; idx の要素の値をeに書き換える．
// void SegmentTree::add_valrange(int begin, int end, T e)
// ; 区間[begin,end)にeを加算する
// void SegmentTree::get_maxrange(int idx, T e)
// ; 区間[begin,end)の最大値を計算する
// void SegmentTree::get_maxrangeIdx(int idx, T e)
// ; 区間[begin,end)の最大値が存在する要素のインデックスを取得する
//
// %require
// ```
#include <vector>
using namespace std;
// ```
// %verified
// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=3208410#1 (setval, getminrange)
// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=3208412#1 (addvalrange, getval)
// https://yukicoder.me/submissions/294581 (addvalrange, getminrange)
// Legacy:
// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2521389#1 (setval, getminrange)
// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2521434#1 (addvalrange, getval)
// https://yukicoder.me/submissions/227863 (addvalrange, getminrange)
//
// %references
// プログラミングコンテストチャレンジブック
// https://www.slideshare.net/kazumamikami1/ss-15160153

template <typename T>
// using T = ll;
class SegmentTree {
 public:
  struct IdxVal {
    int index;
    T value;
    inline IdxVal(int _i = 0, const T& _v = T()) : index(_i), value(_v) {}
    inline void set(int i, const T& v) {
      index = i;
      value = v;
    }
    inline bool operator<(const T& t) const { return value < t; }
    inline bool operator==(const T& t) const { return value == t; }
    inline bool operator<(const IdxVal& iv) const { return value < iv.value || (value == iv.value && index < iv.index); }
    inline bool operator==(const IdxVal& iv) const { return index == iv.index && value == iv.value; }
  };

  struct Node {
    T add;
    IdxVal max;
  };

 private:
  const int size_;
  vector<T> leaf_data_;
  vector<Node> node_data_;
  T* leaf_;
  Node* node_;

  void _resize() {
    leaf_data_.resize(size_);
    node_data_.resize(size_ - 1);

    leaf_ = &leaf_data_[0];
    node_ = &node_data_[0] - 1;
  }

  void _init() {
    for (int i = size_ - 1; 0 < i; --i) {
      int c = i << 1;
      if (size_ <= c)
        node_[i].max.set(c - size_, leaf_[c - size_]);
      else
        node_[i].max = node_[c].max;
      node_[i].add = 0;
    }
  }

 public:
  inline void fill(T val) {
    std::fill(leaf_data_.begin(), leaf_data_.end(), val);
    _init();
  }

  SegmentTree(int _n)
      : size_([](int _n) {
          int s = 8;
          while (s < _n)
            s <<= 1;
          return s;
        }(_n)) {
    _resize();
    _init();
  }
  SegmentTree(int _n, const T& _fillVal)
      : size_([](int _n) {
          int s = 8;
          while (s < _n)
            s <<= 1;
          return s;
        }(_n)) {
    _resize();
    fill(_fillVal);
  }

  // idx の要素の値を取得する
  inline T getValue(int index) const {
    T sum = leaf_[index];
    index += size_;
    while (1 < index) {
      index = index >> 1;  // 親へ移動
      sum += node_[index].add;
    }
    return sum;
  }

  // idx の要素の値をeに書き換える
  void setValue(int index, const T e) {
    leaf_[index] = e;

    for (int i = index + size_; 1 < i;) {
      i >>= 1;
      leaf_[index] -= node_[i].add;
    }
    // bottom nodes
    {
      int c1 = index >> 1 << 1;
      int c2 = index >> 1 << 1;
      if (leaf_[c1] < leaf_[c2])
        node_[(index + size_) >> 1].max.set(c2, leaf_[c2]);
      else
        node_[(index + size_) >> 1].max.set(c1, leaf_[c1]);
    }
    for (int i = (index + size_) >> 1; 1 < i;) {
      i >>= 1;
      if (node_[i << 1].max.value < node_[(i << 1) + 1].max.value)
        node_[i].max = node_[(i << 1) + 1].max;
      else
        node_[i].max = node_[i << 1].max;
    }
  }

 private:
  void _addValueRange(int begin, int end, const T e, int ptr, int rangebegin, int rangeend) {
    if (rangeend <= begin || end <= rangebegin)
      return;
    if (begin <= rangebegin && rangeend <= end) {
      if (size_ <= ptr)
        leaf_[ptr - size_] += e;
      else
        node_[ptr].add += e, node_[ptr].max.value += e;
      return;
    }

    _addValueRange(begin, end, e, ptr << 1, rangebegin, (rangebegin + rangeend) >> 1);
    _addValueRange(begin, end, e, (ptr << 1) + 1, (rangebegin + rangeend) >> 1, rangeend);

    if (size_ <= ptr << 1) {
      int i = (ptr << 1) - size_;
      if (leaf_[i] < leaf_[i + 1])
        node_[ptr].max.set(i + 1, leaf_[i + 1]);
      else
        node_[ptr].max.set(i, leaf_[i]);
    } else {
      if (node_[ptr << 1].max.value < node_[(ptr << 1) + 1].max.value)
        node_[ptr].max = node_[(ptr << 1) + 1].max;
      else
        node_[ptr].max = node_[ptr << 1].max;
    }
    node_[ptr].max.value += node_[ptr].add;
  }

 private:
  IdxVal _getMaxRange(int begin, int end, int ptr, int rangebegin, int rangeend) const {
    if (rangeend <= begin || end <= rangebegin)
      return IdxVal(-1);
    if (begin <= rangebegin && rangeend <= end) {
      if (size_ <= ptr)
        return IdxVal(rangebegin, leaf_[rangebegin]);
      else
        return node_[ptr].max;
    }

    auto l = _getMaxRange(begin, end, ptr << 1, rangebegin, (rangebegin + rangeend) >> 1);
    auto r = _getMaxRange(begin, end, (ptr << 1) + 1, (rangebegin + rangeend) >> 1, rangeend);

    if (l.index == -1)
      return move(r);
    if (r.index == -1)
      return move(l);
    return l.value < r.value ? move(r) : move(l);
  }

 public:
  // 区間[begin,end)にeを加算する
  inline void addValueRange(int begin, int end, const T e) { _addValueRange(begin, end, e, 1, 0, size_); }

  // 区間[begin,end)の最大値を計算する
  inline IdxVal getMaxRange(int begin, int end) const {
    if (end - begin <= 1)
      return IdxVal(begin, getValue(begin));
    return _getMaxRange(begin, end, 1, 0, size_);
  }
};
