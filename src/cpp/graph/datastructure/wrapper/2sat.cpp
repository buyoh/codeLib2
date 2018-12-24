// %title
// 2-SAT Solver
// 
// %overview
// 2-SATを解く．
// 強連結成分分解は内包済み．DGraphが必要．
//
// %usage
// Sat_2(size_t n)
// ; 
// void Sat_2::emplace(int a, int b)
// ; 節を追加する．節は2つのリテラルから構成される．
// ; リテラルの番号は1-indexed．
// ; 負のインデックスを指定すると，否定として扱われる．
// bool Sat_2::solve(vector<int>& result)
// @ret : 充足可能かどうか
// ; result[i]は，i番目のリテラルがTかFか．
// ; result[0]はdontcare
//
// %verified
// http://yukicoder.me/submissions/142141
// 
// %require
// cpp/graph/datastruct/dgraph.cpp
// 
// %references
// http://www.prefield.com/algorithm/misc/2-sat.html



class Sat_2 {
public:
    size_t n;
    DGraph graph;

    Sat_2(size_t n) :n(n), graph(n * 2 + 1) {}

private:
    inline int _cv(int v) { return 0<v ? v : -v + n; }

    void _scc_dfs1(int v, vector<int>& ord, vector<int> &num, int k) {
        if (0 <= num[v]) return;
        num[v] = k;
        for (int to : graph.vertex_to[v])
            _scc_dfs1(to, ord, num, k);

        ord.push_back(v);
    }
    void _scc_dfs2(int v, vector<int> &num, int k) {
        if (num[v] < 0) return;
        num[v] = -k;
        for (int to : graph.vertex_from[v])
            _scc_dfs2(to, num, k);
    }
public:

    // 1 <= a <= n OR -1 >= a >= -n
    // 正ならばx_a，負ならばNOT x_aを表現．
    inline void emplace(int a, int b) {
        // assert(a!=0 && b!=0);
        graph.connect(_cv(-a), _cv(b));
        graph.connect(_cv(-b), _cv(a));
    }

    bool solve(vector<int>& result) {
        vector<int> num(graph.n, -1), ord;
        ord.reserve(graph.n + 1);

        for (int i = 0; i<graph.n; ++i)
            _scc_dfs1(i, ord, num, i);

        for (int i = (int)ord.size() - 1; 0 <= i; --i)
            _scc_dfs2(ord[i], num, i + 1);

        result.resize(n + 1);
        for (int i = 1; i <= n; ++i) {
            if (num[i] == num[i + n])
                return false;
            result[i] = (num[i] < num[i + n]);
        }
        return true;
    }
};