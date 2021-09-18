#include "test/common/testutil.hpp"
#include "src/cpp/container/balancing/treap_segment.hpp"

void test_insertAndDelete() {
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
    auto tpv = tp.toVector();
    repeat(i, cnt) CHKEQ(vc[i], tpv[i]);
  }
}

void test_concatAndSplit() {
  const int N = 30;

  Treap tp;
  repeat(i, N) tp.insert(i, i);

  vector<int> vc(N);
  iota(all(vc), 0);

  repeat(i, N) CHKEQ(vc[i], tp[i]);

  repeat(_, 1000) {
    int n_left_split = Rand::i(1, N - 1);
    {
      auto a = tp.split(n_left_split);
      tp.concat(move(a));
    }
    {
      vector<int> vc2(N);

      auto it = vc.begin();
      advance(it, n_left_split);
      auto it2 = copy(it, vc.end(), vc2.begin());
      copy(vc.begin(), it, it2);

      vc = move(vc2);
    }
    repeat(i, N) CHKEQ(vc[i], tp[i]);
  }
}

int main() {
  test_insertAndDelete();
  test_concatAndSplit();

  return 0;
}
