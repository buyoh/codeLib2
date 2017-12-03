// %title
// Miller–Rabin primality test
// 
// %overview
// 素数判定する．
// 2^63でも素数判定可能だが，__int128型を使う．
//
// %usage
// bool isprime_mr(ll val) 
// x : N
// @ret : is N prime?
//
// %verified
// https://yukicoder.me/submissions/210977
// 
// %references
// https://qiita.com/gushwell/items/ff9ed83ba55350aaa369
// https://yukicoder.me/submissions/210680



powm_strict(ll x, ll p, ll mod=1000000007ll){
    typedef __int128_t ll128;
    ll y = 1;
    x = x%mod;
    while (0 < p) {
        if (p%2 == 1)
            y = (ll)((((ll128)y)*x)%mod);
        x = (ll)((((ll128)x)*x)%mod);
        p /= 2;
    }
    return y;
}

// Miller–Rabin primality test
bool isprime_mr(ll val) {
    typedef __int128_t ll128;
    static const ll test[12] = {2,3,5,7,11,13,17,19,23,29,31,37};
    if (val <= 1 || val % 2 == 0)
        return val == 2;
    for (auto t : test)
        if (val % t == 0)
            return val == t;
    if (val < test[11]*test[11])
        return true;
    ll d = val - 1, s = 0;
    while (!(d & 1)) { ++s; d >>= 1; } // d*2**s
    for (auto t : test) {
        ll z = powm_strict(t, d, val);
        if (z == 1 || z == val - 1)
            continue;
        for (ll r = 1; r < s; ++r) {
            z = (ll)((ll128)(z) * z % val);
            if (z == val - 1)
                goto l_isprime_mr_ct;
        }
        return false;
    l_isprime_mr_ct:;
    }
    return true;
}