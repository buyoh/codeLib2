#ifndef SRC_CPP_STRING_DATASTRUCTURE_SUFFIXARRAY_HPP__
#define SRC_CPP_STRING_DATASTRUCTURE_SUFFIXARRAY_HPP__
// %=BEGIN DOC
// %title
// SuffixArray
//
// %overview
//
// %usage
//
// %require
// ```
#include <vector>
#include <string>
#include <cassert>
using namespace std;
// ```
//
// %verified
// tested
// %references
// 蟻本
// http://wk1080id.hatenablog.com/entry/2018/12/25/005926v
// %=END DOC
// %=BEGIN CODE

class SuffixArray {
  const char* str;
  int size;
  vector<const char*> sa;
  string keeper;

 public:
  SuffixArray() {}
  SuffixArray(const string& d) {
    str = d.c_str();
    size = d.size();
    assert(str[size] == 0);
    ++size;
  }
  SuffixArray(string&& d) {
    keeper = move(d);
    str = keeper.c_str();
    size = keeper.size();
    assert(str[size] == 0);
    ++size;
  }

  void build() {
    const int Alpha = 128;
    vector<int> sfx(size);                  // suffix array permutation
    vector<int> div(size);                  // division
    vector<int> cnt(max(size + 1, Alpha));  // counter

    for (int i = 0; i < size; ++i)
      cnt[str[i]] += 1;
    for (int i = 1; i < Alpha; ++i)
      cnt[i] += cnt[i - 1];
    for (int i = 0; i < size; ++i)
      sfx[--cnt[str[i]]] = i;

    div[sfx[0]] = 0;
    for (int i = 1; i < size; ++i)
      div[sfx[i]] = div[sfx[i - 1]] + (str[sfx[i - 1]] < str[sfx[i]]);

    for (int d = 1; d < size; d *= 2) {
      vector<int> sfx2(size);
      {
        fill(cnt.begin(), cnt.end(), 0);
        for (int i = 0; i < size; ++i)
          cnt[div[(sfx[i] - d + size) % size]] += 1;
        for (int i = 1; i <= size; ++i)
          cnt[i] += cnt[i - 1];
        for (int i = size - 1; 0 <= i; --i) {
          sfx2[--cnt[div[(sfx[i] - d + size) % size]]] = (sfx[i] - d + size) % size;
        }
      }
      sfx2.swap(sfx);
      vector<int> div2(size);
      div2[sfx[0]] = 0;
      for (int i = 1; i < size; ++i) {
        pair<int, int> curr = {div[sfx[i]], div[(sfx[i] + d) % size]};
        pair<int, int> prev = {div[sfx[i - 1]], div[(sfx[i - 1] + d) % size]};
        div2[sfx[i]] = div2[sfx[i - 1]] + (curr != prev);
      }
      div2.swap(div);
    }
    sa.resize(size);
    for (int i = 0; i < size; ++i)
      sa[i] = str + sfx[i];
  }

  pair<bool, pair<int, int>> find(const string& keyword) {
    const int n = keyword.size();

    if (n == 0)
      return make_pair(false, make_pair(0, -1));

    int low = 0;
    int high = sa.size() - 1;

    for (int i = 0; i < n; ++i) {
      int ng, ok;
      ng = low - 1;
      ok = high;
      while (ok - ng > 1) {
        int m = (ok + ng) / 2;
        if (keyword[i] <= *(sa[m] + i))
          ok = m;
        else
          ng = m;
      }
      low = ok;

      ng = high + 1;
      ok = low;
      while (ng - ok > 1) {
        int m = (ok + ng) / 2;
        if (keyword[i] >= *(sa[m] + i))
          ok = m;
        else
          ng = m;
      }
      high = ok;

      if (low > high || keyword[i] != *(sa[low] + i) || keyword[i] != *(sa[high] + i))
        return make_pair(false, make_pair(0, -1));
    }
    return make_pair(true, make_pair(low, high));
  }

  inline int foundToIdx(int found) { return sa[found] - str; }
};
// %=END CODE
#endif  // SRC_CPP_STRING_DATASTRUCTURE_SUFFIXARRAY_HPP__
