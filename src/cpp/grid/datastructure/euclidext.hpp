#ifndef SRC_CPP_GRID_DATASTRUCTURE_EUCLIDEXT_HPP__
#define SRC_CPP_GRID_DATASTRUCTURE_EUCLIDEXT_HPP__
// %=BEGIN DOC
// %title
// グリッドと座標(拡張)
//
// %overview
// ゲームAI用。
// クラス名は極端に短くしてある。
// 3次元座標、正方形、立方体を実装。
//
// %usage
//
// %require
// ```
#include <cmath>
#include <ostream>
#include <iomanip>
#include <iostream>
#include <vector>
#include "src/cpp/grid/datastructure/euclid.hpp"
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
// %=END DOC
// %=BEGIN CODE

struct P3 {
  using T = int;
  T z, y, x;

  inline explicit P3(T _z, T _y, T _x) : z(_z), y(_y), x(_x) {}
  inline P3() : z(0), y(0), x(0) {}

  inline bool operator==(P3 p) const { return z == p.z && y == p.y && x == p.x; }
  inline bool operator<(P3 p) const { return z == p.z ? y == p.y ? x < p.x : y < p.y : z < p.z; }
  inline P3 operator+(P3 p) const { return P3(z + p.z, y + p.y, x + p.x); }
  inline P3 operator-(P3 p) const { return P3(z - p.z, y - p.y, x - p.x); }
  inline P3& operator+=(P3 p) {
    z += p.z;
    y += p.y;
    x += p.x;
    return *this;
  }
  inline P3& operator-=(P3 p) {
    z -= p.z;
    y -= p.y;
    x -= p.x;
    return *this;
  }
  inline P3& operator*=(T m) {
    y *= m;
    x *= m;
    return *this;
  }
  inline T distM(P3 p) const { return abs(z - p.z) + abs(y - p.y) + abs(x - p.x); }
  inline T distC(P3 p) const { return max(abs(z - p.z), max(abs(y - p.y), abs(x - p.x))); }
};
inline ostream& operator<<(ostream& os, P3 p) {
  os << '(' << p.z << ',' << p.y << ',' << p.x << ')';
  return os;
}

const P3 SixMoving[] = {P3(-1, 0, 0), P3(1, 0, 0),  P3(0, -1, 0),
                        P3(0, 1, 0),  P3(0, 0, -1), P3(0, 0, 1)};

template <typename T>
// using T = int;
struct Sq {
  int width;
  vector<T> data;

  explicit Sq(int w) : width(w), data(w * w) {}
  Sq() : Sq(1) {}

  inline bool safe(int y, int x) const { return 0 <= y && y < width && 0 <= x && x < width; }
  inline bool safe(P p) const { return 0 <= p.y && p.y < width && 0 <= p.x && p.x < width; }

#if 1
  void assert_safe(int y, int x) const {
    if (!safe(y, x)) {
      clog << "assertion failed: field=" << width << "x" << width << ": try=" << y << "," << x
           << endl;
      assert(safe(y, x));
    }
  }
  inline T& operator()(int y, int x) {
    assert_safe(y, x);
    return data[x + y * width];
  }
  inline T& operator()(P p) {
    assert_safe(p);
    return data[p.x + p.y * width];
  }
  inline T operator()(int y, int x) const {
    assert_safe(y, x);
    return data[x + y * width];
  }
  inline T operator()(P p) const {
    assert_safe(p);
    return data[p.x + p.y * width];
  }
#else
  inline T& operator()(int y, int x) {
    return data[x + y * width];
  }
  inline T& operator()(P p) {
    return data[p.x + p.y * width];
  }
  inline T operator()(int y, int x) const {
    return data[x + y * width];
  }
  inline T operator()(P p) const {
    return data[p.x + p.y * width];
  }
#endif
  inline T getA(int i) const {
    return data[i];
  }
  inline T& getAmut(int i) {
    return data[i];
  }

  inline void fill(T e) {
    std::fill(data.begin(), data.end(), e);
  }
  inline void resize(int w) {
    width = w;
    data.resize(w * w);
  }

  void print(ostream& os, int setw_arg = 4) {
    for (int y = 0; y < width; ++y) {
      for (int x = 0; x < width; ++x)
        os << setw(setw_arg) << operator()(y, x) << ' ';
      os << '\n';
    }
  }
};

template <typename T>
// using T = int;
struct Cu {
  int width;
  vector<T> data;

  explicit Cu(int w) : width(w), data(w * w * w) {}
  Cu() : Cu(1) {}

  inline bool safe(int z, int y, int x) const {
    return 0 <= z && z < width && 0 <= y && y < width && 0 <= x && x < width;
  }
  inline bool safe(P3 p) const {
    return 0 <= p.z && p.z < width && 0 <= p.y && p.y < width && 0 <= p.x && p.x < width;
  }
#if 1
  void assert_safe(int z, int y, int x) const {
    if (!safe(z, y, x)) {
      clog << "assertion failed: field=" << width << "x" << width << "x" << width << ": try=" << z
           << "," << y << "," << x << endl;
      assert(safe(z, y, x));
    }
  }
  inline T& operator()(int z, int y, int x) {
    assert_safe(z, y, x);
    return data[x + y * width + z * width * width];
  }
  inline T operator()(int z, int y, int x) const {
    assert_safe(z, y, x);
    return data[x + y * width + z * width * width];
  }
  inline T& operator()(P3 p) {
    assert_safe(p.z, p.y, p.x);
    return data[p.x + p.y * width + p.z * width * width];
  }
  inline T operator()(P3 p) const {
    assert_safe(p.z, p.y, p.x);
    return data[p.x + p.y * width + p.z * width * width];
  }
#else
  inline T& operator()(int z, int y, int x) {
    return data[x + y * width + z * width * width];
  }
  inline T operator()(int z, int y, int x) const {
    return data[x + y * width + z * width * width];
  }
  inline T& operator()(P3 p) {
    return data[p.x + p.y * width + p.z * width * width];
  }
  inline T operator()(P3 p) const {
    return data[p.x + p.y * width + p.z * width * width];
  }
#endif
  inline T getA(int i) const {
    return data[i];
  }
  inline T& getAmut(int i) {
    return data[i];
  }

  inline void fill(T e) {
    std::fill(data.begin(), data.end(), e);
  }
  inline void resize(int w) {
    width = w;
    data.resize(w * w * w);
  }

  // void print(ostream &os, int setw_arg = 4) {
  //   for (int y = 0; y < width; ++y) {
  //     for (int x = 0; x < width; ++x)
  //       os << setw(setw_arg) << operator()(y, x) << ' ';
  //     os << '\n';
  //   }
  // }
};
// %=END CODE
#endif  // SRC_CPP_GRID_DATASTRUCTURE_EUCLIDEXT_HPP__