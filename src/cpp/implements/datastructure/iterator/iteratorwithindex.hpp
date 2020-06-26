// %=BEGIN
// %title
// iterator with index
//
// %overview
// イテレータにインデックスを付け加える．
//
//
// %usage
// vector<int> x = {3,1,4,1,5,9,2,6,5,3,5};
// for (auto p : make_IteratorWithIndex(ALL(x))){
//     cout << p.first << ',' << p.second << endl;
// }
//
// %require
// ```
#include <algorithm>
using namespace std;
// ```
//
// %words
// iterator,index
// %verified
//
// %references
// %=END

template <typename ITER>
class IteratorWithIndex {
  // using ITER = int*;
  const ITER begin_;
  const ITER end_;

 public:
  IteratorWithIndex(ITER _begin, ITER _end) : begin_(move(_begin)), end_(move(_end)) {}
  struct Iterator {
    ITER it;
    int i;
    Iterator(ITER _it, int _i) : it(move(_it)), i(_i) {}
    inline pair<int, decltype(*it)> operator*() const { return pair<int, decltype(*it)>(i, *it); }
    inline Iterator& operator++() {
      ++it, ++i;
      return *this;
    }
    inline auto operator!=(const Iterator& another) const -> decltype(it != another.it) {
      return it != another.it;
    }
  };
  inline Iterator begin() const { return Iterator(begin_, 0); }
  inline Iterator end() const { return Iterator(end_, 0); }
};
template <typename ITER>
inline IteratorWithIndex<ITER> make_IteratorWithIndex(ITER _begin, ITER _end) {
  return IteratorWithIndex<ITER>(_begin, _end);
}
