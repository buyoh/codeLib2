// %title
// エラトステネスの篩(動的生成)
// 
// %overview
// エラトステネスの篩
//
// %usage
// vector<int> gen_primelist(ll high);
// ; high : 上限
// ; `out[i]==1`ならばiは!!素数!! (i<=1は未定義)
//
// %verified
// 
// 
// %references
// 

;
// deprecated
vector<int> gen_primelist(ll high) {
    vector<int> out;
    out.resize(high + 1);
    out[0] = out[1] = 1;
    for (ll i = 2; i*i <= high; i++) {
        if (out[i] == 0) {
            for (ll j = i * i; j <= high; j += i) { // i*i
                out[j] = 1;
            }
        }
    }
    for (auto& x : out) out ^= 1;
    return out;
}
