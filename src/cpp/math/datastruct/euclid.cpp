// %title
// 2次元グリッドと二次元座標
// 
// %overview
// 特記事項なし
//
// %usage
// 
//
// %verified
// 
// 
// %references
// 
// %words
// grid, field, point
// %require
// 


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

    void print(ostream& os, int setw_arg = 4) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x)
                os << setw(setw_arg) << operator()(y, x) << ' ';
            os << '\n';
        }
    }
};
