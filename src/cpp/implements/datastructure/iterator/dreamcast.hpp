#ifndef SRC_CPP_IMPLEMENTS_DATASTRUCTURE_ITERATOR_DREAMCAST_HPP__
#define SRC_CPP_IMPLEMENTS_DATASTRUCTURE_ITERATOR_DREAMCAST_HPP__
// %=BEGIN DOC
// %title
// ぐるぐるイテレータ
//
// %overview
// 正方形限定
//
//
// %usage
// for (pair<int,int> xy : DreamcastScan(20)){}
//
// %require
// ```
#include <algorithm>
using namespace std;
// ```
// %words
// yuyuko
// %verified
//
// %references
// %=END DOC
// %=BEGIN CODE

struct DreamcastScan {
  const int Width;
  constexpr DreamcastScan(int _w) : Width(_w) {}
  struct Iterator {
    int w;     // width
    int i, o;  // index, offset
    constexpr Iterator(int _w, int _i, int _o) : w(_w), i(_i), o(_o) {}

    inline pair<int, int> operator*() const {
      if (i < w - 1)
        return make_pair(o + i, o);
      else if (i < (w - 1) * 2)
        return make_pair(o + w - 1, o + i - (w - 1));
      else if (i < (w - 1) * 3)
        return make_pair(o + (w - 1) * 3 - i, o + w - 1);
      else
        return make_pair(o, o + (w - 1) * 4 - i);
    }
    inline Iterator& operator++() {
      if (++i >= (w - 1) * 4)
        i = 0, w -= 2, o += 1;
      return *this;
    }
    inline bool operator!=(const Iterator& it) const { return w != it.w || i != it.i; }
  };
  constexpr inline Iterator begin() const { return Iterator(Width, 0, 0); }
  constexpr inline Iterator end() const { return Iterator(-(Width & 1), 0, 0); }
};
// %=END CODE
#endif  // SRC_CPP_IMPLEMENTS_DATASTRUCTURE_ITERATOR_DREAMCAST_HPP__
