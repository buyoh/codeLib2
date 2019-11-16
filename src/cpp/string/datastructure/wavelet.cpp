// %title
// Wavelet Tree (Wavelet Matrix)
//
// %overview
// 試作品．メソッドは殆ど実装していない．
// 完備辞書の代わりにBITを使っている
//
// %usage
//
// %require
// ```
#include <vector>
#include <string>
#include "src/cpp/container/likermq/bit.cpp"
using namespace std;
// ```
//
// %verified
//
// %references
// 岡之原大輔，高速文字列解析の世界，岩波書店．

class Wavelet {
 public:
  const int kChrSizeLg = 5;
  const int kChrSize = 1 << kChrSizeLg;

  const int size_;
  vector<Bitree<int>> tree_;
  vector<int> zerocnt_;

  void build(const vector<char>& _raw) {
    tree_.reserve(kChrSizeLg);
    for (int i = 0; i < kChrSizeLg; ++i)
      tree_.emplace_back(size_);
    zerocnt_.resize(kChrSizeLg);

    vector<char> stac = _raw;
    vector<char> stac0, stac1;
    stac0.reserve(size_);
    stac1.reserve(size_);

    for (int d = 0; d < kChrSizeLg; ++d) {
      stac0.clear();
      stac1.clear();
      int cnt = 0;
      for (char c : stac) {
        int bit = (c & (1 << d)) >> d;
        tree_[d].add(cnt + 1, bit);
        if (bit)
          stac1.push_back(c);
        else
          stac0.push_back(c);
        ++cnt;
      }
      stac.clear();
      stac.insert(stac.end(), stac0.begin(), stac0.end());
      stac.insert(stac.end(), stac1.begin(), stac1.end());
      zerocnt_[d] = stac0.size();
    }
  }

  // void dump() {
  //   for (int d = 0; d < kChrSizeLg; ++d) {
  //     for (int i = 0; i < size_; ++i)
  //       cout << tree_[d].get(i + 1);
  //     cout << endl;
  //   }
  // }

  Wavelet(const vector<char>& _raw) : size_(_raw.size()) { build(_raw); }

  char get(int i) {
    char data = 0;
    for (int d = 0; d < kChrSizeLg; ++d) {
      char b = tree_[d].get(i + 1);  // 0 or 1
      data |= b << d;
      i = b ? zerocnt_[d] + tree_[d].sum(i) : i - tree_[d].sum(i);
    }
    return data;
  }

  // [0,i)の文字cの出現回数
  int rank(int i, char c) { return 0; }  // TODO:
};
