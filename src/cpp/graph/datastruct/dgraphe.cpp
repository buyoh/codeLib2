// %title
// 有向グラフ(辺集合)
// 
// %overview
// 辺集合の情報を持つ．
// 辺には1つの値を持たせることができる．
// 必要な空間計算量は |V|+5|E|
//
// %usage
// void connect(int from, int to, W_T val = 0)
// ;つなげる．ループ辺は非推奨．
// void resize(size_t _n)
// ;頂点数を変更する．
// ;global変数として宣言した時に用いる．
//
// %verified
// 
// %references
//

class DGraphE {
public:
    typedef int W_T;
    struct Arc {
        int from, to;
        W_T value;
        Arc(int f = 0, int t = 0, W_T value = 0) :from(f), to(t), value(value) {}
        inline int pair(int _v) const { return _v == to ? from : to; }
    };
    size_t n;
    vector<vector<int>> vertex_to;
    vector<vector<int>> vertex_from;
    vector<Arc> arcs;

    DGraphE(int n = 1) :n(n), vertex_to(n), vertex_from(n) { }

    void resize(size_t _n) {
        n = _n;
        vertex_to.resize(_n);
        vertex_from.resize(_n);
    }
    void connect(int from, int to, W_T val = 0) {
        vertex_to[(size_t)from].push_back((int)arcs
            .size());
        vertex_from[(size_t)to].push_back((int)arcs
            .size());
        arcs
            .emplace_back(from, to, val);
    }
};
