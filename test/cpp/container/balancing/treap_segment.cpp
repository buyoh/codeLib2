#include "test/common/testutil.hpp"
#include "src/cpp/container/balancing/treap_segment.hpp"

int main() {
  Treap tp;
  deque<int> vc;
  int cnt = 0;

  for (int lop = 0; lop < 4000; ++lop) {
    // cout << "lop" << lop << endl;
    int k = Rand::i(0, cnt);
    int v = lop;  // rand_range(0, 10);
    if (Rand::i(0, 3)) {
      auto it = vc.begin();
      advance(it, k);
      vc.insert(it, v);
      tp.insert(k, v);
      ++cnt;
    } else {
      if (k == 0)
        continue;
      --k;
      auto it = vc.begin();
      advance(it, k);
      vc.erase(it);
      tp.erase(k);
      --cnt;
    }
    // tp.print_tour();
    repeat(i, cnt) {
      (tp.find(i))->value;
      // cout << make_pair(vc[i], (tp.find(i))->value) << endl;
      CHKEQ(vc[i], (tp.find(i))->value);
    }
  }

  return 0;
}
