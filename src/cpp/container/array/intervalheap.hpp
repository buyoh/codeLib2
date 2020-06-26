#ifndef SRC_CPP_CONTAINER_ARRAY_INTERVALHEAP_HPP__
#define SRC_CPP_CONTAINER_ARRAY_INTERVALHEAP_HPP__
// %=BEGIN
// %title
// IntervalHeap
//
// %overview
// 両端優先キュー．
// Heapの各ノードは1~2つの要素を持つ．この2つの要素は順序関係によって区間とみなすことができる．
// 子⊆親を満たすようにHeapを保つことで，data_.front()には常に最小値と最大値が格納される．
//
// うーん相当無駄な実装が多いような．
//
// %usage
// push
// min
// max
// popmin
// popmax
// size
// clear
// reserve
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
// https://topcoder.g.hatena.ne.jp/spaghetti_source/20121006/1349491389
// %=END

template <typename T>
class IntervalHeap {
  using TT = pair<T, T>;  // first <= second

  vector<TT> data_;  // data_の配列要素は2つの要素を持つ．data_.size() は (size_ + 1) / 2 と等価
  int size_;  // データ構造が持つ要素の数

  // data_[ptr]は1つの要素しか持っていない？
  inline bool one(int ptr) const noexcept {
    // ptr == 末尾 かつ 末尾を1要素しか持たない
    return ptr == (size_ - 1) / 2 && (size_ & 1) == 1;
  }

  void fixup(int ptr) {
    if (ptr <= 0)
      return;
    // ptr == 末尾 かつ 末尾を1要素しか持たない場合
    if (one(ptr)) {
      int parent = (ptr - 1) / 2;
      if (data_[ptr].first < data_[parent].first)
        swap(data_[ptr].first, data_[parent].first);  // 左端
      else if (data_[ptr].first > data_[parent].second)
        swap(data_[ptr].first, data_[parent].second);  // 右端
      ptr = parent;
    }
    while (ptr > 0) {
      int parent = (ptr - 1) / 2;
      if (data_[ptr].first < data_[parent].first)
        swap(data_[ptr].first, data_[parent].first);  // 左端
      if (data_[ptr].second > data_[parent].second)
        swap(data_[ptr].second, data_[parent].second);  // 右端
      ptr = parent;
    }
  }

  void fixdown(int ptr) {
    // ptr == 末尾 かつ 末尾を1要素しか持たない場合
    if (one(ptr))
      return;

    // ptrの整合
    if (data_[ptr].first > data_[ptr].second)
      swap(data_[ptr].first, data_[ptr].second);

    int child = ptr * 2 + 1;       // 左の子のid
    if (child >= (size_ + 1) / 2)  // 左の子が居ない
      return;

    if (child + 1 >= (size_ + 1) / 2) {  // 左の子しか居ない
      if (data_[child].first < data_[ptr].first)
        swap(data_[child].first, data_[ptr].first);  // 左端
      if (!one(child) && data_[child].second > data_[ptr].second)
        swap(data_[child].second, data_[ptr].second);  // 右端
      // childの整合
      if (!one(child) && data_[child].first > data_[child].second)
        swap(data_[child].first, data_[child].second);
      return;
    }
    bool leftfix = false, rightfix = false;
    if (data_[child].first < data_[child + 1].first) {  // 左の子が右の子より小さい
      if (data_[child].first < data_[ptr].first)
        swap(data_[child].first, data_[ptr].first);  // 左端
      leftfix = true;
    } else {  // 右の子が左の子より小さい
      if (data_[child + 1].first < data_[ptr].first)
        swap(data_[child + 1].first, data_[ptr].first);  // 左端
      rightfix = true;
    }
    if (one(child + 1) ||
        data_[child].second >
            data_[child + 1]
                .second) {  // 左の子が右の子より大きい または 右の子は1要素しかもっていない
      if (data_[child].second > data_[ptr].second)
        swap(data_[child].second, data_[ptr].second);  // 右端
      leftfix = true;
    } else {  // 右の子が左の子より大きい
      if (data_[child + 1].second > data_[ptr].second)
        swap(data_[child + 1].second, data_[ptr].second);  // 右端
      rightfix = true;
    }
    if (leftfix)
      fixdown(child);
    if (rightfix)
      fixdown(child + 1);
  }

 public:
  inline IntervalHeap() : size_(0) {}

  inline void reserve(size_t s) { data_.reserve(s); }
  inline size_t size() const noexcept { return size_; }
  inline void clear() {
    size_ = 0;
    data_.clear();
  }

  // TODO forward
  void push(const T& elem) {
    // 1. とりあえず末尾に追加
    if ((size_ & 1) == 0) {
      data_.emplace_back(elem, T());
    } else {
      data_.back().second = elem;
      if (data_.back().first > data_.back().second)
        swap(data_.back().first, data_.back().second);
    }
    ++size_;
    // 2. 整合
    fixup((size_ - 1) / 2);
  }

  const T& min() const { return data_.front().first; }

  const T& max() const { return size_ <= 1 ? data_.front().first : data_.front().second; }

  void popmin() {
    if (size_ <= 0) {
      //
    } else if (size_ == 1) {
      --size_;
      data_.pop_back();
    } else if (size_ == 2) {
      --size_;
      swap(data_.front().first, data_.front().second);
    } else if ((size_ & 1) == 0) {
      // 現在偶数個あり，奇数個に減る
      // 1. 末尾と交換
      swap(data_.front().first, data_.back().second);
      // 2. delete
      --size_;
      // 3. 整合
      fixdown(0);
    } else {
      // 現在奇数個あり，偶数個に減る
      // 1. 末尾と交換
      swap(data_.front().first, data_.back().first);
      data_.pop_back();
      // 2. delete
      --size_;
      // 3. 整合
      fixdown(0);
    }
  }

  void popmax() {
    if (size_ <= 0) {
      //
    } else if (size_ == 1) {
      --size_;
      data_.pop_back();
    } else if (size_ == 2) {
      --size_;
    } else if ((size_ & 1) == 0) {
      // 現在偶数個あり，奇数個に減る
      // 1. 末尾と交換
      swap(data_.front().second, data_.back().second);
      // 2. delete
      --size_;
      // 3. 整合
      fixdown(0);
    } else {
      // 現在奇数個あり，偶数個に減る
      // 1. 末尾と交換
      swap(data_.front().second, data_.back().first);
      data_.pop_back();
      // 2. delete
      --size_;
      // 3. 整合
      fixdown(0);
    }
  }
};
#endif  // SRC_CPP_CONTAINER_ARRAY_INTERVALHEAP_HPP__
