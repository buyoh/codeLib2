#ifndef TEST_COMMON_TESTUTIL_HPP__
#define TEST_COMMON_TESTUTIL_HPP__

#include <bits/stdc++.h>

using namespace std;
using ll = int64_t;

// util
#define all(v) (v).begin(), (v).end()
#define repeat(cnt, l) for (auto cnt = remove_reference<decltype(l)>::type(); (cnt) < (l); ++(cnt))
#define iterate(it, be, en) for (auto it = (be); it != (en); ++it)

// assertion
#define CHK(cond)                                               \
  if (!(cond)) {                                                \
    cerr << "in" << __LINE__ << "; CHK failed; " #cond << endl; \
    abort();                                                    \
  }
#define CHKEQ(expected, actual)                                                               \
  {                                                                                           \
    const auto& e = expected;                                                                 \
    const auto& a = actual;                                                                   \
    if (!(e == a)) {                                                                          \
      cerr << "in" << __LINE__ << "; CHK failed; " << #expected << " == " << #actual << endl; \
      _chkerr_eq_dump(e, a);                                                                  \
      abort();                                                                                \
    }                                                                                         \
  }

namespace {
template <typename E, typename A>
auto _chkerr_eq_dump(const E& e, const A& a) -> decltype(e << a, nullptr) {
  cerr << "left = " << e << ", right = " << a << endl;
  return nullptr;
}
auto _chkerr_eq_dump(...) -> decltype(nullptr) {
  return nullptr;
}
}  // namespace

namespace Rand {

mt19937_64 device(1341398);
template <typename T,
          typename Random = decltype(device),
          typename enable_if<is_integral<T>::value>::type* = nullptr>
inline T i(T l, T h, Random& rand = device) {
  return uniform_int_distribution<T>(l, h)(rand);
}
template <typename T,
          typename Random = decltype(device),
          typename enable_if<is_floating_point<T>::value>::type* = nullptr>
inline T r(T l, T h, Random& rand = device) {
  return uniform_real_distribution<T>(l, h)(rand);
}

}  // namespace Rand

#endif  // TEST_COMMON_TESTUTIL_HPP__
