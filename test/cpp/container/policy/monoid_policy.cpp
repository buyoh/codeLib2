#include "test/common/testutil.hpp"
#include "src/cpp/container/policy/monoid_policy.hpp"
#include "src/cpp/container/policy/monoid_policy_impl_range_add_range_min.hpp"
#include "src/cpp/container/policy/monoid_policy_impl_range_add_range_sum.hpp"
#include "src/cpp/container/policy/monoid_policy_impl_min.hpp"
#include "src/cpp/container/policy/monoid_policy_impl_sum.hpp"

using MinLL = RangeAddRangeMin<long long>;
using SumLL = RangeAddRangeSum<long long>;

// LazyMonoidPolicy は MonoidPolicy を要求するデータ構造にも渡せる
static_assert(is_base_of<LazyMonoidPolicy<long long, long long>, MinLL>::value, "");
static_assert(is_base_of<MonoidPolicy<long long>, MinLL>::value, "");
static_assert(is_base_of<LazyMonoidPolicy<SumLL::value_type, long long>, SumLL>::value, "");
static_assert(is_base_of<MonoidPolicy<SumLL::value_type>, SumLL>::value, "");
static_assert(!is_base_of<MonoidPolicy<int>, MinLL>::value, "");
static_assert(is_abstract<MonoidPolicy<int>>::value, "");
static_assert(is_abstract<LazyMonoidPolicy<int, int>>::value, "");
static_assert(!is_abstract<MinLL>::value, "");
static_assert(!is_abstract<SumLL>::value, "");

// GroupPolicy も MonoidPolicy として渡せる
static_assert(is_base_of<GroupPolicy<long long>, SumGroup<long long>>::value, "");
static_assert(is_base_of<MonoidPolicy<long long>, SumGroup<long long>>::value, "");
static_assert(is_base_of<MonoidPolicy<int>, MinMonoid<int>>::value, "");
static_assert(!is_base_of<GroupPolicy<int>, MinMonoid<int>>::value, "");
static_assert(is_abstract<GroupPolicy<int>>::value, "");

// 作用なしのモノイド
struct Max final : MonoidPolicy<int> {
  value_type operate(const value_type& a, const value_type& b) const override { return max(a, b); }
  value_type unit() const override { return numeric_limits<value_type>::min(); }
};
static_assert(is_base_of<MonoidPolicy<int>, Max>::value, "");
static_assert(!is_base_of<LazyMonoidPolicy<int, int>, Max>::value, "");

// モノイドの法則（結合律・単位元）
template <class P, class GenV, class Eq>
void check_monoid(GenV gen_v, Eq eq) {
  const P p{};
  repeat(_, 1000) {
    auto a = gen_v(), b = gen_v(), c = gen_v();
    CHK(eq(p.operate(p.operate(a, b), c), p.operate(a, p.operate(b, c))));
    CHK(eq(p.operate(p.unit(), a), a));
    CHK(eq(p.operate(a, p.unit()), a));
  }
}

// 作用の法則．mapping は空でない区間の集約値にのみ適用されるため，unit() には適用しない
template <class P, class GenV, class GenF, class Eq>
void check_lazy(GenV gen_v, GenF gen_f, Eq eq) {
  check_monoid<P>(gen_v, eq);
  const P p{};
  repeat(_, 1000) {
    auto a = gen_v(), b = gen_v();
    auto f = gen_f(), g = gen_f();
    CHK(eq(p.mapping(f, p.operate(a, b)), p.operate(p.mapping(f, a), p.mapping(f, b))));
    CHK(eq(p.mapping(p.composition(f, g), a), p.mapping(f, p.mapping(g, a))));
    CHK(eq(p.mapping(p.id(), a), a));
  }
}

void test_range_add_range_min() {
  auto gen_v = [] { return Rand::i(-1000000LL, 1000000LL); };
  auto gen_f = [] { return Rand::i(-1000LL, 1000LL); };
  auto eq = [](long long a, long long b) { return a == b; };
  check_lazy<MinLL>(gen_v, gen_f, eq);

  const MinLL p{};
  CHKEQ(numeric_limits<long long>::max(), p.unit());
  CHKEQ(3LL, p.operate(3, 5));
  CHKEQ(8LL, p.mapping(5, 3));
  CHKEQ(0LL, p.id());

  // 型引数を変えられる
  const RangeAddRangeMin<int> q{};
  CHKEQ(numeric_limits<int>::max(), q.unit());
  CHKEQ(-2, q.operate(-2, 7));
}

void test_range_add_range_sum() {
  // 1 要素または複数要素をまとめた値
  auto gen_v = [] {
    int len = Rand::i(1, 5);
    return SumLL::value_type{Rand::i(-1000000LL, 1000000LL), len};
  };
  auto gen_f = [] { return Rand::i(-1000LL, 1000LL); };
  auto eq = [](const SumLL::value_type& a, const SumLL::value_type& b) {
    return a.sum == b.sum && a.len == b.len;
  };
  check_lazy<SumLL>(gen_v, gen_f, eq);

  const SumLL p{};
  CHKEQ(0LL, p.unit().sum);
  CHKEQ(0, p.unit().len);
  SumLL::value_type x = p.mapping(2, SumLL::value_type{10, 3});
  CHKEQ(16LL, x.sum);
  CHKEQ(3, x.len);
}

void test_monoid_only() {
  auto gen_v = [] { return Rand::i(-1000000, 1000000); };
  check_monoid<Max>(gen_v, [](int a, int b) { return a == b; });
}

// 群の法則（逆元）
template <class P, class GenV, class Eq>
void check_group(GenV gen_v, Eq eq) {
  check_monoid<P>(gen_v, eq);
  const P p{};
  repeat(_, 1000) {
    auto a = gen_v();
    CHK(eq(p.operate(a, p.inverse(a)), p.unit()));
    CHK(eq(p.operate(p.inverse(a), a), p.unit()));
  }
}

void test_sum_min() {
  auto gen_v = [] { return Rand::i(-1000000LL, 1000000LL); };
  auto eq = [](long long a, long long b) { return a == b; };
  check_group<SumGroup<long long>>(gen_v, eq);
  check_monoid<MinMonoid<long long>>(gen_v, eq);
  const SumGroup<long long> s{};
  CHKEQ(0LL, s.unit());
  CHKEQ(-5LL, s.inverse(5));
  const MinMonoid<long long> m{};
  CHKEQ(numeric_limits<long long>::max(), m.unit());
  CHKEQ(2LL, m.operate(2, 3));
}

int main() {
  test_sum_min();
  test_range_add_range_min();
  test_range_add_range_sum();
  test_monoid_only();
  return 0;
}
