#ifndef SRC_CPP_IMPLEMENTS_DATASTRUCTURE_HASHES_HPP__
#define SRC_CPP_IMPLEMENTS_DATASTRUCTURE_HASHES_HPP__
// %=BEGIN
// %title
// std::hashの追加定義
//
// %overview
// std::hashに定義を追加することでunordered_setが扱える型を増やす．
// std::pair<T1,T2> のみ
//
// %usage
//
// %require
// ```
#include <unordered_set>
#include <unordered_map>
using namespace std;
// ```
// %verified
//
// %references
// %=END

namespace std {
template <typename T1, typename T2>
struct hash<pair<T1, T2>> {
  constexpr inline size_t operator()(const pair<T1, T2>& x) const {
    return hash<T1>()(x.first) ^ hash<T2>()(x.second);
  }
};
}  // namespace std
#endif  // SRC_CPP_IMPLEMENTS_DATASTRUCTURE_HASHES_HPP__
