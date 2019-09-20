// %title
// SuffixArray
//
// %overview
//
// %usage
//
// %verified
//
//
// %references
// 蟻本

#include <bits/stdc++.h>
using namespace std;

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
  }
  SuffixArray(string&& d) {
    keeper = move(d);
    str = keeper.c_str();
    size = keeper.size();
  }

  void build() {
    vector<int> sfx(size);
    iota(sfx.begin(), sfx.end(), 0);
    sort(sfx.begin(), sfx.end(), [this](int l, int r) { return str[l] < str[r]; });

    vector<int> rank(size * 2, -1);
    vector<int> tmp(size * 2, -1);

    for (int i = 0; i < size; ++i)
      rank[i] = str[i];
    for (int d = 1; d < size; d *= 2) {
      auto compare = [d, &rank](int l, int r) { return rank[l] != rank[r] ? rank[l] < rank[r] : rank[l + d] < rank[r + d]; };
      sort(sfx.begin(), sfx.end(), compare);
      tmp[sfx[0]] = 0;
      for (int i = 1; i < size; ++i)
        tmp[sfx[i]] = tmp[sfx[i - 1]] + compare(sfx[i - 1], sfx[i]);
      tmp.swap(rank);
    }
    sa.resize(size);
    for (int i = 0; i < size; ++i)
      sa[i] = str + sfx[i];
  }
  pair<bool, pair<int, int>> find(const string& keyword) {
    auto n = keyword.size();

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
