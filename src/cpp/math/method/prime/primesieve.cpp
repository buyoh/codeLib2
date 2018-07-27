// %title
// エラトステネスの篩
// 
// %overview
// エラトステネスの篩
// constexpr版もある(IsPrimeC)W
//
// %usage
// vector<int> gen_primelist(ll high);
// ; high : 
// ; `out[i]==1`ならばiは!!合成数!!
// 
// constexpr IsPrimeC<2000> ipc;
// ; `ipc[x] == true` なら，xは!!素数!!
//
// %verified
// 
// 
// %references
// 



vector<int> gen_primelist(ll high) {
    vector<int> out;
    out.resize(high + 1);
    out[0] = out[1] = 1;
    ll sqh = (ll)sqrt(high);
    for (ll i = 2; i <= sqh; i++) {
        if (out[i] == 0) {
            for (ll j = i * i; j <= high; j += i) { // i*i
                out[j] = 1;
            }
        }
    }
    return out;
}


template<int Max = 2000>
class IsPrimeC {
    int d_[Max + 1];

    template<typename T>
    constexpr T cesqrt(T s) {
        double x = s / 2.0;
        double prev = 0.0;
        while (x != prev) {
            prev = x;
            x = (x + s / x) / 2.0;
        }
        return x;
    }
public:
    constexpr IsPrimeC(): d_() {
        d_[0] = d_[1] = 1;
        int sqh = cesqrt<int>(Max);
        for (int i = 2; i <= sqh; i++) {
            if (d_[i] == 0) {
                for (int j = i * i; j <= Max; j += i) { // i*i
                    d_[j] = 1;
                }
            }
        }
        for (int i = 0; i <= Max; ++i) d_[i] = !d_[i];
    }
    constexpr inline bool operator[](int x) const{return d_[x]; }
};
