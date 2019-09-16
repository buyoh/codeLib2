// %title
// RadixHeap
//
// %overview
// priority_queueに次の制約を与えたもの
// - 最後に取り出した値より小さな値を追加出来ない
// 計算量はならし O(logD) (D:=max data_value) らしい
//
// %usage
//
// %verified
// todo
//
// %references
// https://www.slideshare.net/yosupo/ss-46612984

template <typename T>
// using T = int;
class RadixHeap {
 public:
  using value_type = uint64_t;

 private:
  using pair_type = pair<value_type, T>;
  value_type last_;
  int size_;
  int lastcnt_ = 0;
  vector<pair_type> vec[65];

  // 一番左のbitの位置
  inline int bsr(value_type x) {
#ifdef __GNUC__
    return x > 0 ? 63 - __builtin_clz(x) : -1;
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

  inline void push(value_type val, T elem) {
    assert(last_ <= val);
    ++size_;
    vec[bsr(val ^ last_) + 1].emplace_back(val, elem);
  }

  inline const pair_type& top() {
    if (!vec[0].empty())
      return vec[0].front();
    for (int i = 1; i < 65; ++i)
      if (!vec[i].empty())
        return *min_element(vec[i].begin(), vec[i].end(),
                            [](const pair<value_type, T>& l, const pair<value_type, T>& r) {
                              return l.first < r.first;
                            });
    assert(false && "RadixHeap is empty.");
    return vec[0].front();
  }

  void pop() {
    if (vec[0].empty()) {
      for (int i = 1; i < 65; ++i) {
        if (vec[i].empty())
          continue;
        last_ = min_element(vec[i].begin(), vec[i].end(),
                            [](const pair<value_type, T>& l, const pair<value_type, T>& r) {
                              return l.first < r.first;
                            })
                    ->first;
        for (auto& p : vec[i])
          vec[bsr(p.first ^ last_) + 1].push_back(move(p));
        vec[i].clear();
        break;
      }
    }

    --size_;
    vec[0].pop_back();
  }
};