// %title
// ダイクストラ法(無向・頂点重み)
// 
// %overview
//
// %usage
//
// %words
// dijkstra
// %require
// cpp/graph/datastructure/graph.cpp
// %verified
// 
// %references
// 


template<typename T, typename ITER>
vector<T> dijkstraVertexWeighted(const Graph& graph, int startIndex, const ITER& weight) {
    vector<T> dist(graph.size(), numeric_limits<int>::max());

    priority_queue<pair<T, int>> que; // <dist, idx>
    que.emplace(0, startIndex);
    dist[startIndex] = 0;

    while (!que.empty()) {
        GraphE::W_T d = -que.top().first;
        int v = que.top().second;
        que.pop();
        if (dist[v] < d) continue;

        for (int u : graph.vertex_to[v])
            if (d + weight[u] < dist[u])
                que.emplace(-(dist[u] = d + weight[u]), u);
    }
    return move(dist);
}

