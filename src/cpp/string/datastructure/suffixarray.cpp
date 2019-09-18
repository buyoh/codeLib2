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

class SuffixArray {
  const char* data;
  int size;
  vector<const char*> sa;
  string keeper;

 public:
  SuffixArray() {}
  SuffixArray(const string& d) {
    data = d.c_str();
    size = d.size();
  }
  SuffixArray(string&& d) {
    keeper = move(d);
    data = keeper.c_str();
    size = keeper.size();
  }

  void build() {
    sa.resize(size);

    for (int i = 0; i < size; i++) {
      sa[i] = data + i;
    }
    sort(sa.begin(), sa.end(), [](const char* l, const char* r) { return *l < *r; });

    vector<int> rank(size * 2, -1);
    vector<int> tmp(size * 2, -1);

    rank[sa[0] - data] = 0;
    for (int i = 1; i < size; ++i)
      rank[sa[i] - data] = rank[sa[i - 1] - data] + (*sa[i - 1] < *sa[i]);
    for (int d = 1; d < size; d *= 2) {
      auto compare = [this, d, &rank](const char* l, const char* r) {
        return rank[l - data] != rank[r - data] ? rank[l - data] < rank[r - data] : rank[l + d - data] < rank[r + d - data];
      };
      sort(sa.begin(), sa.end(), compare);
      tmp[sa[0] - data] = 0;
      for (int i = 1; i < size; ++i)
        tmp[sa[i] - data] = tmp[sa[i - 1] - data] + compare(sa[i - 1], sa[i]);
      for (int i = 0; i < size; ++i)
        rank[i] = tmp[i];
    }
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

  inline int foundToIdx(int found) { return sa[found] - data; }
};
