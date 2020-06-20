#include "test/common/testutil.hpp"
#include "src/cpp/container/balancing/treap_set.cpp"

int main() {
  Treap tp;
  map<int, int> mp;

  unordered_set<int> erased;

  for (int lop = 0; lop < 4000; ++lop) {
    int k = Rand::i(1, 5000);
    int v = Rand::i(0, 5000);
    if (v == 0) {
      tp.erase(k);
      mp.erase(k);
      erased.insert(k);
    } else {
      tp[k] = v;
      mp[k] = v;
      if (erased.count(k))
        erased.erase(k);
    }
  }

  for (auto p : mp) {
    assert(tp[p.first] == p.second);
  }
  for (auto e : erased) {
    assert(bool(tp.find(e)) == bool(mp.count(e)));
  }

  return 0;
}
