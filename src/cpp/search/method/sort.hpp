#ifndef SRC_CPP_SEARCH_METHOD_SORT_HPP__
#define SRC_CPP_SEARCH_METHOD_SORT_HPP__
// %=BEGIN DOC
// %title
// 並び替えないソート
//
// %overview
// - i番目の要素はソート後何番目の要素になるか？
// - ソート後i番目の要素はソート前何番目の要素だったか？
// を求める
//
// %usage
// void sortedIndex(const ITER begin, const ITER end, vector<int> &idx, vector<int> &idxr)
// ; ITERはランダムアクセスイテレータ
//
// %require
// ```
#include <vector>
using namespace std;
// ```
// %verified
// atcoder
// %references
//
// %=END DOC
// %=BEGIN CODE

// ソートしてi番目の値は 元々idx[i]番目の値
// 元々i番目の値は ソートするとidxr[i]番目の値
template <typename ITER>
void sortedIndex(const ITER begin, const ITER end, vector<int>& idx, vector<int>& idxr) {
  size_t n = end - begin;
  idx.resize(n);
  idxr.resize(n);
  for (int i = 0; i < n; ++i)
    idx[i] = i;
  sort(idx.begin(), idx.end(), [&begin](int l, int r) { return begin[l] < begin[r]; });
  for (int i = 0; i < n; ++i)
    idxr[idx[i]] = i;
}
// %=END CODE
#endif  // SRC_CPP_SEARCH_METHOD_SORT_HPP__
