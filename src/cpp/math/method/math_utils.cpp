// %title
// GCD,LCM,組み合わせ計算
// 
// %overview
// 必須の計算用関数．
// 組み合わせ計算については，事前にあらかじめ並列的に求めておくことで
// 時間計算量を抑えられる可能性がある．
//
// %usage
// T gcd(T a, T b)
// ; a,bの最大公約数
// T gcd(T a, T b)
// ; a,bの最小公倍数
// T pow_md(T x, T p, T mod = MD)
// ; xのp乗をmodで割ったもの．時間O(logp)
// T fact_md(T n, T mod = MD)
// ; n!をmodで割ったもの．時間O(n)
// T nPr_md(T n, T r, T mod = MD)
// T nCr_md(T n, T r, T mod = MD)
// T nHr_md(T n, T r, T mod = MD)
// 
//
// %verified
// 
// %references

template<typename T> inline T gcd(T a, T b) { while(b){T t=a%b;a=b;b=t;}return a; }
template<typename T> inline T lcm(T a, T b) { return a*b/gcd(a,b); }

// MEMO : 逆元...pow_md(n,MOD-2,MOD)
template<typename T> T pow_md(T x, T p, T mod = MD) {
    T y = 1; x = x%mod;
    while (0<p) {
        if (p & 1)
            y = (y*x) % mod;
        x = (x*x) % mod;
        p >>= 1;
    }
    return y;
}
template<typename T> T fact_md(T n, T mod = MD) {
    T p = 1; for (; 1<n; p = (p*n--) % mod); return p;
}
template<typename T> T nPr_md(T n, T r, T mod = MD) {
    T p = 1; for (r = n - r; r<n; p = (p*n--) % mod); return p;
}
template<typename T> T nCr_md(T n, T r, T mod = MD) {
    if (n / 2 < r) return nCr_md(n, n - r);
    T p = 1; for (; 0<r; r--)
        p = (((p*(n - r + 1)) % mod)*pow_md(r, mod - 2)) % mod;
    return p;
}
template<typename T> T nHr_md(T n, T r, T mod = MD) {
    return nCr_md(n - 1 + r, n - 1, mod);
}

// 1周をmodで表現するような角度の単位系で，
// 角度xから角度yへ向きを変えるとき，xに加算するべき値
inline double diffangle(double x, double y, double mod) {
    double a1 = fmod(y-x,mod), a2 = fmod(y-x+mod,mod), a3 = fmod(y-x-mod,mod);
    return abs(a2) < abs(a1) ? (abs(a3) < abs(a2) ? a3 : a2) : (abs(a3) < abs(a1) ? a3 : a1);
}
