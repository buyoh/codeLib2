// %title
// algorithm name here
// 
// %overview
// my nice algorithm.
//
// %usage
// int func(int x);
// x : input
// @ret
//
// %verified
// problem.
// 
// %references
// thanks.
// %words
// bsearch, nibuntansaku
// %require
// cpp/graph/datastruct/dgraph.cpp
// 


struct P {
    using value_t = double;
    value_t x, y;
    P(value_t _x = 0, value_t _y = 0) :x(_x), y(_y) {  }
    inline void normalize() { if (x != 0 || y != 0) x /= hypot(), y /= hypot(); }
    inline value_t hypot() const { return std::hypot(x, y); }
    inline value_t hypot(P p) const { return std::hypot(x - p.x, y - p.y); }
    inline value_t dot(P p) const { return x * p.x + y * p.y; }
    inline value_t cross(P p) const { return x * p.y - y * p.x; }
    inline P normalized() const { return x != 0 || y != 0 ? P(x / hypot(), y / hypot()) : P(0, 0); }
    inline P operator+(P p) const { return P(x + p.x, y + p.y); }
    inline P operator-(P p) const { return P(x - p.x, y - p.y); }
    inline P& operator+=(P p) { x += p.x; y += p.y; return *this; }
    inline P& operator-=(P p) { x -= p.x; y -= p.y; return *this; }
};
P operator*(P::value_t a, P p) { return P(a*p.x, a*p.y); }


// http://www.prefield.com/algorithm/geometry/convex_hull.html
int ccw(P a, P b, P c) {
    b.x() -= a.x();
    b.y() -= a.y();
    c.x() -= a.x();
    c.y() -= a.y();
    if (b.x()*c.y() - b.y()*c.x() > 0)   return +1;       // counter clockwise
    if (b.x()*c.y() - b.y()*c.x() < 0)   return -1;       // clockwise
    if (b.x()*c.x() + b.y()*c.y() < 0)   return +2;       // c--a--b on line
    if (b.hypot() < c.hypot()) return -2;       // a--b--c on line
    return 0;
}

ll m, n, kei;

pair<P, int> points_uq[111];

double len[111], lensum;

int main() {
    cin >> n;
    if (n == 2) bye("0.5\n0.5");

    repeat(i, n) {
        double x, y;
        cin >> x >> y;
        points_uq[i] = make_pair(P(x, y), i);
        //points[i] = P(hypot(x, y), atan2(x, y));
    }

    sort(points_uq, points_uq + n);

    // http://www.prefield.com/algorithm/geometry/convex_hull.html
    vector<pair<P, int>> ch(2 * n);
    int k = 0;
    for (int i = 0; i < n; ch[k] = points_uq[i], ++k, ++i) // lower-hull
        while (k >= 2 && ccw(ch[k - 2].first, ch[k - 1].first, points_uq[i].first) <= 0) --k;
    for (int i = n - 2, t = k + 1; i >= 0; ch[k] = points_uq[i],++k, --i) // upper-hull
        while (k >= t && ccw(ch[k - 2].first, ch[k - 1].first, points_uq[i].first) <= 0) --k;
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

        len[ch[i].second] += acos(a.dot(b) / (a.hypot()*b.hypot()));
        lensum = PI*2;
    }

    repeat(i, n) {
        printf("%.10f\n", len[i] / lensum);
    }

    return 0;
}