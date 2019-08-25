// %title
// トポロジカルソート
// 
// %overview
// トポロジカルソートについては次のリンクを参照
// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_4_B&lang=jp
//
// %usage
// vector<int> topological_sort(const DGraph& graph)
// graph : 
// @ret  : 頂点集合0..N-1がトポロジカルソートされたもの．
// ; @ret.size() < N の時，入力のグラフがDAGでは無かったことを示す．
//
// %verified
// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2641044#1
// %require
// cpp/graph/datastructure/dgraph.cpp
// 
// %references
//

vector<int> topologicalSort(const DGraph& graph) {
    const int n = graph.n;
    vector<int> result; result.reserve(n);
    vector<int> start; start.reserve(n);
    vector<int> sync(n);

    for (int i = 0; i < n; ++i) {
        int x = graph.vertex_from[i].size();
        if (x == 0)
            start.push_back(i);
        else
            sync[i] = x;
    }

    function<void(int)> dfs = [&](int idx) {
        result.push_back(idx);
        for (int to : graph.vertex_to[idx]) {
            if (--sync[to] <= 0)
                dfs(to);
        }
    };
    for (int s : start) dfs(s);
    return result;
}
