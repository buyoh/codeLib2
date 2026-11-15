#include "test/common/testutil.hpp"
#include "src/cpp/container/balancing/rbtree_multiset_monoid.hpp"
#include "src/cpp/container/policy/monoid_policy_impl_range_add_range_min.hpp"

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

// 素朴な multimap: キーの昇順，同じキーは挿入順
template <class S>
struct Naive {
  vector<pair<int, S>> v;
  int lower(int k) const {
    int i = 0;
    while (i < (int)v.size() && v[i].first < k)
      ++i;
    return i;
  }
  int upper(int k) const {
    int i = 0;
    while (i < (int)v.size() && v[i].first <= k)
      ++i;
    return i;
  }
};

template <class P, class GenS, class GenF, class Eq>
void test_random(int n_ops, int key_range, GenS gen_s, GenF gen_f, Eq eq) {
  using S = typename P::value_type;
  const P p{};
  RBTreeMultiSetMonoid<int, P> st;
  Naive<S> nv;
  auto& v = nv.v;

  auto naive_prod = [&](int l, int r) {
    S res = p.unit();
    for (int i = l; i < r; ++i)
      res = p.operate(res, v[i].second);
    return res;
  };

  for (int lop = 0; lop < n_ops; ++lop) {
    int n = (int)v.size();
    int type = Rand::i(0, 11);
    int k = Rand::i(-1, key_range + 1);
    if (type <= 3) {
      S x = gen_s();
      int i = st.insert(k, x);
      int ni = nv.upper(k);
      CHKEQ(ni, i);
      v.insert(v.begin() + ni, make_pair(k, x));
    } else if (type == 4) {
      bool ok = st.erase_one(k);
      int l = nv.lower(k), r = nv.upper(k);
      CHKEQ(l < r, ok);
      if (l < r)
        v.erase(v.begin() + l);
    } else if (type == 5) {
      int c = st.erase_all(k);
      int l = nv.lower(k), r = nv.upper(k);
      CHKEQ(r - l, c);
      v.erase(v.begin() + l, v.begin() + r);
    } else if (type == 6) {
      if (n > 0) {
        int i = Rand::i(0, n - 1);
        st.erase_at(i);
        v.erase(v.begin() + i);
      }
    } else if (type == 7) {
      int l = nv.lower(k), r = nv.upper(k);
      CHKEQ(l, st.lower_bound(k));
      CHKEQ(r, st.upper_bound(k));
      CHKEQ(r - l, st.count(k));
      CHKEQ(l < r ? l : -1, st.index_of(k));
      CHKEQ(l < r, st.contains(k));
    } else if (type == 8) {
      if (n > 0) {
        int i = Rand::i(0, n - 1);
        CHKEQ(v[i].first, st.key_at(i));
        CHK(eq(v[i].second, st.value_at(i)));
        S x = gen_s();
        st.set_at(i, x);
        v[i].second = x;
      }
    } else if (type == 9) {
      int lo = Rand::i(-1, key_range + 1), hi = Rand::i(-1, key_range + 1);
      S expected = lo < hi ? naive_prod(nv.lower(lo), nv.lower(hi)) : p.unit();
      CHK(eq(expected, st.prod(lo, hi)));
    } else if (type == 10) {
      int lo = Rand::i(-1, key_range + 1), hi = Rand::i(-1, key_range + 1);
      auto f = gen_f();
      st.apply(lo, hi, f);
      if (lo < hi)
        for (int i = nv.lower(lo); i < nv.lower(hi); ++i)
          v[i].second = p.mapping(f, v[i].second);
    } else {
      CHK(eq(naive_prod(0, n), st.all_prod()));
    }
    CHKEQ((int)v.size(), st.size());
    CHK(st.validate());
  }
  auto out = st.to_vector();
  CHKEQ(v.size(), out.size());
  for (int i = 0; i < (int)v.size(); ++i) {
    CHKEQ(v[i].first, out[i].first);
    CHK(eq(v[i].second, out[i].second));
  }
}

void test_usage() {
  RBTreeMultiSetMonoid<int, AddMin> m;
  m.insert(10, 3);
  m.insert(20, 1);
  m.insert(20, 5);
  m.insert(30, 4);
  m.apply(0, 25, 10);
  CHKEQ(11LL, m.prod(15, 25));
  CHKEQ(2, m.count(20));
  CHKEQ(13LL, m.value_at(0));
  CHKEQ(11LL, m.value_at(1));
  CHKEQ(15LL, m.value_at(2));
  CHKEQ(4LL, m.value_at(3));
  CHKEQ(AddMin().unit(), m.prod(25, 15));
  CHK(m.validate());
}

// 同じキーの要素は挿入順に operate される
void test_order() {
  RBTreeMultiSetMonoid<int, Concat> m;
  m.insert(1, "a");
  m.insert(0, "x");
  m.insert(1, "b");
  m.insert(2, "y");
  m.insert(1, "c");
  CHK(m.prod(1, 2) == "abc");
  CHK(m.all_prod() == "xabcy");
  CHK(m.erase_one(1));
  CHK(m.prod(1, 2) == "bc");
  m.set_at(2, "C");
  CHK(m.all_prod() == "xbCy");
  CHKEQ(2, m.erase_all(1));
  CHK(m.all_prod() == "xy");
}

void test_performance() {
  const int n = 100000;
  RBTreeMultiSetMonoid<int, AddMin> m;
  for (int i = 0; i < n; ++i)
    m.insert(i % 1000, (long long)i);
  CHK(m.validate());
  long long acc = 0;
  for (int i = 0; i < 20000; ++i) {
    int lo = Rand::i(0, 999), hi = Rand::i(lo + 1, 1000);
    acc += m.prod(lo, hi) == (long long)lo;
    m.apply(lo, hi, 0LL);
  }
  CHKEQ(20000LL, acc);
  for (int k = 0; k < 1000; ++k)
    m.erase_all(k);
  CHK(m.empty());
}

int main() {
  test_usage();
  test_order();
  auto eq_affine = [](const AffineSumValue& a, const AffineSumValue& b) { return a == b; };
  auto eq_str = [](const string& a, const string& b) { return a == b; };
  repeat(_, 3) {
    for (int key_range : {5, 1000}) {
      test_random<AffineSum>(
          3000, key_range,
          [] {
            return AffineSumValue{Rand::i(0LL, AffineSum::MD - 1), 1};
          },
          [] {
            return AffineSumOperator{Rand::i(0LL, AffineSum::MD - 1),
                                     Rand::i(0LL, AffineSum::MD - 1)};
          },
          eq_affine);
      test_random<Concat>(
          3000, key_range, [] { return string(1, char('a' + Rand::i(0, 25))); }, [] { return 0; },
          eq_str);
    }
  }
  test_performance();
  return 0;
}
