// %title
// 有向グラフ(最大流用)
// 
// %overview
// 辺集合の情報を持つ．
//
// %usage
//
// %verified
// 
// %references
//




class DGraphF {
public:
    typedef int cap_t;
    size_t n_;
    struct Arc {
        int from, to;
        // 残量
        cap_t left;
        // 容量
        cap_t cap;

        Arc(int from = 0, int to = 0, cap_t w = 1) :from(from), to(to), left(w), cap(w) {}
        inline bool operator<(const Arc& a) const { return (left != a.left) ? left < a.left : (left<a.left) | (cap<a.cap) | (from<a.from) | (to<a.to); }
        inline bool operator==(const Arc& a) const { return (from == a.from) && (to == a.to) && (left == a.left) && (cap == a.cap); }
    };
    vector<vector<int>> vertex_to;
    vector<vector<int>> vertex_from;
    vector<Arc> edges;

    DGraphF(int n = 1) :n_(n), vertex_to(n), vertex_from(n) { }

    void connect(int from, int to, cap_t left) {
        vertex_to[(size_t)from].push_back((int)edges.size()); // toto
        vertex_from[(size_t)to].push_back((int)edges.size()); // fromfrom
        edges.emplace_back(from, to, left);
    }

    inline size_t size() const {return n_;}
};
