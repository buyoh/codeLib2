#include "test/common/testutil.hpp"
#include "src/cpp/search/method/tri_search.hpp"

int main() {
  constexpr int nq = 10;
  constexpr int n = 100;
  int arr[n];
  repeat(_, nq) {
    arr[0] = 0;
    int z = Rand::i(1, n - 1);
    iterate(i, 0, z) {
      arr[i + 1] = arr[i] - Rand::i(1, 3);
    }
    iterate(i, z, n) {
      arr[i + 1] = arr[i] + Rand::i(1, 3);
    }
    int ans = ternarySearchMinimize(0, n, [&arr](int i) { return arr[i]; });
    CHKEQ(z, ans);
  }
  return 0;
}
