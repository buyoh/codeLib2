// %title
// ビット演算
// 
// %overview
// 1単位の整数に対して，立っているビットの数を数えたりする．
// コンパイラ組込み関数があれば，なるべくそれを使うようにする．
// 計算量は最悪でもlog(bitsize)時間．
//
// note: __lzcntはCPU依存なので実装から除外
//
// %usage
//
// %verified
// 乱数
// 
// %references
// http://naoyat.hatenablog.jp/entry/2014/05/12/143650
// http://www.nminoru.jp/~nminoru/programming/bitcount.html
// http://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
// https://docs.microsoft.com/ja-jp/cpp/intrinsics/alphabetical-listing-of-intrinsic-functions


namespace BitAlgo {
    // 1が立っている数(f(1) = 1, f(6) = 2)
    inline int popcount32(uint32_t v) noexcept {
#if defined(__GNUC__)
        return __builtin_popcount(v);
#elif defined(_MSC_VER)
        return __popcnt(v);
#else
        v = (v & 0x55555555) + (v >> 1 & 0x55555555);
        v = (v & 0x33333333) + (v >> 2 & 0x33333333);
        v = (v & 0x0f0f0f0f) + (v >> 4 & 0x0f0f0f0f);
        v = (v & 0x00ff00ff) + (v >> 8 & 0x00ff00ff);
        return (v & 0x0000ffff) + (v >> 16 & 0x0000ffff);
#endif
    }
    // 1が立っている数(f(1) = 1, f(6) = 2)
    inline int popcount64(uint64_t v) noexcept {
#if defined(__GNUC__)
        return __builtin_popcountll(v);
#elif defined(_MSC_VER)
        return __popcnt64(v);
#else
        v = (v & 0x5555555555555555ll) + (v >> 1 & 0x5555555555555555ll);
        v = (v & 0x3333333333333333ll) + (v >> 2 & 0x3333333333333333ll);
        v = (v & 0x0f0f0f0f0f0f0f0fll) + (v >> 4 & 0x0f0f0f0f0f0f0f0fll);
        v = (v & 0x00ff00ff00ff00ffll) + (v >> 8 & 0x00ff00ff00ff00ffll);
        v = (v & 0x0000ffff0000ffffll) + (v >> 16 & 0x0000ffff0000ffffll);
        return (v & 0x00000000ffffffffll) + (v >> 32 & 0x00000000ffffffffll);
#endif
    }
    // popcount % 2
    inline int parity32(uint32_t v) noexcept {
#if defined(__GNUC__)
        return __builtin_parity(v);
#else
        return popcount32(v) & 1;
#endif
    }
    // popcount % 2
    inline int parity64(uint64_t v) noexcept {
#if defined(__GNUC__)
        return __builtin_parityll(v);
#else
        return popcount32(v) & 1;
#endif
    }
    // 2進で表した場合に小さい方から何桁目に初めて1が現れるか
    // (f(3) = 1, f(8) = 4, f(0) = 0)
    // 関連：__builtin_ctz
    inline int ffs32(uint32_t v) noexcept {
#if defined(__GNUC__)
        return __builtin_ffs(v);
#else
        int c = 1;
        if (v == 0) return 0;
        if (!(v & 0xFFFF)) c += 16, v >>= 16;
        if (!(v & 0x00FF)) c += 8, v >>= 8;
        if (!(v & 0x000F)) c += 4, v >>= 4;
        if (!(v & 0x0003)) c += 2, v >>= 2;
        if (!(v & 0x0001)) c += 1;
        return c;
#endif
    }
    // 2進で表した場合に小さい方から何桁目に初めて1が現れるか
    // (f(3) = 1, f(8) = 4, f(0) = 0)
    // 関連：__builtin_ctzll
    inline int ffs64(uint64_t v) noexcept {
#if defined(__GNUC__)
        return __builtin_ffsll(v);
#else
        int c = 1;
        if (v == 0) return 0;
        if (!(v & 0xFFFFFFFFll)) c += 32, v >>= 32;
        if (!(v & 0x0000FFFFll)) c += 16, v >>= 16;
        if (!(v & 0x00FF)) c += 8, v >>= 8;
        if (!(v & 0x000F)) c += 4, v >>= 4;
        if (!(v & 0x0003)) c += 2, v >>= 2;
        if (!(v & 0x0001)) c += 1;
        return c;
#endif
    }
    // 末尾から続く0の個数
    // (f(2) = 30, f(0) = 32)
    inline int clz32(uint32_t x) noexcept {
#if defined(__GNUC__)
        return x == 0 ? 32 : __builtin_clz(x);
// #elif defined(_MSC_VER)
//         return __lzcnt(x); // _BitScanReverse
#else
        union {
            uint32_t i;
            float f;
        } data;
        data.f = (float)x + 0.5;
        return 158 - (data.i >> 23);
#endif
    }
    // 末尾から続く0の個数
    // (f(2) = 62, f(0) = 64)
    inline int clz64(uint64_t x) noexcept {
#if defined(__GNUC__)
        return x == 0 ? 64 : __builtin_clzll(x);
// #elif defined(_MSC_VER)
//         return __lzcnt64(x);
#else
        union {
            uint64_t i;
            double f;
        } datad;
        datad.f = (double)x + 0.5;
        return int(1086 - (datad.i >> 52));
#endif
    }
}
