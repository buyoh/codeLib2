// %title
// 強連結成分分解
// 
// %overview
// [TODO] validation
//
// %usage
// 
//
// %verified
// 
// %references
// %words
// scc
// %require
// cpp/graph/datastruct/dgraph.cpp



int strongly_connected_components(const DGraph& graph, Unionfind& result) {
    stack<int> s;
    int size = graph.n;
    vector<int> num(size), low(size);
    vector<int> flg(size);
    int count = 0;
    int n_components = graph.n;

    function<void(int)> dfs = [&](int idx) {
        low[idx] = num[idx] = ++count;
        s.push(idx);
        flg[idx] = true;

        for (int w : graph.vertex_to[idx]) {
            if (num[w] == 0) {
                dfs(w);
                low[idx] = min(low[idx], low[w]);
            }
            else if (flg[w]) { // ?
                low[idx] = min(low[idx], num[w]);
            }
        }
        if (low[idx] == num[idx]) {
            while (!s.empty()) {
                int w = s.top(); s.pop();
                flg[w] = false;
                if (idx == w) break;
                n_components -= result.connect(idx, w);
            }
        }
    };
    for (int i = 0; i < graph.n; ++i) {
        if (num[i] == 0) dfs(i);
    }
    return n_components;
}


class SCComps {
public:
    const DGraph& orig;
    DGraph sccg;
    vector<int> ori2scc;
    vector<vector<int>> scc2ori;

    SCComps(const DGraph& g) :orig(g), sccg(1), ori2scc(g.n, -1) { build(); }

    void build() {
        Unionfind uf(orig.n);
        strongly_connected_components(orig, uf);

        int n_vtx = 0;
        for (int i = 0; i < orig.n; ++i) {
            int r = uf.root(i);
            if (ori2scc[r] == -1) {
                ori2scc[r] = n_vtx++;
                scc2ori.emplace_back();
            }
            ori2scc[i] = ori2scc[r];
            scc2ori[ori2scc[i]].push_back(i);
        }
        sccg.resize(n_vtx);

        for (int i = 0; i < orig.n; ++i) {
            for (int to : orig.vertex_to[i]) {
                if (ori2scc[i] == ori2scc[to]) continue;
                sccg.connect(ori2scc[i], ori2scc[to]);
            }
        }
    }

    inline const vector<int>& vertex_to(int v) const { return sccg.vertex_to[v]; }
    inline const vector<int>& vertex_from(int v) const { return sccg.vertex_from[v]; }
    inline int size() const { return sccg.n; }

};