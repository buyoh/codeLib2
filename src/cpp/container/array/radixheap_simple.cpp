// %title
// RadixHeap(simple)
//
// %overview
// priority_queueに次の制約を与えたもの
// - 最後に取り出した値より小さな値を追加出来ない
// 計算量はならし O(logD) (D:=max data_value) らしい
//
// %usage
//
// %require
// ```
#include <vector>
#include <cassert>
#include <type_traits>
#include <algorithm>
#include <limits>
using namespace std;
// ```
// %verified
// todo
//
// %references
// https://www.slideshare.net/yosupo/ss-46612984

class RadixHeap {
 public:
  using value_type = long long;

 private:
  value_type last_;
  int size_;
  int lastcnt_ = 0;
  vector<value_type> vec[64];

  // 一番左のbitの位置
  static inline int bsr(value_type x) noexcept {
#ifdef __GNUC__
    return 63 - __builtin_clz(x);  // x == 0 の時，未定義
#else
    int r = -1, i = 0;
    while (x)
      x >>= 1, r = i++;
    return r;
#endif
  }

 public:
  inline RadixHeap() : last_(0), size_(0) {}

  inline bool empty() const noexcept { return size_ == 0; }
  inline int size() const noexcept { return size_; }

  inline void push(value_type val) {
    assert(last_ <= val);
    ++size_;
    if (val == last_)
      ++lastcnt_;
    else
      vec[bsr(val ^ last_)].push_back(val);
  }

  inline value_type top() {
    if (lastcnt_)
      return last_;
    for (int i = 0; i < 64; ++i)
      if (!vec[i].empty())
        return *min_element(vec[i].begin(), vec[i].end());
    assert(true && "RadixHeap is empty.");
    return numeric_limits<value_type>::max();
  }

  void pop() {
    if (lastcnt_ == 0) {
      for (int i = 0; i < 64; ++i) {
        if (vec[i].empty())
          continue;
        last_ = *min_element(vec[i].begin(), vec[i].end());
        for (auto val : vec[i]) {
          if (val == last_)
            ++lastcnt_;
          else
            vec[bsr(val ^ last_)].push_back(val);
        }
        vec[i].clear();
        break;
      }
    }
    --size_;
    --lastcnt_;
  }
};
