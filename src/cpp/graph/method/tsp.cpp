// %title
// 巡回セールスマン問題(bitDP)
//
// %overview
// 巡回セールスマン問題をbitDPで解く．
// 三角不等式を満たしていなくても解ける．
// TODO
//
// %usage
// void connect(int from, int to, W_T dist = 1)
// ;つなげる．ループ辺は非推奨．
// void resize(int _n)
// ;頂点数を変更する．
// ;global変数として宣言した時に用いる．
// inline W_T& operator()(int y, int x);
// ;行列の値の参照
//
// %require
// ```
#include <vector>
#include <algorithm>
using namespace std;
#include "src/cpp/graph/datastructure/graph2d.cpp"
// ```
// %words
// tsp
// %verified
// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2588066#1
// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2641075#1
// %references
//
// %require
// cpp/graph/datastructure/graph2dcpp

vector<int> tsProblem(int n, const Graph2d& graph) {
  using dist_T = long long;
  using ull = unsigned long long;

  if (n <= 1)
    return {0};
  if (n == 2)
    return {0, 1};

  vector<vector<dist_T>> dp;
  ull bit;

  // initialize
  dp.resize(n);
  for (auto& v : dp)
    v.resize(1 << n), fill(v.begin(), v.end(), (dist_T)5e15);

  // bitdp
  dp[0][1] = 0;
  for (int bit_i = 1; bit_i < (1 << n) - 1; ++bit_i) {
    bit = bit_i;

    for (int from = 0; from < n; ++from) {
      if (!(bool)(bit & (1ull << from)))
        continue;
      dist_T pt = dp[from][bit_i];

      for (int to = 0; to < n; ++to) {
        if ((bool)(bit & (1ull << to)))
          continue;
        dp[to][bit_i | (1ull << to)] = min(dp[to][bit_i | (1 << to)], pt + graph(from, to));
      }
    }
  }
  // 末尾を探す
  dist_T best = 5e15;
  int tail = 0;
  for (int i = 0; i < n; ++i)
    if (dp[i][(1 << n) - 1] + graph(i, 0) < best)
      best = dp[i][(1 << n) - 1] + graph(i, 0), tail = i;

  // 復元
  vector<int> tourist;
  tourist.reserve(n);
  bit = (1ull << n) - 1ull;
  for (int z = 0; z < n - 1; ++z) {
    ull b = bit;
    tourist.push_back(tail);
    bit ^= bit & (1ull << tail);
    ull c = bit;

    for (int nxt = 0; nxt < n; ++nxt) {
      if ((bit & (1ull << (ull)nxt)) && abs(dp[tail][b] - dp[nxt][c] - graph(nxt, tail)) == 0) {
        tail = nxt;
        break;
      }
    }
  }
  tourist.push_back(0);
  reverse(tourist.begin(), tourist.end());
  return tourist;
}