// %title
// エラトステネスの篩
// 
// %overview
// エラトステネスの篩
// constexpr版もある(IsPrimeC)W
//
// %usage
// vector<int> gen_primelist(ll high);
// ; high : 上限
// ; `out[i]==1`ならばiは!!素数!! (i<=1は未定義)
// 
// constexpr IsPrimeC<9999> ipc;
// ; `ipc[x] == true` なら，xは!!素数!!
// 
// constexpr PrimeListC<9999> plc;
// ; `plc[i]` i 番目の素数
// ; イテレータあるよ
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
    for (auto& x : out) out ^= 1;
    return out;
}


template<int Max = 2000>
class IsPrimeC {
    bool d_[Max + 1];

    template<typename T>
    static constexpr T cesqrt(T s) {
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


template<int Max = 2000>
class PrimeListC {
    int d_[Max];
public:
    constexpr PrimeListC() : d_() {
        int n = 1;
        d_[0] = 2;
        for (int x = 3; n < Max; ++x) {
            bool f = true;
            for (int i = 0; d_[i] * d_[i] <= x; ++i)
                if (x % d_[i] == 0) { f = false; break; }
            if (f) d_[n++] = x;
        }
    }
    constexpr inline int operator[](int x) const { return d_[x]; }
    template<int _Max>
    class iterator {
        const PrimeListC<_Max>& pl;
        int ptr = 0;
    public:
        constexpr iterator(const decltype(pl)& _pl, int _ptr = 0) :pl(_pl),ptr(_ptr) { }
        constexpr int operator*() const { return pl[ptr]; }
        constexpr iterator<_Max>& operator++() { ptr++; return *this; } // prefix
        constexpr inline bool operator!=(const iterator<_Max>& it) const { return ptr != it.ptr ? !(_Max < ptr && _Max < it.ptr) : false; }
        constexpr inline bool operator==(const iterator<_Max>& it) const { return ptr != it.ptr ? (_Max < ptr && _Max < it.ptr) : true; }
    };
    constexpr PrimeListC::iterator<Max> begin() const { return PrimeListC::iterator<Max>(*this, 0); }
    constexpr PrimeListC::iterator<Max> end() const { return PrimeListC::iterator<Max>(*this, Max); }
};
