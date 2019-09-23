// %title
// SparseTable(区間最小)
//
// %overview
// 構築済みの配列に対して，次のクエリを処理できる．
// - 区間の最小値を計算する．
// 0-indexedで，[begin,end)．beginを含み，endを含まない．
// 何度も加減算を繰り返すと内部でオーバーフローを起こす可能性．
// O(log^2N)．最大値のみの機能なら出来そう．[TODO]
//
// %usage
// SparseTable<typename T>(int n)
// ; [0,n) の配列を確保する．
// T& SparseTable::operator[](size_t i)
// ; i の要素の参照を得る．build後に更新してはならない．
// void SparseTable::build()
// ; クエリに応えられるように準備する．
// void SparseTable::getminrangeIdx(int begin, int end)
// ; 区間[begin,end)の最小値を計算する
//
// %require
// ```
#include <vector>
using namespace std;
// ```
// %verified
// http://yukicoder.me/submissions/172470
//
// %references
// http://tookunn.hatenablog.com/entry/2016/07/13/211148

template <typename T>
class SparseTable {
 public:
  int size;
  const vector<int> log2;
  vector<T> data;
  vector<T> dp;

  SparseTable(int size) : size(size), log2(size + 1), data(size) {
    // for fast calculate log2
    for (int i = 2; i <= size; ++i) {
      log2[i] = log2[i >> 1] + 1;
    }

    dp.resize(size * (log2[size] + 1));
  }

  inline T& operator[](size_t i) { return data[i]; }
  inline T operator[](size_t i) const { return data[i]; }

  void build() {
    int l, i, f, b;
    for (i = 0; i < size; i++) {
      dp[i] = i;
    }
    for (l = 1; (1 << l) <= size; l++) {
      for (i = 0; i + (1 << l) <= size; i++) {
        f = dp[i + size * (l - 1)];
        b = dp[(i + (1 << (l - 1))) + size * (l - 1)];

        dp[i + size * l] = (data[f] <= data[b]) ? f : b;  // minimum
      }
    }
  }

  // range [l,r)
  int getminrangeIdx(int l, int r) const {
    int lg = log2[r - l];
    int i1 = dp[l + size * lg];
    int i2 = dp[r - (1 << lg) + size * lg];
    return (data[i1] <= data[i2]) ? i1 : i2;  // minimum
  }
};
