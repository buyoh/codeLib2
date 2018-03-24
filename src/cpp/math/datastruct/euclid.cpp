

struct P {
    using T = int;
    T y, x;

    P(T _y, T _x) :y(_y), x(_x) {}

    inline bool operator == (P p) const { return y == p.y && x == p.x; }
    inline bool operator < (P p) const { return y == p.y ? x < p.x : y < p.y; }
    inline P operator+(P p) const { return P(y + p.y, x + p.x); }
    inline P operator-(P p) const { return P(y - p.y, x - p.x); }
    inline P operator+=(P p) { y += p.y; x += p.x; return *this; }
    inline P operator-=(P p) { y -= p.y; x -= p.x; return *this; }
};

template<typename T>
// using T = int;
struct F {
    int height, width;
    vector<T> data;

    F(int h, int w) :height(h), width(w), data(h*w) {}

    inline T& operator()(int y, int x) { return data[x + y * height]; }
    inline T& operator()(P p) { return data[p.x + p.y * height]; }
    inline T operator()(int y, int x) const { return data[x + y * height]; }
    inline T operator()(P p) const { return data[p.x + p.y * height]; }

    inline void fill(T e) { std::fill(ALL(data), e); }

    F<int> distance(const vector<P>& start_point, function<bool(T)> ismoveable) {

    }
};

