// %title
// dinic(最大流ソルバー)
// 
// %overview
// LPの特殊な形．
//
// %usage
// void dinic(DGraphF &graph, vector<DGraphF::cap_t>& result, int i_source, int i_sink)
// graph    : ネットワーク
// result   : 各頂点にどれだけの水流が流れたか
// i_source : 源
// i_sink   : 流し台
//
// %verified
// [TODO] リファクタリング後のコードのverify
// https://yukicoder.me/submissions/208764
// 
// %references
// http://tubo28.me/algorithm/dinic/
// http://topcoder.g.hatena.ne.jp/Mi_Sawa/20140311


void dinic(DGraphF &graph, vector<DGraphF::cap_t>& result, int i_source, int i_sink) {
    assert(i_source != i_sink);

    result.resize(graph.n_);
    vector<int> dist(graph.n_);
    queue<int> q;
    vector<int> flag(graph.n_);

    static function<DGraphF::cap_t(int, int, DGraphF::cap_t)> _dfs = [&](int u, int i_sink, DGraphF::cap_t mini) {
        // DAG
        // TODO: 経路再利用
        if (i_sink == u) return mini;
        if (flag[u]) return (DGraphF::cap_t) - 1;
        flag[u] = true;

        DGraphF::cap_t sumw = 0;
        bool term = true;
        for (int e : graph.vertex_to[u]) {
            auto& edge = graph.edges[e];
            if (edge.left > 0 && dist[u]>dist[edge.to]) {
                DGraphF::cap_t w = (mini < 0) ? edge.left : min(edge.left, mini);

                w = _dfs(edge.to, i_sink, w);
                if (w == -1) continue;
                edge.left -= w;
                result[edge.to] += w;

                sumw += w;
                mini -= w;
                term = false;
                flag[u] = false; // TODO: 末尾では? 

                if (mini == 0) return sumw;
            }
        }
        for (int e : graph.vertex_from[u]) {
            auto& edge = graph.edges[e];
            if (edge.cap>edge.left && dist[u]>dist[edge.from]) {
                DGraphF::cap_t w = (mini < 0) ? (edge.cap - edge.left) : min(edge.cap - edge.left, mini);

                w = _dfs(edge.from, i_sink, w);
                if (w == -1) continue;
                edge.left += w;
                result[edge.to] -= w;

                sumw += w;
                mini -= w;
                term = false;
                flag[u] = false;
                if (mini == 0) return sumw;
            }
        }
        return term ? (DGraphF::cap_t)(-1) : sumw;
    };


    for (int distbegin = 0; ; distbegin += (int)graph.n_) {

        q.emplace(i_sink); // bfsはsinkからsourceへの距離を計算．
        dist[i_sink] = distbegin + 1;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int ie : graph.vertex_from[v]) {
                const auto edge = graph.edges[ie];
                if (0 < edge.left && dist[edge.from] <= distbegin) {
                    dist[edge.from] = dist[v] + 1;
                    q.emplace(edge.from);
                }
            }
            for (int ie : graph.vertex_to[v]) {
                const auto edge = graph.edges[ie];
                if (edge.left < edge.cap && dist[edge.to] <= distbegin) {
                    dist[edge.to] = dist[v] + 1;
                    q.emplace(edge.to);
                }
            }
        }
        fill(flag.begin(), flag.end(), false);

        if (dist[i_source] <= distbegin)
            break;
        else
            result[i_source] += _dfs(i_source, i_sink, -1);
    }
}
