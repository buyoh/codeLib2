#ifndef SRC_CPP_CONTAINER_POLICY_MONOID_POLICY_IMPL_RANGE_ADD_RANGE_SUM_HPP__
#define SRC_CPP_CONTAINER_POLICY_MONOID_POLICY_IMPL_RANGE_ADD_RANGE_SUM_HPP__
// %=BEGIN DOC
// %title
// モノイドの Policy 実装例(区間加算・区間和)
//
// %overview
// LazyMonoidPolicy の実装例．作用素は T（加算する値）．
// 区間への加算には区間の長さが必要なため，要素は RangeAddRangeSumValue<T>{和, 長さ} で持つ．
// 1 つの要素 x は {x, 1} として入れる．
//
// %usage
// RangeAddRangeSum<T>
// RangeAddRangeSumValue<T>{T sum, int len}
//
// using P = RangeAddRangeSum<long long>;
// RBTreeSegment<P> t(3, P::value_type{1, 1});
// t.apply(1, 3, 2);   // {1, 3, 3}
// t.prod(0, 3).sum;   // 7
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
// monoid, policy, range add, range sum
// %=END DOC
// %=BEGIN CODE

template <class T>
struct RangeAddRangeSumValue {
  T sum;
  int len;
};

template <class T>
struct RangeAddRangeSum final : LazyMonoidPolicy<RangeAddRangeSumValue<T>, T> {
  using value_type = RangeAddRangeSumValue<T>;
  using operator_type = T;
  value_type operate(const value_type& a, const value_type& b) const override {
    return value_type{a.sum + b.sum, a.len + b.len};
  }
  value_type unit() const override { return value_type{T(0), 0}; }
  value_type mapping(const operator_type& f, const value_type& x) const override {
    return value_type{x.sum + f * x.len, x.len};
  }
  operator_type composition(const operator_type& f, const operator_type& g) const override {
    return f + g;
  }
  operator_type id() const override { return operator_type(0); }
};

// %=END CODE
#endif  // SRC_CPP_CONTAINER_POLICY_MONOID_POLICY_IMPL_RANGE_ADD_RANGE_SUM_HPP__
