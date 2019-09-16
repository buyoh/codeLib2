// %title
// Unionfind
//
// %overview
// 頂点数N，辺∅のグラフがある．
// グラフに対して，辺の追加と同じ連結成分かどうかの判定を行う．
//
// %usage
// bool connect(int x, int y)
// @ret : 元々繋がっていたならばno，
// bool same(int x, int y)
// int root(int x)
// int size(int x)
//
// %verified
//
// %references
//

class Unionfind {
 public:
  vector<int> data;
  Unionfind(size_t size) : data(size, -1) {}
  bool connect(size_t x, size_t y) {
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
  inline bool same(size_t x, size_t y) { return root(x) == root(y); }
  inline size_t root(size_t x) { return (size_t)(data[x] < 0 ? x : data[x] = root(data[x])); }
  inline int size(size_t x) { return -data[root(x)]; }
};
