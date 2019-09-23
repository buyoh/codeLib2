// %title
// trie tree
//
// %overview
// 文字列(あるいは配列)の集合を管理する．
// map<string,int> みたいなイメージ
//
// %usage
// TVAL& operator()(IBEGIN begin, IEND end)
// 配列[begin,end)に対応するTVALの参照を返す
//
// %require
// ```
#include <map>
using namespace std;
// ```
// %verified
// (旧) : http://yukicoder.me/submissions/123054
// %references
// プログラミングチャレンジブック

template <typename TKEY = char, typename TVAL = int>
struct TrieTree {
  TVAL value;
  map<TKEY, TrieTree> trie;
  TrieTree(TVAL v) : value(v) {}
  TrieTree() : value(0) {}

  template <typename IBEGIN, typename IEND>
  TVAL& operator()(IBEGIN begin, IEND end) {
    auto begin_succ = begin;
    return (begin == end) ? value : trie[*begin](++begin_succ, end);
  }
};
