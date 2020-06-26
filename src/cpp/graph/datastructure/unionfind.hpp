// %=BEGIN
// %title
// Unionfind
//
// %overview
// 頂点数N，辺∅のグラフがある．
// グラフに対して，辺の追加と同じ連結成分かどうかの判定を行う．int
//
// %usage
// bool connect(int x, int y)
// @ret : 元々繋がっていたならばno，
// bool same(int x, int y)
// int root(int x)
// int size(int x)
//
// %require
// ```
#include <vector>
#include <algorithm>
using namespace std;
// ```
// %verified
//
// %references
//
// %=END

class Unionfind {
 public:
  vector<int> data;
  explicit Unionfind(int size) : data(size, -1) {}
  bool connect(int x, int y) {
    x = root(x);
    y = root(y);
    if (x != y) {
      if (data[y] < data[x])
        swap(x, y);
      data[x] += data[y];
      data[y] = (int)x;
    }
    return x != y;
  }
  inline bool same(int x, int y) { return root(x) == root(y); }
  inline int root(int x) { return (int)(data[x] < 0 ? x : data[x] = root(data[x])); }
  inline int size(int x) { return -data[root(x)]; }
};
