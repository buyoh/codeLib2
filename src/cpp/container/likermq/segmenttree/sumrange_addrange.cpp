// %title
// セグメント木(区間加算，区間最大，区間総和)[deprecated]
//
// %overview
// 次のクエリを処理できる．
// - 区間の要素すべてに加算する．
// - 区間の要素の最大値を計算する．
// - 区間の要素の総和を計算する．
// 0-indexedで，[begin,end)．beginを含み，endを含まない．
// インデックスは size_t ではなく int で扱う．[TODO]
// 要リファクタリング[TODO]
// 必ずfillを呼んでから使う．
//
// %usage
// getsumrange(range) getval(index) getmaxrange(range)
// addvalrange(range)
//
// %require
// ```
#include <vector>
using namespace std;
// ```
// %verified
// http://yukicoder.me/submissions/170798
// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2521353#1 (addvalrange, getsumrange)
//
// %references
// プログラミングコンテストチャレンジブック
//

template <typename T>
// typedef int T;
class SegmentTree {
  int size;

 public:
  struct VI {
    T val;
    int idx;
    VI(T val = 0, int idx = 0) : val(val), idx(idx) {}

    static inline const VI& max(const VI& a, const VI& b) { return a.val == b.val ? (a.idx < b.idx ? a : b) : (a.val >= b.val ? a : b); }
    static inline const VI& min(const VI& a, const VI& b) { return a.val == b.val ? (a.idx < b.idx ? a : b) : (a.val <= b.val ? a : b); }

    inline bool operator==(const VI& e) const { return val == e.val && idx == e.idx; }
    inline bool operator<(const VI& e) const { return (val < e.val) || (idx > e.idx); }
  };

 private:
  vector<int> mark;
  vector<T> single;            // max(min)遅延伝搬用
  vector<pair<T, T>> addtree;  // sum,lazy
  vector<VI> maxtree;          // val,idx
 public:
  SegmentTree(int n) {
    size = 8;
    while (size < n)
      size <<= 1;

    addtree.resize(size * 2);
    mark.resize(size * 2);
    single.resize(size * 2);
    maxtree.resize(size * 2);
    _flat();
  }

  void _flat() {
    for (int i = 0; i < size; ++i) {
      maxtree[i + size - 1] = VI(0, i);
    }
    for (int i = size - 2; 0 <= i; --i) {
      int c = 2 * i + 1;
      maxtree[i] = maxtree[c];
    }
  }

  // 区間[begin,end)に一様にvalを加算する(private)
  void _addvalrange(int begin, int end, T val, int ptr, int rangebegin, int rangeend) {
    if (rangeend <= begin || end <= rangebegin)
      return;  // note:範囲外
    if (begin <= rangebegin && rangeend <= end) {
      addtree[ptr].first += val * (rangeend - rangebegin);
      addtree[ptr].second += val * (rangeend - rangebegin) / 2;
      mark[ptr] |= 3;
      maxtree[ptr].val += val;
      single[ptr] += val;

      // if (rangeend - rangebegin > 1) { // せっかくだから
      //    _applyDown(ptr);
      //}
      return;
    }
    //_applyDown(ptr);

    int rangemid = (rangebegin + rangeend) / 2;
    _addvalrange(begin, end, val, ptr * 2 + 1, rangebegin, rangemid);
    _addvalrange(begin, end, val, ptr * 2 + 2, rangemid, rangeend);
    addtree[ptr].first += val * (min(rangeend, end) - max(rangebegin, begin));
    maxtree[ptr] = VI::max(maxtree[ptr * 2 + 1], maxtree[ptr * 2 + 2]);
  }
  // 区間[begin,end)に一様にvalを加算する
  inline void addvalrange(int begin, int end, T val) { _addvalrange(begin, end, val, 0, 0, size); }

  inline void _applyDown(int ptr) {
    // if (size - 1 <= ptr) return;
    if (mark[ptr] & 1) {
      mark[ptr] ^= 1;
      int c1 = ptr * 2 + 1;
      int c2 = ptr * 2 + 2;
      addtree[c1].first += addtree[ptr].second;
      addtree[c1].second += addtree[ptr].second / 2;
      mark[c1] |= 1;
      addtree[c2].first += addtree[ptr].second;
      addtree[c2].second += addtree[ptr].second / 2;
      mark[c2] |= 1;
      addtree[ptr].second = 0;
    }
    if (mark[ptr] & 2) {
      mark[ptr] ^= 2;
      int c1 = ptr * 2 + 1;
      int c2 = ptr * 2 + 2;
      maxtree[c1].val += single[ptr];
      single[c1] += single[ptr];
      mark[c1] |= 2;
      maxtree[c2].val += single[ptr];
      single[c2] += single[ptr];
      mark[c2] |= 2;
      single[ptr] = 0;
    }
  }

  inline void _downward_applyDown(int ptr1) {
    if (1 < ptr1)
      _downward_applyDown(ptr1 >> 1);
    _applyDown(ptr1 - 1);
  }

  T getval(int index) {
    T sum = 0;
    _downward_applyDown((index + size) >> 1);
    return addtree[index + size - 1].first;
  }

  // 区間[begin,end)の和を求める(private)
  T _getsumrange(int begin, int end, int ptr, int rangebegin, int rangeend) {
    if (rangeend <= begin || end <= rangebegin)
      return 0;  // note:範囲外
    if (begin <= rangebegin && rangeend <= end)
      return addtree[ptr].first;

    _applyDown(ptr);

    int rangemid = (rangebegin + rangeend) / 2;
    return _getsumrange(begin, end, ptr * 2 + 1, rangebegin, rangemid) + _getsumrange(begin, end, ptr * 2 + 2, rangemid, rangeend);
  }
  // 区間[begin,end)の和を求める
  inline T getsumrange(int begin, int end) { return _getsumrange(begin, end, 0, 0, size); }

  // 区間[begin,end)の最大値とその位置を求める(private)
  VI _getmaxrange(int begin, int end, int ptr, int rangebegin, int rangeend) {
    if (rangeend <= begin || end <= rangebegin)
      return VI(0, -1);  // note:範囲外
    if (begin <= rangebegin && rangeend <= end)
      return maxtree[ptr];

    _applyDown(ptr);

    int rangemid = (rangebegin + rangeend) / 2;
    VI l = _getmaxrange(begin, end, ptr * 2 + 1, rangebegin, rangemid);
    VI r = _getmaxrange(begin, end, ptr * 2 + 2, rangemid, rangeend);

    if (l.idx == -1)
      return r;
    if (r.idx == -1)
      return l;
    return VI::max(l, r);
  }
  // 区間[begin,end)の最大値とその位置を求める
  inline VI getmaxrange(int begin, int end) { return _getmaxrange(begin, end, 0, 0, size); }
};
