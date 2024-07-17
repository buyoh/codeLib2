#ifndef SRC_CPP_GRAPH_METHOD_2opt_HPP__
#define SRC_CPP_GRAPH_METHOD_2opt_HPP__

// %=BEGIN DOC
// %title
// TSP solver 2opt
//
// %overview
// 2-opt 法で近似解を焼きなます。
// 無向グラフのみ利用可能。逆順でも距離が変わらないもの。
//
// %usage
//
// %require
// ```
#include <algorithm>
#include <numeric>
#include <random>
#include <vector>
using namespace std;
#include "src/cpp/grid/datastructure/euclid.hpp"
using ll = long long;
mt19937_64 randdev(8901016);
template <typename T,
          typename Random = decltype(randdev),
          typename enable_if<is_integral<T>::value>::type* = nullptr>
inline T rand(T l, T h, Random& rand = randdev) {
  return uniform_int_distribution<T>(l, h)(rand);
}
template <typename T,
          typename Random = decltype(randdev),
          typename enable_if<is_floating_point<T>::value>::type* = nullptr>
inline T rand(T l, T h, Random& rand = randdev) {
  return uniform_real_distribution<T>(l, h)(rand);
}
template <typename T>
// ```
// %verified
// yukicoder5007
//
// %references
// %words
// tsp 2opt
// %=END DOC
// %=BEGIN CODE

ll swap2opt(const F<ll>& dist, int p, int q, int pb, int qa);
ll swap2opt(const F<ll>& dist, int p, int q, int pb, int qa) {
  return dist(pb, q) + dist(p, qa) - dist(pb, p) - dist(q, qa);
}

vector<int> solveTSP2opt(const F<ll>& dist) {
  const int N = dist.width;
  vector<int> sequence(N);
  iota(sequence.begin(), sequence.end(), 0);

  ll current_distance = dist(sequence[0], sequence[N - 1]);
  for (int i = 0; i < N - 1; ++i) {
    current_distance += dist(sequence[i], sequence[i + 1]);
  }

  for (int _ = 0; _ < 9999; ++_) {
    // repeat(_, 100) {
    // do not change startp
    int a = rand(1, N - 2);
    int b = rand(2, N - 1);
    if (a == b)
      b += 1;
    if (a > b)
      swap(a, b);

    int p = sequence[a];
    int q = sequence[b];
    int pb = sequence[a == 0 ? N - 1 : a - 1];
    int qa = sequence[b == N - 1 ? 0 : b + 1];

    ll dd = swap2opt(dist, p, q, pb, qa);
    if (dd < 0) {
      if (a < b)
        reverse(sequence.begin() + a, sequence.begin() + b + 1);
      else
        reverse(sequence.begin() + b, sequence.begin() + a + 1);
      current_distance += dd;
    }
  }
  return sequence;
}
// %=END CODE
#endif  // SRC_CPP_GRAPH_METHOD_2opt_HPP__
