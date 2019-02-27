// %title
// warshall-floyd
// 
// %overview
// 全ペアの最短経路を求める．
// [note] 辺が無い頂点ペアの距離はinfで埋めておくこと．
//
// %usage
// void warshall_floyd(Graph2d& g)
//
// %verified
// 
// %references
//
// %require
// cpp/graph/datastructure/graph2d.cpp

void warshall_floyd(Graph2d& g) {
    int i, j, k;
    for (i = 0; i < g.n; i++) {
        for (j = 0; j < g.n; j++) {
            for (k = 0; k < g.n; k++) {
                g(j, k) = min(g(j, k), g(j, i) + g(i, k));
            }
        }
    }
}

