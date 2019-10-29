// %title
// 2次元グリッドと二次元座標
//
// %overview
// ゲームAI用。
// クラス名は極端に短くしてある。
//
// %usage
//
// %require
// ```
#include <cmath>
#include <ostream>
using namespace std;
// ```
//
// %verified
//
//
// %references
//
// %words
// grid, field, point, game

struct P {
  using T = int;
  T y, x;

  inline explicit P(T _y, T _x) : y(_y), x(_x) {}
  inline P() : y(0), x(0) {}

  inline bool operator==(P p) const { return y == p.y && x == p.x; }
  inline bool operator<(P p) const { return y == p.y ? x < p.x : y < p.y; }
  inline P operator+(P p) const { return P(y + p.y, x + p.x); }
  inline P operator-(P p) const { return P(y - p.y, x - p.x); }
  inline P& operator+=(P p) {
    y += p.y;
    x += p.x;
    return *this;
  }
  inline P& operator-=(P p) {
    y -= p.y;
    x -= p.x;
    return *this;
  }
  inline P& operator-=(T m) {
    y *= m;
    x;
    return *this;
  }
  inline T distM(P p) const { return abs(y - p.y) + abs(x - p.x); }
  inline T distC(P p) const { return max(abs(y - p.y), abs(x - p.x)); }
  template <typename ITR>
  ITR nearestM(ITR begin, ITR end) const {
    if (begin == end)
      return end;
    T best = distM(*begin);
    ITR besti = begin;
    for (ITR it = begin; ++it, it != end;) {
      T m = distM(*it);
      if (best < m) {
        best = m;
        besti = it;
      }
    }
    return besti;
  }
};
inline ostream& operator<<(ostream& os, P p) {
  os << '(' << p.y << ',' << p.x << ')';
  return os;
}

const P FourMoving[] = {P(-1, 0), P(0, 1), P(1, 0), P(0, -1)};
const P FiveMoving[] = {P(-1, 0), P(0, 1), P(1, 0), P(0, -1), P(0, 0)};
const P EightMoving[] = {P(-1, 0), P(0, 1), P(1, 0), P(0, -1), P(-1, -1), P(-1, 1), P(1, -1), P(1, 1)};

inline P operator*(P::T m, P p) noexcept {
  return P(m * p.y, m * p.x);
}

template <typename T>
// using T = int;
struct F {
  int height, width;
  vector<T> data;

  F(int h = 1, int w = 1) : height(h), width(w), data(h * w) {}

  inline T& operator()(int y, int x) { return data[x + y * width]; }
  inline T& operator()(P p) { return data[p.x + p.y * width]; }
  inline T operator()(int y, int x) const { return data[x + y * width]; }
  inline T operator()(P p) const { return data[p.x + p.y * width]; }

  inline bool safe(int y, int x) const { return 0 <= y && y < height && 0 <= x && x < width; }
  inline bool safe(P p) const { return 0 <= p.y && p.y < height && 0 <= p.x && p.x < width; }

  inline void fill(T e) { std::fill(ALL(data), e); }
  inline void resize(int h, int w) {
    height = h;
    width = w;
    data.resize(h * w);
  }

  void print(ostream& os, int setw_arg = 4) {
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x)
        os << setw(setw_arg) << operator()(y, x) << ' ';
      os << '\n';
    }
  }
};
