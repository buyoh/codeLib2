#ifndef SRC_CPP_CONTAINER_ARRAY_PRIORITYQUEUE_HPP__
#define SRC_CPP_CONTAINER_ARRAY_PRIORITYQUEUE_HPP__
// %=BEGIN DOC
// %title
// PriorityQueue
//
// %overview
// clear, reserveが欲しいだけ
//
// %usage
//
// %require
// ```
#include <vector>
#include <algorithm>
using namespace std;
// ```
// %verified
// rand
//
// %references
//
// %=END DOC
// %=BEGIN CODE

template <typename T>
class PriorityQueue {
  vector<T> data_;

 public:
  inline PriorityQueue() {}

  inline bool empty() const noexcept { return data_.empty(); }
  inline int size() const noexcept { return data_.size(); }
  inline void reserve(int s) { data_.reserve(s); }
  inline void clear() { data_.clear(); }
  inline const T& top() const noexcept { return data_.front(); }
  inline void pop() {
    pop_heap(data_.begin(), data_.end());
    data_.pop_back();
  }
  inline void push(const T& val) {
    data_.push_back(val);
    push_heap(data_.begin(), data_.end());
  }
  inline void push(T&& val) {
    data_.push_back(move(val));
    push_heap(data_.begin(), data_.end());
  }
  template <typename... Arg>
  inline void emplace(Arg&&... args) {
    data_.emplace_back(forward<Arg>(args)...);
    push_heap(data_.begin(), data_.end());
  }
};
// %=END CODE
#endif  // SRC_CPP_CONTAINER_ARRAY_PRIORITYQUEUE_HPP__
