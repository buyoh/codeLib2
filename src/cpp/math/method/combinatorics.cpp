// %title
// 組合せ計算
// 
// %overview
// 
//
// %usage
// T pow_md(T x, T p, T mod = MD)
// ; xのp乗をmodで割ったもの．時間O(logp)
// T fact_md(T n, T mod = MD)
// ; n!をmodで割ったもの．時間O(n)
// T nPr_md(T n, T r, T mod = MD)
// T nCr_md(T n, T r, T mod = MD)
// T nHr_md(T n, T r, T mod = MD)
// 
//
// %words
// pow,fact,ncr,npr,nhr,nck,npk,nhk
// %verified
// 
// %references


// MEMO : 逆元...pow_md(n,MOD-2,MOD)
template<typename T> T pow_md(T x, T p, T mod = MD) {
    T y = 1; x = x%mod;
    for (;0 < p; p >>= 1) {
        if (p & 1) y = y*x % mod;
        x = x*x % mod;
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
    if (n < 0 || r < 0 || n < r) return 0;
    if (n / 2 < r) return nCr_md(n, n - r, mod);
    T num = 1, den = 1;
    for (; 0 < r; --r, --n)
        num = num*n % mod, den = den*r % mod;
    return num * pow_md(den, mod - 2, mod) % mod;
}
template<typename T> inline T nHr_md(T n, T r, T mod = MD) {
    return nCr_md(n - 1 + r, n - 1, mod);
}

template<typename T> T nCr(T n, T r) {
    if (n / 2 < r) return nCr(n, n - r);
    T p = 1; for (T t = 1; t <= r; ++t) p = p * (n - t + 1) / t;
    return p;
}
template<typename T> inline T nHr(T n, T r) {
    return nCr(n - 1 + r, n - 1);
}
