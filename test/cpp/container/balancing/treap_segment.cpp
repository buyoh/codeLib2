#include "test/common/testutil.hpp"
#include "src/cpp/container/balancing/treap_segment.hpp"
#include "src/cpp/container/policy/monoid_policy_impl_range_add_range_min.hpp"
#include "src/cpp/container/policy/monoid_policy_impl_range_add_range_sum.hpp"

template <class P>
using Seq = TreapSegment<P>;

template <class T>
bool check_valid(T&) {
  return true;
}

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
    Seq<AddMin> t(vector<long long>{3, 1, 4});
    t.apply(0, 2, 10);
    CHKEQ(11LL, t.prod(0, 2));
    CHKEQ(4LL, t.all_prod());
  }
  {
    using P = RangeAddRangeSum<long long>;
    Seq<P> t(3, P::value_type{1, 1});
    t.apply(1, 3, 2);
    CHKEQ(7LL, t.prod(0, 3).sum);
    CHKEQ(3, t.prod(0, 3).len);
    CHKEQ(3LL, t.get(2).sum);
  }
  {
    Seq<PrivateConcat> t(vector<string>{"a", "b", "c"});
    t.insert(1, "x");
    CHK(t.all_prod() == "axbc");
  }
}

int clip(int i, int n) {
  return i < 0 ? 0 : i > n ? n : i;
}

template <class P, class GenS, class GenF>
void test_random(int n_ops, GenS gen_s, GenF gen_f) {
  using S = typename P::value_type;
  const P p{};
  Seq<P> t;
  vector<S> v;

  auto naive_prod = [&](int l, int r) {
    S res = p.unit();
    for (int i = l; i < r; ++i)
      res = p.operate(res, v[i]);
    return res;
  };

  for (int lop = 0; lop < n_ops; ++lop) {
    int n = (int)v.size();
    int type = Rand::i(0, 9);
    if (type <= 2) {
      // insert（範囲外も含む）
      int i = Rand::i(-2, n + 2);
      S x = gen_s();
      t.insert(i, x);
      v.insert(v.begin() + clip(i, n), x);
    } else if (type == 3) {
      // erase（範囲外も含む）
      int i = Rand::i(-2, n + 1);
      t.erase(i);
      if (0 <= i && i < n)
        v.erase(v.begin() + i);
    } else if (type == 4) {
      if (n > 0) {
        int i = Rand::i(0, n - 1);
        CHK(t.get(i) == v[i]);
      }
    } else if (type == 5) {
      // set（範囲外も含む）
      int i = Rand::i(-1, n);
      S x = gen_s();
      t.set(i, x);
      if (0 <= i && i < n)
        v[i] = x;
    } else if (type == 6) {
      int l = Rand::i(-2, n + 2), r = Rand::i(-2, n + 2);
      CHK(t.prod(l, r) == naive_prod(clip(l, n), clip(r, n)));
    } else if (type == 7) {
      int l = Rand::i(-2, n + 2), r = Rand::i(-2, n + 2);
      auto f = gen_f();
      t.apply(l, r, f);
      for (int i = clip(l, n); i < clip(r, n); ++i)
        v[i] = p.mapping(f, v[i]);
    } else if (type == 8) {
      // 先頭 k 要素を切り出して末尾に回す
      int k = Rand::i(-1, n + 1);
      Seq<P> a;
      a = t.split(k);  // ムーブ代入
      t.concat(a);
      CHK(a.empty());
      rotate(v.begin(), v.begin() + clip(k, n), v.end());
    } else {
      // 自分自身との concat は何もしない
      t.concat(t);
      CHK(t.to_vector() == v);
      CHK(t.all_prod() == naive_prod(0, n));
    }
    CHKEQ((int)v.size(), t.size());
    CHK(check_valid(t));
  }
  CHK(t.to_vector() == v);
}

void test_construct() {
  for (int n = 0; n <= 70; ++n) {
    vector<string> v(n);
    for (int i = 0; i < n; ++i)
      v[i] = string(1, char('a' + i % 26));
    Seq<Concat> t(v);
    CHKEQ(n, t.size());
    CHK(t.empty() == (n == 0));
    CHK(check_valid(t));
    CHK(t.all_prod() == accumulate(all(v), string()));
    CHK(t.to_vector() == v);

    Seq<AddMin> u(n, 7LL);
    CHK(check_valid(u));
    CHKEQ(n, u.size());
    CHKEQ((n == 0 ? AddMin().unit() : 7LL), u.all_prod());
  }
  {
    const int n = 50000;
    vector<long long> v(n);
    iota(all(v), 0LL);
    Seq<AddMin> t(v);
    CHK(check_valid(t));
    CHKEQ(n, t.size());
    CHKEQ(1000LL, t.prod(1000, 2000));
  }
  {
    Seq<AddMin> t(-5, 1LL);
    CHK(t.empty());
  }
}

// 大きさの大きく異なる列の連結
void test_concat_unbalanced() {
  for (int big : {1, 2, 10, 1000}) {
    vector<long long> va(1, -1), vb(big);
    iota(all(vb), 0LL);
    {
      Seq<AddMin> sa(va), sb(vb);
      sa.concat(move(sb));
      CHK(sb.empty());
      CHK(check_valid(sa));
      CHKEQ(big + 1, sa.size());
      CHKEQ(-1LL, sa.get(0));
      CHKEQ((long long)big - 1, sa.get(big));
    }
    {
      Seq<AddMin> sa(va), sb(vb);
      sb.concat(sa);
      CHK(sa.empty());
      CHK(check_valid(sb));
      CHKEQ(-1LL, sb.get(big));
      CHKEQ(-1LL, sb.all_prod());
    }
  }
}

void test_performance() {
  const int n = 50000;
  Seq<AddMin> t;
  for (int i = 0; i < n; ++i)
    t.insert(t.size(), (long long)i);
  CHK(check_valid(t));
  long long acc = 0;
  for (int i = 0; i < n; ++i) {
    int l = Rand::i(0, n - 1), r = Rand::i(l + 1, n);
    acc += t.prod(l, r) == (long long)l;
    t.apply(l, r, 0LL);
  }
  CHKEQ((long long)n, acc);
  for (int i = 0; i < n; ++i)
    t.erase(0);
  CHK(t.empty());
  CHK(check_valid(t));
}

int main() {
  test_construct();
  test_concat_unbalanced();
  test_impl_policies();
  repeat(_, 3) {
    test_random<AffineSum>(
        3000, [] { return AffineSumValue{Rand::i(0LL, AffineSum::MD - 1), 1}; },
        [] {
          return AffineSumOperator{Rand::i(0LL, AffineSum::MD - 1),
                                   Rand::i(0LL, AffineSum::MD - 1)};
        });
    test_random<Concat>(
        3000, [] { return string(1, char('a' + Rand::i(0, 25))); }, [] { return 0; });
  }
  test_performance();
  return 0;
}
