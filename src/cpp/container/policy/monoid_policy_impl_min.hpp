#ifndef SRC_CPP_CONTAINER_POLICY_MONOID_POLICY_IMPL_MIN_HPP__
#define SRC_CPP_CONTAINER_POLICY_MONOID_POLICY_IMPL_MIN_HPP__
// %=BEGIN DOC
// %title
// モノイドの Policy 実装例(最小値)
//
// %overview
// MonoidPolicy の実装例．min による（可換で冪等な）モノイド．
// 単位元は numeric_limits<T>::max()．
//
// %usage
// MinMonoid<T>
//
// SparseTableMonoid<MinMonoid<int>> st(vector<int>{3, 1, 4});
// st.prod(0, 2);  // 1
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
// monoid, policy, min
// %=END DOC
// %=BEGIN CODE

template <class T>
struct MinMonoid final : MonoidPolicy<T> {
  using value_type = T;
  value_type operate(const value_type& a, const value_type& b) const override { return min(a, b); }
  value_type unit() const override { return numeric_limits<value_type>::max(); }
};

// %=END CODE
#endif  // SRC_CPP_CONTAINER_POLICY_MONOID_POLICY_IMPL_MIN_HPP__
