// %title
// 最小頂点被覆の数
// 
// %overview
// グラフの最小頂点被覆の大きさを求める．
// 最大独立集合，最大クリークに転用可能．
// 半分全列挙の実装だが，分岐限定法の方が効率が良いので実装し直す[TODO]
//
// %usage
// int vertex_cover(const Graph& graph)
//
// %verified
// http://buyoh.hateblo.jp/entry/2017/12/10/184345
// 
// %words
// vertexcover
// %require
// cpp/graph/datastruct/graph.cpp
// 
// %references
//


int vertex_cover_l(const Graph& graph) {
    ll n = graph.n;

    vector<int> choice(n, 0);
    repeat(i, n) {
        choice[n - i - 1] = 1;
        do {
            bool ok = true;
            repeat(j, n) {
                if (choice[j] == 0) {
                    for (int to : graph.vertex_to[j])
                        if (!choice[to]) {
                            ok = false; j = n;
                            break;
                        }
                }
            }
            if (ok) return i + 1;
        } while (next_permutation(ALL(choice)));
    }
    abort(); // あり得ない
}


inline int bitcount(int x) {
    return bitset<31>(x).count();
}

// 半分全列挙
int vertex_cover(const Graph& graph) {
    const int n = graph.n;
    const int n_A = n / 2;
    const int n_B = n - n_A;
    const int inf = 1e9;

    // 少なすぎるケースは回避
    if (n < 6) return vertex_cover_l(graph);

    // group A : i <  n_A
    // group B : i >= n_A

    // dp[S] : S \subseteq V_A， 
    vector<int> ans_partial_A(1 << n_A, 0);

    // Sが独立集合なら0，独立集合でないならinfとなるように初期状態を構成する．
    for (int i = 0; i < n_A; ++i) {
        int bit = 1 << i;
        for (int j : graph.vertex_to[i])
            if (j < n_A)
                ans_partial_A[bit | (1 << j)] = inf; // 頂点数2
    }
    for (int bit = 3; bit < 1 << n_A; ++bit) {
        if (bitcount(bit) <= 1) continue;
        for (int i = 0; i < n_A; ++i) {
            ans_partial_A[bit | (1 << i)] |= ans_partial_A[bit];
        }
    }

    // groupA の部分集合の解を全列挙
    // ここでは groupA と groupB のカットエッジの被覆は考えない．
    for (int bit = 1; bit < 1 << n_A; ++bit) {
        for (int i = 0; i < n_A; ++i) {
            if (bit & (1 << i)) continue;
            ans_partial_A[bit | (1 << i)] = min(
                ans_partial_A[bit | (1 << i)],
                ans_partial_A[bit] + 1
            );
        }
    }

    // dp[S] Sは独立集合ではない
    vector<int> not_independent_B(1 << n_B, 0);
    for (int _i = 0; _i < n_B; ++_i) {
        int bit = 1 << _i;
        for (int j : graph.vertex_to[n_A + _i])
            if (n_A <= j)
                not_independent_B[bit | (1 << (j - n_A))] = 1;
    }
    for (int bit = 3; bit < 1 << n_B; ++bit) {
        if (bitcount(bit) <= 1) continue;
        for (int _i = 0; _i < n_B; ++_i) {
            not_independent_B[bit | (1 << _i)] |= not_independent_B[bit];
        }
    }

    // dp[S] Sに隣接する頂点のbit配列
    vector<int> adjacent2B(1 << n_B, 0);
    for (int _i = 0; _i < n_B; ++_i) {
        int bit = 0;
        for (int j : graph.vertex_to[n_A + _i])
            if (j < n_A)
                bit |= 1 << j;
        adjacent2B[1 << _i] = bit;
    }
    for (int bit = 1; bit < 1 << n_B; ++bit) {
        for (int _i = 0; _i < n_B; ++_i) {
            adjacent2B[bit | (1 << _i)] |= adjacent2B[bit];
        }
    }

    // answer
    int best = inf;

    // groupB の部分集合を全列挙
    for (int bit = 0; bit < 1 << n_B; ++bit) {

        // choice_Bが頂点被覆でないなら，reject
        if (not_independent_B[((1 << n_B) - 1) ^ bit]) continue;

        int mask_A = (1 << n_A) - 1;
        int fix_A = 0;

        int adj = adjacent2B[((1 << n_B) - 1) ^ bit];
        for (int i = 0; i < n_A; ++i) {
            // choice_B で選んでいない頂点が，groupAと隣接するならば
            if (adj & (1 << i)) {
                // そのgroupAの頂点は必ず選択する．
                mask_A ^= 1 << i; // 頂点jは考慮しなくてよい
                ++fix_A;          // 必ず選択することにしたので
            }
        }

        best = min(best, ans_partial_A[mask_A] + bitcount(bit) + fix_A);
    }

    return best;
}

