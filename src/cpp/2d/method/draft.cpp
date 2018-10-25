
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


// http://shogo82148.github.io/homepage/memo/geometry/

/// 円と円の接点を求める．
tuple<bool, P, P> intersection_circle(P p1, double r1, P p2, double r2) {
    P p = p2 - p1;
    double d = p.x*p.x + p.y*p.y;
    double a = (d + r1 * r1 - r2 * r2) / 2;
    double f = d*r1*r1 - a;
    if (f < 0 || abs(d) < 1e-13) return make_tuple(false, P(), P());
    return make_tuple(true,
        P((a*p.x + f * p.y) / d, (a*p.y - f * p.x) / d),
        P((a*p.x - f * p.y) / d, (a*p.y + f * p.x) / d)
    );
}


// 点と線の距離と線上の最寄りの点を求める
// 線はlo+t*lvで表現する．tは実数．
tuple<double, P> distance_point_to_line(P p, P lo, P lv) {

}
