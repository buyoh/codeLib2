// %title
// ダイクストラ法(有向・辺重み)
// 
// %overview
//
// %usage
//
// %words
// dijkstra
// %require
// cpp/graph/datastructure/dgraphe.cpp
// %verified
// 
// %references
// 


vector<DGraphE::W_T> dijkstraDEdgeWeighted(const DGraphE& graph, int startIndex) {
    using T = DGraphE::W_T;
    vector<T> dist(graph.size(), numeric_limits<T>::max());

    priority_queue<pair<T, int>> que; // <dist, idx>
    que.emplace(0, startIndex);
    dist[startIndex] = 0;

    while (!que.empty()) {
        T d = -que.top().first;
        int v = que.top().second;
        que.pop();
        if (dist[v] < d) continue;

        for (int ei : graph.vertex_to[v]) {
            auto e = graph.arcs[ei];
            int u = e.to;
            if (d + e.value < dist[u]) {
                que.emplace(-(dist[u] = d + e.value), u);
            }
        }
    }
    return move(dist);
}