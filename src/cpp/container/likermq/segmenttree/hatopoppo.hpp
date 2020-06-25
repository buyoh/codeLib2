// %title
// 遅延セグメント木(bitset風味)
//
// %overview
// 次のクエリを処理できる．
// - 区間の要素をすべて1にする
// - 区間の要素をすべて0にする．
// - 1になっている区間の要素の個数を求める．
// 0-indexedで，[begin,end)．beginを含み，endを含まない．
// 反転クエリも書ける。
//
// %usage
//
// %require
// ```
#include <vector>
using namespace std;
// ```
// %verified
// 乱数テスト
//
// %references
//
//

struct SegmentTree {
  struct LZ {
    bool enable;
    int fillval;
  };
  vector<int> data;
  vector<LZ> lazy;
  int n;

  SegmentTree(int _n) {
    n = 8;
    while (n < _n)
      n <<= 1;
    data.resize(n << 1);
    lazy.resize(n << 1);
  }

  inline void _setup_lazy(int idx, int fillval) {
    if (idx < n) {
      lazy[idx * 2].enable = true;
      lazy[idx * 2].fillval = fillval;
      lazy[idx * 2 + 1].enable = true;
      lazy[idx * 2 + 1].fillval = fillval;
    }
  }

  inline void _apply_lazy(int idx, int size) {
    if (lazy[idx].enable) {
      lazy[idx].enable = false;
      data[idx] = size * lazy[idx].fillval;
      if (idx < n) {
        _setup_lazy(idx, lazy[idx].fillval);
      }
    }
  }

  void _set_range(int begin, int end, int idx, int left, int right) {
    _apply_lazy(idx, right - left);
    if (end <= left || right <= begin)
      return;  // out of range
    if (begin <= left && right <= end) {
      data[idx] = right - left;
      _setup_lazy(idx, 1);
      return;
    }
    int center = (right + left) / 2;
    _set_range(begin, end, idx * 2, left, center);
    _set_range(begin, end, idx * 2 + 1, center, right);
    data[idx] = data[idx * 2] + data[idx * 2 + 1];
  }

  inline void set_range(int begin, int end) { return _set_range(begin, end, 1, 0, n); }

  void _reset_range(int begin, int end, int idx, int left, int right) {
    _apply_lazy(idx, right - left);
    if (end <= left || right <= begin)
      return;  // out of range
    if (begin <= left && right <= end) {
      data[idx] = 0;
      _setup_lazy(idx, 0);
      return;
    }
    int center = (right + left) / 2;
    _reset_range(begin, end, idx * 2, left, center);
    _reset_range(begin, end, idx * 2 + 1, center, right);
    data[idx] = data[idx * 2] + data[idx * 2 + 1];
  }

  inline void reset_range(int begin, int end) { return _reset_range(begin, end, 1, 0, n); }

  inline int _count_range(int begin, int end, int idx, int left, int right) {
    if (end <= left || right <= begin)
      return 0;  // out of range
    _apply_lazy(idx, right - left);
    if (begin <= left && right <= end)
      return data[idx];

    int center = (right + left) / 2;
    return _count_range(begin, end, idx * 2, left, center) +
           _count_range(begin, end, idx * 2 + 1, center, right);
  }

  inline int count_range(int begin, int end) { return _count_range(begin, end, 1, 0, n); }
};
