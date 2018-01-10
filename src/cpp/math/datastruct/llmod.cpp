// %title
// 剰余演算付き整数型(llmod)
// 
// %overview
// MODを法として計算する．
//
// %usage
// [TODO]
//
// %verified
// verify: https://yukicoder.me/submissions/183521
// verify: https://yukicoder.me/submissions/183523
// 
// %references

class llmod {
private:
    ll val_;
    inline ll cut(ll v) const { return ((v%MOD) + MOD) % MOD; }
public:
    static const ll MOD = MD; // <= 

    llmod() : val_(0) {}
    llmod(ll num) :val_(cut(num)) {}
    llmod(const llmod& lm) : val_(lm.val_) {}

    inline operator ll() const { return val_; }
    inline ll operator *() const { return val_; }
    inline llmod& operator=(const llmod& lm) { val_ = lm.val_; return *this; }
    inline llmod& operator=(ll v) { val_ = cut(v); return *this; }

    inline llmod& operator+=(ll v) { val_ = cut(val_ + v); return *this; }
    inline llmod& operator+=(const llmod& l) { val_ = cut(val_ + l.val_); return *this; }
    inline llmod& operator-=(ll v) { val_ = cut(val_ - v); return *this; }
    inline llmod& operator-=(const llmod& l) { val_ = cut(val_ - l.val_); return *this; }
    inline llmod& operator*=(ll v) { val_ = cut(val_ * v); return *this; }
    inline llmod& operator*=(const llmod& l) { val_ = cut(val_ * l.val_); return *this; }
    inline llmod& operator++() { val_ = (val_ + 1) % MOD; return *this; }
    inline llmod operator++(int) { llmod t = *this; val_ = (val_ + 1) % MOD; return t; }
};
inline ostream& operator<<(ostream& os, const llmod& l) { os << *l; return os; }

inline llmod operator+(llmod t, const llmod& r) { return t += r; }
inline llmod operator-(llmod t, const llmod& r) { return t -= r; }
inline llmod operator*(llmod t, const llmod& r) { return t *= r; }



// MEMO : 逆元...powm(n,MD-2)
llmod pow(llmod x, ll p) {
    llmod y = 1;
    while (0 < p) {
        if (p % 2)
            y *= x;
        x *= x;
        p /= 2;
    }
    return y;
}

inline llmod& operator/=(llmod& l, const llmod& r) { return l *= pow(r, llmod::MOD - 2); }

