// %title
// オイラー路検出
// 
// %overview
// 要は一筆書き．
// 以前ブログ記事用に書いてそれっきりなのでなんとかする[TODO]
//
// %usage
// vector<int> find_eulerialTrail(const GraphE& graph)
// @ret : 辿り方．頂点番号，辺番号，頂点番号，…と交互に記述される．
//
// %verified
// [TODO]
// %references
// http://shonen9th.blog.fc2.com/blog-entry-147.html


vector<int> find_eulerialTrail(const GraphE& graph) {

    vector<int> group(graph.edges.size());

    int lastgroupNo = 0;

    for (int idx = 0; idx < graph.edges.size(); ++idx) {
        if (group[idx] != 0) continue;

        group[idx] = ++lastgroupNo;
        int tail = graph.edges[idx].v;
        int v = graph.edges[idx].u;

        bool running = true;
        while (running) {
            running = false;
            for (int ie : graph.vertex_to[v]) {
                if (group[ie] == 0) {
                    group[ie] = lastgroupNo;
                    v = graph.edges[ie].to(v);
                    running = true;
                    break;
                }
            }
        }
    }

    vector<int> result;

    vector<int> checked(lastgroupNo + 1);
    stack<int> ss;

    int tail = graph.edges[0].v;
    int v = graph.edges[0].u;
    ss.push(group[0]);
    checked[group[0]] = true;

    result.push_back(graph.edges[0].v);
    result.push_back(0);
    result.push_back(v);

    group[0] = -1;

    int stat = 1;

    // (ループの外に出た！)
    vector<pair<int, int>> dic(lastgroupNo + 1);

    for (int step = 1; stat != 0; ++step) {
        stat = 0;
        for (int ie : graph.vertex_to[v]) {
            if (group[ie] < 0) continue;

            int g = group[ie];

            if (!checked[g]) {
                checked[g] = true;
                ss.push(g);

                group[ie] = -1;
                v = graph.edges[ie].to(v);

                result.push_back(ie);
                result.push_back(v);

                stat = 2; break;
            }
            else {
                if (dic[g].first != step) {
                    dic[g].first = step;
                    dic[g].second = ie;
                    stat = 1;
                }
            }
        }
        if (stat == 1) {
            while (dic[ss.top()].first != step) {
                ss.pop();
                assert(!ss.empty()); // バグ対策用
            }

            int ie = dic[ss.top()].second;
            group[ie] = -1;
            v = graph.edges[ie].to(v);

            result.push_back(ie);
            result.push_back(v);
        }
    }

    return result;
}

