#include "test/common/testutil.hpp"
#include "src/cpp/container/array/cycledarray.hpp"

int check1(vector<int> v, int mid, int term) {
  vector<int> v1 = v;
  vector<int> v2 = v1;
  repeat(i, v2.size() - 1) v2[i + 1] += v2[i];

  CycledVector<int> u1(v1);
  CycledVector<int> u2(v2);
  u1.set_loop(mid, term);

  repeat(i, int(v.size())) {
    CHKEQ(v1[i], u1[i]);
  }

  {
    ll total = 0;
    repeat(i, int(v.size())) {
      total += v1[i];
      CHKEQ(total, u2.sum_as_cum(i));
    }
  }

  return 0;
}

int main() {
  check1({1, 4, 3, 2, 7, 5, 3, 7, 5, 3, 7, 5, 3, 7, 5, 3, 7, 5, 3, 7}, 4, 7);
  check1({1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4}, 0, 4);
  check1({1, 2, 3, 3, 3, 3, 3, 3}, 2, 1);
  return 0;
}