// %title
// 2次元グリッド上のダイクストラ
//
// %overview
// 全てのセルに対してstartからの距離を求める
//
//
// %usage
//
//
// %verified
// https://yukicoder.me/submissions/294734
//
// %references
//
// %words
// grid, dij
// %require
// cpp/grid/datastructure/euclid.cpp

;
/// <summary>
/// 全てのセルに対してstartからの距離を求める
/// </summary>
/// <param name="height">gridの高さ</param>
/// <param name="width">gridの幅</param>
/// <param name="start">開始地点</param>
/// <param name="costFunc">costFunc(curr, dest):
/// 現在地点currから隣接点destに移動する時に掛かるコストを返す関数</param>
/// <returns>開始地点からの最短距離</returns>
F<ll> gridDistance(int height, int width, P start, function<ll(P, P)> costFunc) {
  priority_queue<pair<ll, P>> pque;

  F<ll> dist(height, width);
  dist.fill(numeric_limits<ll>::max());
  pque.emplace(0, start);
  dist(start) = 0;

  while (!pque.empty()) {
    auto dx = pque.top();
    pque.pop();
    dx.first = -dx.first;

    for (auto y : FourMoving) {
      y += dx.second;
      if (!dist.safe(y))
        continue;
      auto c = costFunc(dx.second, y);
      if (c >= 0 && dist(y) > dx.first + c) {
        dist(y) = dx.first + c;
        pque.emplace(-(dx.first + c), y);
      }
    }
  }

  return dist;
}