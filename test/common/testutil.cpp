#include <bits/stdc++.h>

using namespace std;
using ll = int64_t;

// util
#define all(v) (v).begin(), (v).end()
#define repeat(cnt, l) for (auto cnt = remove_reference<decltype(l)>::type(); (cnt) < (l); ++(cnt))

// assertion
#define CHK(cond)                                               \
  if (!(cond)) {                                                \
    cerr << "in" << __LINE__ << "; CHK failed; " #cond << endl; \
    abort();                                                    \
  }
#define CHKEQ(expected, actual)                                                   \
  if (!((expected) == (actual))) {                                                \
    cerr << "in" << __LINE__ << "; CHK failed; " #expected " = " #actual << endl; \
    _chkerr_eq_dump(expected, actual, #expected, #actual);                        \
    abort();                                                                      \
  }

namespace {
template <typename E, typename A>
auto _chkerr_eq_dump(const E& e, const A& a, const char* ename, const char* aname) -> decltype(e << a, nullptr) {
  cerr << ename << " = " << e << ", " << aname << " = " << a << endl;
}
auto _chkerr_eq_dump(...) -> decltype(nullptr) {}
}  // namespace

namespace Rand {

mt19937_64 device(1341398);
template <typename T, typename Random = decltype(device), typename enable_if<is_integral<T>::value>::type* = nullptr>
inline T i(T l, T h, Random& rand = device) {
  return uniform_int_distribution<T>(l, h)(rand);
}
template <typename T, typename Random = decltype(device), typename enable_if<is_floating_point<T>::value>::type* = nullptr>
inline T r(T l, T h, Random& rand = device) {
  return uniform_real_distribution<T>(l, h)(rand);
}

}  // namespace Rand