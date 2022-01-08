#include "test/common/testutil.hpp"
#include "src/cpp/math/method/combinatorics_precalc.hpp"
#include "src/cpp/math/method/combinatorics_static.hpp"
#include "src/cpp/math/method/combinatorics.hpp"

constexpr ll MD = 998244353;

Combinatorics<1000, MD> comb;
nCrTable<1000, MD> table;

int main() {
  repeat(n, 100) {
    repeat(r, 100) {
      int ncr1 = nCr_md<ll>(n, r, MD);
      int ncr2 = comb.nCr(n, r);
      int ncr3 = table.nCr(n, r);
      CHKEQ(ncr1, ncr2);
      CHKEQ(ncr2, ncr3);
      int nhr1 = nHr_md<ll>(n, r, MD);
      int nhr2 = comb.nHr(n, r);
      int nhr3 = table.nHr(n, r);
      CHKEQ(nhr1, nhr2);
      CHKEQ(nhr2, nhr3);
    }
  }
  repeat(n, 1000) {
    repeat(r, 1000) {
      int ncr2 = comb.nCr(n, r);
      int ncr3 = table.nCr(n, r);
      CHKEQ(ncr2, ncr3);
    }
  }
}
