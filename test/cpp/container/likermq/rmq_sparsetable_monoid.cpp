#include "test/common/testutil.hpp"
#include "src/cpp/container/likermq/rmq_sparsetable_monoid.hpp"
#include "src/cpp/container/policy/monoid_policy_impl_min.hpp"

struct Gcd final : MonoidPolicy<long long> {
  value_type operate(const value_type& a, const value_type& b) const override {
    return __gcd(a, b);
  }
  value_type unit() const override { return 0; }
};

// 冪等で非可換: 最初の非空要素（-1 が単位元）
struct First final : MonoidPolicy<int> {
  value_type operate(const value_type& a, const value_type& b) const override {
    return a == -1 ? b : a;
  }
  value_type unit() const override { return -1; }
};

template <class P, class Gen>
void test_random(Gen gen) {
  using T = typename P::value_type;
  const P p{};
  repeat(_, 30) {
    const int n = Rand::i(0, 70);
    vector<T> v(n);
    for (auto& x : v)
      x = gen();
    SparseTableMonoid<P> st(v);
    CHKEQ(n, st.size());
    for (int l = -2; l <= n + 2; ++l)
      for (int r = -2; r <= n + 2; ++r) {
        T s = p.unit();
        for (int i = max(l, 0); i < min(r, n); ++i)
          s = p.operate(s, v[i]);
        CHKEQ(s, st.prod(l, r));
      }
  }
}

void test_usage() {
  SparseTableMonoid<MinMonoid<int>> st(vector<int>{3, 1, 4});
  CHKEQ(1, st.prod(0, 2));
  CHKEQ(4, st.prod(2, 3));
  CHKEQ(numeric_limits<int>::max(), st.prod(1, 1));
  SparseTableMonoid<MinMonoid<int>> empty_st;
  CHKEQ(0, empty_st.size());
  CHKEQ(numeric_limits<int>::max(), empty_st.prod(0, 3));
}

void test_performance() {
  const int n = 200000;
  vector<int> v(n);
  for (auto& x : v)
    x = Rand::i(-1000000000, 1000000000);
  SparseTableMonoid<MinMonoid<int>> st(v);
  long long acc = 0;
  repeat(_, 200000) {
    int l = Rand::i(0, n - 1), r = Rand::i(l + 1, n);
    acc += st.prod(l, r) <= v[l];
  }
  CHKEQ(200000LL, acc);
}

int main() {
  test_random<MinMonoid<int>>([] { return Rand::i(-100, 100); });
  test_random<Gcd>([] { return Rand::i(0LL, 60LL) * 6; });
  test_random<First>([] { return Rand::i(0, 3) == 0 ? -1 : Rand::i(0, 100); });
  test_usage();
  test_performance();
  return 0;
}
