#include "test/common/testutil.hpp"
#include "src/cpp/container/balancing/rbtree_set_monoid.hpp"
#include "src/cpp/container/policy/monoid_policy_impl_range_add_range_min.hpp"
#include "src/cpp/container/policy/monoid_policy_impl_range_add_range_sum.hpp"

// アフィン変換 x -> b*x + c (mod 998244353) × 区間和．composition が非可換
struct AffineSumValue {
  long long sum;
  int len;
  bool operator==(const AffineSumValue& o) const { return sum == o.sum && len == o.len; }
};
struct AffineSumOperator {
  long long b, c;
};
struct AffineSum final : LazyMonoidPolicy<AffineSumValue, AffineSumOperator> {
  static const long long MD = 998244353;
  value_type operate(const value_type& a, const value_type& b) const override {
    return value_type{(a.sum + b.sum) % MD, a.len + b.len};
  }
  value_type unit() const override { return value_type{0, 0}; }
  value_type mapping(const operator_type& f, const value_type& x) const override {
    CHK(x.len > 0);  // mapping は空の区間（unit()）には適用されない
    return value_type{(f.b * x.sum + f.c * x.len) % MD, x.len};
  }
  operator_type composition(const operator_type& f, const operator_type& g) const override {
    return operator_type{f.b * g.b % MD, (f.b * g.c + f.c) % MD};
  }
  operator_type id() const override { return operator_type{1, 0}; }
};

// 文字列連結．operate が非可換．作用素は使わない
struct Concat final : LazyMonoidPolicy<string, int> {
  value_type operate(const value_type& a, const value_type& b) const override { return a + b; }
  value_type unit() const override { return value_type(); }
  value_type mapping(const operator_type&, const value_type& x) const override { return x; }
  operator_type composition(const operator_type&, const operator_type&) const override { return 0; }
  operator_type id() const override { return 0; }
};

// 区間加算・区間最小値（%usage の例）
using AddMin = RangeAddRangeMin<long long>;

// private 継承も is_base_of の検査を通る（C++17 での制約）
struct PrivateConcat final : private LazyMonoidPolicy<string, int> {
  using value_type = string;
  using operator_type = int;
  value_type operate(const value_type& a, const value_type& b) const override { return a + b; }
  value_type unit() const override { return value_type(); }
  value_type mapping(const operator_type&, const value_type& x) const override { return x; }
  operator_type composition(const operator_type&, const operator_type&) const override { return 0; }
  operator_type id() const override { return 0; }
};

void test_impl_policies() {
  {
    // %usage の例
    RBTreeSetMonoid<int, AddMin> m;
    m.set(10, 3);
    m.set(20, 1);
    m.set(30, 4);
    m.apply(0, 25, 10);
    CHKEQ(11LL, m.prod(0, 25));
    CHKEQ(13LL, m.get(10));
    CHKEQ(4LL, m.get(30));
    CHK(m.validate());
  }
  {
    using P = RangeAddRangeSum<long long>;
    RBTreeSetMonoid<int, P> m;
    for (int k : {5, 1, 9})
      m.set(k, P::value_type{1, 1});
    m.apply(2, 10, 2);  // キー 5, 9
    CHKEQ(7LL, m.prod(0, 10).sum);
    CHKEQ(3, m.prod(0, 10).len);
    CHKEQ(1LL, m.get(1).sum);
  }
  {
    RBTreeSetMonoid<int, PrivateConcat> m;
    m.set(3, "c");
    m.set(1, "a");
    m.set(2, "b");
    CHK(m.prod(1, 3) == "ab");
    CHK(m.all_prod() == "abc");
  }
}

