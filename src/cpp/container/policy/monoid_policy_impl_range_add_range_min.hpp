#ifndef SRC_CPP_CONTAINER_POLICY_MONOID_POLICY_IMPL_RANGE_ADD_RANGE_MIN_HPP__
#define SRC_CPP_CONTAINER_POLICY_MONOID_POLICY_IMPL_RANGE_ADD_RANGE_MIN_HPP__
// %=BEGIN DOC
// %title
// モノイドの Policy 実装例(区間加算・区間最小値)
//
// %overview
// LazyMonoidPolicy の実装例．要素は T，作用素は T（加算する値）．
// 空の区間の集約値（単位元）は numeric_limits<T>::max()．
//
// %usage
// RangeAddRangeMin<T>
//
// RBTreeSegment<RangeAddRangeMin<long long>> t(vector<long long>{3, 1, 4});
// t.apply(0, 2, 10);  // {13, 11, 4}
// t.prod(0, 2);       // 11
//
// %require
// ```
#include <algorithm>
#include <limits>
using namespace std;
#include "src/cpp/container/policy/monoid_policy.hpp"
// ```
// %verified
//
// %references
//
// %words
// monoid, policy, range add, range min
// %=END DOC
// %=BEGIN CODE

template <class T>
struct RangeAddRangeMin final : LazyMonoidPolicy<T, T> {
  using value_type = T;
  using operator_type = T;
  value_type operate(const value_type& a, const value_type& b) const override { return min(a, b); }
  value_type unit() const override { return numeric_limits<value_type>::max(); }
  value_type mapping(const operator_type& f, const value_type& x) const override { return x + f; }
  operator_type composition(const operator_type& f, const operator_type& g) const override {
    return f + g;
  }
  operator_type id() const override { return operator_type(0); }
};

// %=END CODE
#endif  // SRC_CPP_CONTAINER_POLICY_MONOID_POLICY_IMPL_RANGE_ADD_RANGE_MIN_HPP__
