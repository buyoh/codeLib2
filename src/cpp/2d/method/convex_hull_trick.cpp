int ccw(P a, P b, P c) {
  b.x() -= a.x();
  b.y() -= a.y();
  c.x() -= a.x();
  c.y() -= a.y();
  if (b.x() * c.y() - b.y() * c.x() > 0)
    return +1;  // counter clockwise
  if (b.x() * c.y() - b.y() * c.x() < 0)
    return -1;  // clockwise
  if (b.x() * c.x() + b.y() * c.y() < 0)
    return +2;  // c--a--b on line
  if (b.hypot() < c.hypot())
    return -2;  // a--b--c on line
  return 0;
}

ll m, n, kei;

pair<P, int> points_uq[111];

double len[111], lensum;

int main() {
  cin >> n;
  if (n == 2)
    bye("0.5\n0.5");

  repeat(i, n) {
    double x, y;
    cin >> x >> y;
    points_uq[i] = make_pair(P(x, y), i);
    // points[i] = P(hypot(x, y), atan2(x, y));
  }

  sort(points_uq, points_uq + n);

  // http://www.prefield.com/algorithm/geometry/convex_hull.html
  vector<pair<P, int>> ch(2 * n);
  int k = 0;
  for (int i = 0; i < n; ch[k] = points_uq[i], ++k, ++i)  // lower-hull
    while (k >= 2 && ccw(ch[k - 2].first, ch[k - 1].first, points_uq[i].first) <= 0)
      --k;
  for (int i = n - 2, t = k + 1; i >= 0; ch[k] = points_uq[i], ++k, --i)  // upper-hull
    while (k >= t && ccw(ch[k - 2].first, ch[k - 1].first, points_uq[i].first) <= 0)
      --k;
  ch.resize(k - 1);

  debugv(ch);

  m = ch.size();

  repeat(i, m) {
    int j = (i + 1) % m;
    int h = (i - 1 + m) % m;
    P a = ch[h].first - ch[i].first;
    P b = ch[i].first - ch[j].first;

    // double l = ch[i].first.hypot(ch[j].first);
    // len[ch[i].second] += l / 2;
    // len[ch[j].second] += l / 2;
    // lensum += l;

    len[ch[i].second] += acos(a.dot(b) / (a.hypot() * b.hypot()));
    lensum = PI * 2;
  }

  repeat(i, n) { printf("%.10f\n", len[i] / lensum); }

  return 0;
}