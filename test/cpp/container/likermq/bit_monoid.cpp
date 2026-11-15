#include "test/common/testutil.hpp"
#include "src/cpp/container/likermq/bit_monoid.hpp"
#include "src/cpp/container/policy/monoid_policy_impl_min.hpp"
#include "src/cpp/container/policy/monoid_policy_impl_range_add_range_min.hpp"
#include "src/cpp/container/policy/monoid_policy_impl_sum.hpp"

// prod / get / set が使えるかどうか
template <class T, class = void>
struct has_prod : false_type {};
template <class T>
struct has_prod<T, decltype((void)declval<const T&>().prod(0, 0))> : true_type {};
template <class T, class = void>
struct has_set : false_type {};
template <class T>
struct has_set<T, decltype((void)declval<T&>().set(0, typename T::value_type()))> : true_type {};

static_assert(has_prod<BitreeMonoid<SumGroup<long long>>>::value, "");
static_assert(has_set<BitreeMonoid<SumGroup<long long>>>::value, "");
static_assert(!has_prod<BitreeMonoid<MinMonoid<long long>>>::value, "");
static_assert(!has_set<BitreeMonoid<MinMonoid<long long>>>::value, "");
// 明示的な型引数で別の群を指定しても使えない
template <class T, class Q, class = void>
struct has_prod_with : false_type {};
template <class T, class Q>
struct has_prod_with<T, Q, decltype((void)declval<const T&>().template prod<Q>(0, 0))> : true_type {
};
static_assert(has_prod_with<BitreeMonoid<SumGroup<long long>>, SumGroup<long long>>::value, "");
static_assert(!has_prod_with<BitreeMonoid<MinMonoid<long long>>, SumGroup<long long>>::value, "");

// 可換群: xor
struct XorGroup final : GroupPolicy<unsigned> {
  value_type operate(const value_type& a, const value_type& b) const override { return a ^ b; }
  value_type unit() const override { return 0; }
  value_type inverse(const value_type& a) const override { return a; }
};

void test_group_random() {
  repeat(_, 20) {
    const int n = Rand::i(0, 60);
    vector<long long> v(n);
    for (auto& x : v)
      x = Rand::i(-1000LL, 1000LL);
    // 両方のコンストラクタ
    BitreeMonoid<SumGroup<long long>> ta(v), tb(n);
    for (int i = 0; i < n; ++i)
      tb.add(i, v[i]);
    CHKEQ(n, ta.size());
    CHKEQ(n, tb.size());
    repeat(__, 500) {
      int type = Rand::i(0, 4);
      if (type == 0 && n > 0) {
        int i = Rand::i(0, n - 1);
        long long x = Rand::i(-1000LL, 1000LL);
        ta.add(i, x);
        tb.add(i, x);
        v[i] += x;
      } else if (type == 1 && n > 0) {
        int i = Rand::i(0, n - 1);
        long long x = Rand::i(-1000LL, 1000LL);
        ta.set(i, x);
        tb.set(i, x);
        v[i] = x;
      } else if (type == 2 && n > 0) {
        int i = Rand::i(0, n - 1);
        CHKEQ(v[i], ta.get(i));
        CHKEQ(v[i], tb.get(i));
      } else {
        // 範囲外も含む
        int l = Rand::i(-2, n + 2), r = Rand::i(-2, n + 2);
        int cl = max(0, min(l, n)), cr = max(0, min(r, n));
        long long s = 0;
        for (int i = cl; i < cr; ++i)
          s += v[i];
        CHKEQ(s, ta.prod(l, r));
        CHKEQ(s, tb.prod(l, r));
        long long ps = accumulate(v.begin(), v.begin() + cr, 0LL);
        CHKEQ(ps, ta.prefix(r));
      }
    }
  }
}

void test_xor() {
  const int n = 37;
  vector<unsigned> v(n);
  for (auto& x : v)
    x = Rand::i(0u, 1u << 20);
  BitreeMonoid<XorGroup> b(v);
  for (int l = 0; l <= n; ++l)
    for (int r = l; r <= n; ++r) {
      unsigned s = 0;
      for (int i = l; i < r; ++i)
        s ^= v[i];
      CHKEQ(s, b.prod(l, r));
    }
}

// モノイドのみ（prefix のみ）．LazyMonoidPolicy も MonoidPolicy として渡せる
template <class P>
void test_prefix_min() {
  const int n = 50;
  vector<long long> v(n);
  for (auto& x : v)
    x = Rand::i(-1000LL, 1000LL);
  BitreeMonoid<P> b(v);
  repeat(_, 300) {
    int i = Rand::i(0, n - 1);
    long long x = Rand::i(-1000LL, 1000LL);
    b.add(i, x);  // chmin
    v[i] = min(v[i], x);
    int r = Rand::i(-1, n + 1);
    int cr = max(0, min(r, n));
    long long m = numeric_limits<long long>::max();
    for (int k = 0; k < cr; ++k)
      m = min(m, v[k]);
    CHKEQ(m, b.prefix(r));
  }
}

void test_max_right() {
  repeat(_, 20) {
    const int n = Rand::i(0, 60);
    vector<long long> v(n);
    for (auto& x : v)
      x = Rand::i(0LL, 10LL);  // 非負なので前置和は単調
    BitreeMonoid<SumGroup<long long>> b(v);
    repeat(__, 50) {
      long long lim = Rand::i(-1LL, 10LL * n + 1);
      int expected = 0;
      long long s = 0;
      for (int r = 1; r <= n; ++r) {
        s += v[r - 1];
        if (s <= lim)
          expected = r;
        else
          break;
      }
      if (lim < 0)
        expected = 0;  // pred(unit()) == false は事前条件違反だが，0 を返す
      CHKEQ(expected, b.max_right([&](long long x) { return x <= lim; }));
    }
  }
}

// モノイドのみでの max_right（前置最小値は単調非増加）
void test_max_right_min() {
  repeat(_, 20) {
    const int n = Rand::i(0, 60);
    vector<long long> v(n);
    for (auto& x : v)
      x = Rand::i(-100LL, 100LL);
    BitreeMonoid<MinMonoid<long long>> b(v);
    repeat(__, 50) {
      long long lim = Rand::i(-101LL, 101LL);
      int expected = 0;
      long long m = numeric_limits<long long>::max();
      for (int r = 1; r <= n; ++r) {
        m = min(m, v[r - 1]);
        if (m < lim)
          break;
        expected = r;
      }
      CHKEQ(expected, b.max_right([&](long long x) { return x >= lim; }));
    }
  }
}

void test_usage() {
  BitreeMonoid<SumGroup<long long>> b(vector<long long>{3, 1, 4});
  b.add(1, 10);
  CHKEQ(15LL, b.prod(1, 3));
  CHKEQ(2, b.max_right([](long long s) { return s <= 14; }));
  BitreeMonoid<SumGroup<long long>> empty_bit;
  CHKEQ(0, empty_bit.size());
  CHKEQ(0LL, empty_bit.prefix(5));
  CHKEQ(0, empty_bit.max_right([](long long) { return true; }));
}

int main() {
  test_group_random();
  test_xor();
  test_prefix_min<MinMonoid<long long>>();
  test_prefix_min<RangeAddRangeMin<long long>>();
  test_max_right();
  test_max_right_min();
  test_usage();
  return 0;
}
