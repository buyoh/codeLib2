#ifndef SRC_CPP_IMPLEMENTS_DATASTRUCTURE_TAG_HPP__
#define SRC_CPP_IMPLEMENTS_DATASTRUCTURE_TAG_HPP__
// %=BEGIN DOC
// %title
// Tag
//
// %overview
// std::pairに似ているが，secondは比較の対象にならない．
// Tag.firstが等価であれば，構造体も等価であるとみなされる．
// マラソンマッチ用
//
// %usage
// %require
// ```
#include <utility>
using namespace std;
// ```
// %verified
// %references
// %words
// kyopuro
// %=END DOC
// %=BEGIN CODE

template <typename T1, typename T2>
struct Tag {
  T1 first;
  T2 second;

  Tag(T1 t1, T2 t2) : first(t1), second(t2) {}
  Tag(const pair<T1, T2>& p) : first(p.first), second(p.second) {}
  Tag(const pair<T1, T2>&& p) : first(p.first), second(p.second) {}

  inline bool operator==(const Tag& t) const { return first == t.first; }
  inline bool operator<(const Tag& t) const { return first < t.first; }
};
// %=END CODE
#endif  // SRC_CPP_IMPLEMENTS_DATASTRUCTURE_TAG_HPP__
