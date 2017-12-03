// %title
// エラトステネスの篩
// 
// %overview
// エラトステネスの篩
//
// %usage
// void gen_primelist(vector<int>& out, ll high);
// out  : 
// high : 
// `out[i]==1`ならばiは合成数
// `out.resize(high+1)` する．
//
// %verified
// 
// 
// %references
// 



void gen_primelist(vector<int>& out, ll high) {
    out.resize(high + 1);
    out[0] = out[1] = 1;
    ll sqh = (ll)sqrt(high);
    for (ll i = 2; i <= sqh; i++) {
        if (out[i] == 0) {
            for (ll j = i*i; j <= high; j += i) { // i*i
                out[j] = 1;
            }
        }
    }
}
