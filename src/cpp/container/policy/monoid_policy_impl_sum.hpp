#ifndef SRC_CPP_CONTAINER_POLICY_MONOID_POLICY_IMPL_SUM_HPP__
#define SRC_CPP_CONTAINER_POLICY_MONOID_POLICY_IMPL_SUM_HPP__
// %=BEGIN DOC
// %title
// モノイドの Policy 実装例(和，群)
//
// %overview
// GroupPolicy の実装例．加算による（可換な）群．単位元は 0，逆元は符号反転．
// MonoidPolicy を要求するデータ構造にも渡せる．
//
// %usage
// SumGroup<T>
//
// BitreeMonoid<SumGroup<long long>> b(vector<long long>{3, 1, 4});
// b.add(1, 10);  // {3, 11, 4}
// b.prod(1, 3);  // 15
//
// %require
// ```
#include "src/cpp/container/policy/monoid_policy.hpp"
// ```
// %verified
//
// %references
//
// %words
// monoid, group, policy, sum
// %=END DOC
// %=BEGIN CODE

template <class T>
struct SumGroup final : GroupPolicy<T> {
  using value_type = T;
  value_type operate(const value_type& a, const value_type& b) const override { return a + b; }
  value_type unit() const override { return value_type(0); }
  value_type inverse(const value_type& a) const override { return -a; }
};

// %=END CODE
#endif  // SRC_CPP_CONTAINER_POLICY_MONOID_POLICY_IMPL_SUM_HPP__
