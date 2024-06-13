// %title
// 高機能配列
//
// %overview
// 線形時間で区間の総和を計算したり、最大値を計算したりできる、普通の配列。
// テスト用。
//
// %require
// ```
#include <algorithm>
#include <deque>
using namespace std;
#define _all(v) (v).begin(), (v).end()
#define _repeat(cnt, l) for (auto cnt = remove_reference<decltype(l)>::type(); (cnt) < (l); ++(cnt))
#define _rrepeat(cnt, l) for (auto cnt = (l)-1; 0 <= (cnt); --(cnt))
#define _iterate(cnt, b, e) for (auto cnt = (b); (cnt) != (e); ++(cnt))
#define _diterate(cnt, b, e) for (auto cnt = (b); (cnt) != (e); --(cnt))
template <typename T>
inline T& _chmax(T& to, const T& val) {
  return to = max(to, val);
}
template <typename T>
inline T& _chmin(T& to, const T& val) {
  return to = min(to, val);
}
// ```
// %usage
//
// %words
// %verified
// %references

template <typename T>
class Container {
 public:
  deque<T> data_;

  Container(int sz = 1) : data_(sz) {}
  Container(deque<T>&& deq) : data_(move(deq)) {}
  Container(Container&& c) : data_(move(c.data_)) {}

  void fill(const T& x) { std::fill(_all(data_), x); }
  void setValue(int i, const T& x) { data_[i] = x; }
  const T& getValue(int i) const { return data_[i]; }
  T getSumRange(int begin, int end) {
    T t = T();
    _iterate(i, begin, end) t += data_[i];
    return t;
  }
  T getMaxRange(int begin, int end) {
    T t = T();
    _iterate(i, begin, end) _chmax(t, data_[i]);
    return t;
  }
  void setValueRange(int begin, int end, const T& x) { _iterate(i, begin, end) data_[i] = x; }
  void addValueRange(int begin, int end, const T& x) { _iterate(i, begin, end) data_[i] += x; }
  // index番目の要素の手前にvalを挿入する
  void insert(int i, const T& x) {
    auto it = data_.begin();
    advance(it, i);
    data_.insert(it, x);
  }
  void erase(int i) {
    auto it = data_.begin();
    advance(it, i);
    data_.erase(it);
  }
  // 前からsize個を切り出す
  Container<T> split(int s) {
    auto it = data_.begin();
    advance(it, s);
    deque<T> de(data_.begin(), it);
    data_.erase(data_.begin(), it);
    return Container(move(de));
  }
  // 末尾にanotherを結合する
  void concat(const Container<T>& another) { data_.insert(data_.end(), _all(another.data_)); }
};