template <class P, class GenS, class GenF>
void test_random(int n_ops, GenS gen_s, GenF gen_f) {
  using S = typename P::value_type;
  const P p{};
  RBTreeSetMonoid<int, P> st;
  map<int, S> mp;

  auto naive_prod = [&](int lo, int hi) {
    S res = p.unit();
    if (lo < hi)
      for (auto it = mp.lower_bound(lo); it != mp.end() && it->first < hi; ++it)
        res = p.operate(res, it->second);
    return res;
  };

  for (int lop = 0; lop < n_ops; ++lop) {
    int k = Rand::i(-5, 305);
    int type = Rand::i(0, 9);
    if (type == 0) {
      CHKEQ((bool)mp.erase(k), st.erase(k));
    } else if (type == 1) {
      S x = gen_s();
      CHKEQ(mp.insert(make_pair(k, x)).second, st.insert(k, x));
    } else if (type == 2) {
      S x = gen_s();
      st.set(k, x);
      mp[k] = x;
    } else if (type == 3) {
      CHKEQ(mp.count(k) > 0, st.contains(k));
      auto it = mp.find(k);
      CHK(st.get(k) == (it == mp.end() ? p.unit() : it->second));
    } else if (type == 4 || type == 5) {
      // 端が既存キーと一致する場合・lo >= hi も含む
      int lo = Rand::i(-10, 310), hi = Rand::i(-10, 310);
      if (Rand::i(0, 3) == 0 && !mp.empty())
        lo = st.key_at(Rand::i(0, st.size() - 1));
      CHK(st.prod(lo, hi) == naive_prod(lo, hi));
    } else if (type == 6) {
      int lo = Rand::i(-10, 310), hi = Rand::i(-10, 310);
      auto f = gen_f();
      st.apply(lo, hi, f);
      if (lo < hi)
        for (auto it = mp.lower_bound(lo); it != mp.end() && it->first < hi; ++it)
          it->second = p.mapping(f, it->second);
    } else if (type == 7) {
      CHKEQ((int)distance(mp.begin(), mp.lower_bound(k)), st.lower_bound(k));
      CHKEQ((int)distance(mp.begin(), mp.upper_bound(k)), st.upper_bound(k));
      auto it = mp.find(k);
      CHKEQ(it == mp.end() ? -1 : (int)distance(mp.begin(), it), st.index_of(k));
    } else if (type == 8) {
      if (!mp.empty()) {
        int i = Rand::i(0, (int)mp.size() - 1);
        auto it = mp.begin();
        advance(it, i);
        CHKEQ(it->first, st.key_at(i));
        CHK(st.value_at(i) == it->second);
      }
    } else {
      using KV = vector<pair<int, S>>;
      CHK(st.to_vector() == KV(all(mp)));
      CHK(st.all_prod() == naive_prod(INT_MIN, INT_MAX));
    }
    CHKEQ((int)mp.size(), st.size());
    CHK(st.validate());
  }
}

// apply の後に set で上書きした値に，古い作用素が残らないこと
void test_apply_then_set() {
  RBTreeSetMonoid<int, AddMin> st;
  for (int i = 0; i < 100; ++i)
    st.set(i * 10, 100);
  st.apply(0, 1000, 5);
  st.set(500, 1);
  CHKEQ(1LL, st.get(500));
  CHKEQ(1LL, st.prod(0, 1000));
  CHKEQ(105LL, st.prod(510, 1000));
  st.apply(500, 501, 10);
  CHKEQ(11LL, st.get(500));
  CHKEQ(105LL, st.prod(0, 500));
  CHKEQ(11LL, st.all_prod());
  // 空区間
  CHKEQ(AddMin().unit(), st.prod(5, 5));
  CHKEQ(AddMin().unit(), st.prod(7, 3));
  CHKEQ(AddMin().unit(), st.prod(1, 9));
  CHKEQ(AddMin().unit(), st.prod(2000, 3000));
}

void test_empty() {
  RBTreeSetMonoid<int, AddMin> st;
  CHK(st.empty());
  CHKEQ(AddMin().unit(), st.all_prod());
  CHKEQ(AddMin().unit(), st.prod(-100, 100));
  CHKEQ(AddMin().unit(), st.get(0));
  st.apply(-100, 100, 1);
  CHK(!st.erase(0));
  CHK(!st.contains(0));
  CHKEQ(0, st.lower_bound(0));
  CHKEQ(-1, st.index_of(0));
  CHK(st.validate());
}

void test_performance() {
  const int n = 50000;
  RBTreeSetMonoid<int, AddMin> st;
  for (int i = 0; i < n; ++i)
    st.insert(i, (long long)i);
  CHK(st.validate());
  long long acc = 0;
  for (int i = 0; i < n; ++i) {
    int lo = Rand::i(0, n - 1), hi = Rand::i(lo + 1, n);
    acc += st.prod(lo, hi) == (long long)lo;
    st.apply(lo, hi, 0LL);
  }
  CHKEQ((long long)n, acc);
  for (int i = 0; i < n; ++i)
    CHK(st.erase(i));
  CHK(st.empty());
  CHK(st.validate());
}

int main() {
  test_empty();
  test_apply_then_set();
  test_impl_policies();
  test_random<AffineSum>(
      10000, [] { return AffineSumValue{Rand::i(0LL, AffineSum::MD - 1), 1}; },
      [] {
        return AffineSumOperator{Rand::i(0LL, AffineSum::MD - 1), Rand::i(0LL, AffineSum::MD - 1)};
      });
  test_random<Concat>(
      10000, [] { return string(1, char('a' + Rand::i(0, 25))); }, [] { return 0; });
  test_performance();
  return 0;
}
