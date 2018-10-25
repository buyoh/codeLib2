// %title
// 組合せ計算(コンパイル時前計算)[deprecated]
// 
// %overview
// inverse, conbination, permutation, factorial をコンパイル時に計算する．
// 
//
// %usage
// constexpr StaticCombination<1000, 1000, MD> sncr;
// constexpr StaticFactorial<1000, MD> ssf;
// constexpr StaticPermutation<1000, 1000, MD> snpr;
// constexpr StaticInverse<1000, MD> si;
// ; 該当する構造体をconstexprを付けて宣言．
//
// %words
// fact,ncr,npr,nck,npk
// %verified
// 
// %references


template<int MaxN, int MaxR, long long Mod>
struct StaticCombination {
    using value_type = long long;
    value_type table[MaxN+1][MaxR+1];

    constexpr StaticCombination() :table() {
        for (int n = 0; n < MaxN; ++n) {
            table[n][0] = 1;
            for (int r = 0; r <= n && r < MaxR; ++r)
                table[n + 1][r + 1] = (table[n][r + 1] + table[n][r]) % Mod;
        }
    }

    constexpr inline value_type operator()(int n, int r) const {
        return table[n][r];
    }
};


template<int MaxN, int MaxR, long long Mod>
struct StaticPermutation {
    using value_type = long long;
    value_type table[MaxN + 1][MaxR + 1];

    constexpr StaticPermutation() :table() {
        for (value_type n = 0; n < static_cast<value_type>(MaxN); ++n) {
            table[n][0] = 1;
            for (value_type r = 0; r <= n && r < static_cast<value_type>(MaxR); ++r)
                table[n][r + 1] = (table[n][r]*(n-r)) % Mod;
        }
    }

    constexpr inline value_type operator()(int n, int r) const {
        return table[n][r];
    }
};


template<int MaxN, long long Mod>
struct StaticFactorial {
    using value_type = long long;
    value_type table[MaxN + 1];

    constexpr StaticFactorial() :table() {
        table[0] = 1;
        table[1] = 1;
        for (value_type n = 2; n <= MaxN; ++n)
            table[n] = (table[n - 1] * n) % Mod;
    }
    constexpr inline value_type operator()(int n) const {
        return table[n];
    }
};


template<long long MaxN, long long Mod>
struct StaticInverse {
    using value_type = long long;
    value_type table[MaxN + 1];

    static constexpr value_type inv(value_type x) {
        value_type y = 1; x = x % Mod;
        for (ll p = Mod - 2; 0 < p; p >>= 1) {
            if (p & 1) y = y * x % Mod;
            x = x * x % Mod;
        }
        return y;
    }

    constexpr StaticInverse() :table() {
        for (value_type n = 0; n <= MaxN; ++n)
            table[n] = inv(n);
    }
    constexpr inline value_type operator()(int n) const {
        return table[n];
    }
};
