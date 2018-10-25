// %title
// 組合せ計算(前計算)
// 
// %overview
// inverse, conbination, permutation, factorial を構築時に計算する
// constexprを付けられないことは無い
// 
//
// %usage
// Combinatorics<2000100, MD> wakame;
//
// %words
// fact,ncr,npr,nck,npk
// %verified
// https://yukicoder.me/submissions/286997
// %references



template<int MaxN, long long Mod>
class Combinatorics {
    using value_type = long long;
    value_type fact_[MaxN + 1];
    // value_type inv_[MaxN + 1]; // iranai

public:
    value_type pow(value_type x, value_type p) {
        value_type y = 1; x = x % Mod;
        for (; 0 < p; p >>= 1) {
            if (p & 1) y = y * x % Mod;
            x = x * x % Mod;
        }
        return y;
    }

    Combinatorics() :fact_() {
        fact_[0] = 1;
        fact_[1] = 1;
        for (value_type n = 2; n <= MaxN; ++n)
            fact_[n] = (fact_[n - 1] * n) % Mod;
        // inv_[Max] := calculate;
        // iterate => inv_[i] = inv_[i+1] * (i+1) % MD;
    }
    inline value_type fact(int n) const {
        return fact_[n];
    }

    inline value_type inv(value_type n) {
        return pow(n, MD - 2);
    }

    inline value_type nPr(value_type n, value_type r) {
        if (r < 0 || n < r) return 0;
        return fact_[n] * inv(fact_[n - r]) % MD;
    }
    inline value_type nCr(value_type n, value_type r) {
        if (n < r) return 0;
        return ((fact_[n] * inv(fact_[n - r]) % MD) * inv(fact_[r])) % MD;
    }
    inline value_type nHr(value_type n, value_type r) {
        if (n == 0 && r == 0) return 1;
        if (n == 0) return 0;
        return nCr(n - 1 + r, n - 1);
    }
};
