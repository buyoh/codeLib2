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
#include <vector>
using namespace std;
#include "src/cpp/grid/datastructure/euclid.hpp"
// ```
// %verified
// yukicoder5007
//
// %references
// %words
// tsp 2opt
// %=END DOC
// %=BEGIN CODE

ll swap2opt(const F<ll>& dist, int p, int q, int pb, int qa) {
  return dist(pb, q) + dist(p, qa) - dist(pb, p) - dist(q, qa);
}

vector<int> solveTSP2opt(const F<ll>& dist) {
  const int N = dist.width;
  vector<int> sequence(N);
  iota(all(sequence), 0);

  ll current_distance = dist(sequence[0], sequence[N - 1]);
  repeat(i, N - 1) {
    current_distance += dist(sequence[i], sequence[i + 1]);
  }

  clog << current_distance << endl;
  repeat(_, 9999) {
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
