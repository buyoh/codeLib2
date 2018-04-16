// 2Dセグ木を書いたがなにか勘違いしているっぽい
// 愚直と対して実行時間の差がない



#pragma GCC optimize ("O3")
#pragma GCC target ("avx")
#include "bits/stdc++.h" // define macro "/D__MAI"

using namespace std;
typedef long long int ll;

#define debug(v) {printf("L%d %s > ",__LINE__,#v);cout<<(v)<<endl;}
#define debugv(v) {printf("L%d %s > ",__LINE__,#v);for(auto e:(v)){cout<<e<<" ";}cout<<endl;}
#define debuga(m,w) {printf("L%d %s > ",__LINE__,#m);for(int x=0;x<(w);x++){cout<<(m)[x]<<" ";}cout<<endl;}
#define debugaa(m,h,w) {printf("L%d %s >\n",__LINE__,#m);for(int y=0;y<(h);y++){for(int x=0;x<(w);x++){cout<<(m)[y][x]<<" ";}cout<<endl;}}
#define ALL(v) (v).begin(),(v).end()
#define repeat(cnt,l) for(auto cnt=0ll;(cnt)<(l);++(cnt))
#define rrepeat(cnt,l) for(auto cnt=(l)-1;0<=(cnt);--(cnt))
#define iterate(cnt,b,e) for(auto cnt=(b);(cnt)!=(e);++(cnt))
#define diterate(cnt,b,e) for(auto cnt=(b);(cnt)!=(e);--(cnt))
#define MD 1000000007ll
#define PI 3.1415926535897932384626433832795
template<typename T1, typename T2> ostream& operator <<(ostream &o, const pair<T1, T2> p) { o << "(" << p.first << ":" << p.second << ")"; return o; }
template<typename T> T& maxset(T& to, const T& val) { return to = max(to, val); }
template<typename T> T& minset(T& to, const T& val) { return to = min(to, val); }
void bye(string s, int code = 0) { cout << s << endl; exit(code); }
mt19937_64 randdev(8901016);
inline ll rand_range(ll l, ll h) {
    return uniform_int_distribution<ll>(l, h)(randdev);
}

#if defined(_WIN32) || defined(_WIN64)
#define getchar_unlocked _getchar_nolock
#define putchar_unlocked _putchar_nolock
#elif defined(__GNUC__)
#else
#define getchar_unlocked getchar
#define putchar_unlocked putchar
#endif
namespace {
#define isvisiblechar(c) (0x21<=(c)&&(c)<=0x7E)
    class MaiScanner {
    public:
        template<typename T> void input_integer(T& var) {
            var = 0; T sign = 1;
            int cc = getchar_unlocked();
            for (; cc<'0' || '9'<cc; cc = getchar_unlocked())
                if (cc == '-') sign = -1;
            for (; '0' <= cc && cc <= '9'; cc = getchar_unlocked())
                var = (var << 3) + (var << 1) + cc - '0';
            var = var * sign;
        }
        inline int c() { return getchar_unlocked(); }
        inline MaiScanner& operator>>(int& var) { input_integer<int>(var); return *this; }
        inline MaiScanner& operator>>(long long& var) { input_integer<long long>(var); return *this; }
        inline MaiScanner& operator>>(string& var) {
            int cc = getchar_unlocked();
            for (; !isvisiblechar(cc); cc = getchar_unlocked());
            for (; isvisiblechar(cc); cc = getchar_unlocked())
                var.push_back(cc);
            return *this;
        }
        template<typename IT> void in(IT begin, IT end) { for (auto it = begin; it != end; ++it) *this >> *it; }
    };
    class MaiPrinter {
    public:
        template<typename T>
        void output_integer(T var) {
            if (var == 0) { putchar_unlocked('0'); return; }
            if (var < 0)
                putchar_unlocked('-'),
                var = -var;
            char stack[32]; int stack_p = 0;
            while (var)
                stack[stack_p++] = '0' + (var % 10),
                var /= 10;
            while (stack_p)
                putchar_unlocked(stack[--stack_p]);
        }
        inline MaiPrinter& operator<<(char c) { putchar_unlocked(c); return *this; }
        inline MaiPrinter& operator<<(int var) { output_integer<int>(var); return *this; }
        inline MaiPrinter& operator<<(long long var) { output_integer<long long>(var); return *this; }
        inline MaiPrinter& operator<<(char* str_p) { while (*str_p) putchar_unlocked(*(str_p++)); return *this; }
        inline MaiPrinter& operator<<(const string& str) {
            const char* p = str.c_str();
            const char* l = p + str.size();
            while (p < l) putchar_unlocked(*p++);
            return *this;
        }
        template<typename IT> void join(IT begin, IT end, char sep = '\n') { for (auto it = begin; it != end; ++it) *this << *it << sep; }
    };
}
MaiScanner scanner;
MaiPrinter printer;


template<typename T>
// using T = int;
struct F {
    int height, width;
    vector<T> data;

    F(int h, int w) :height(h), width(w), data(h*w) {}

    inline T& operator()(int y, int x) { return data[x + y * height]; }
    inline T operator()(int y, int x) const { return data[x + y * height]; }

    inline void fill(T e) { std::fill(ALL(data), e); }

};



struct usoseg {
    F<int> field;

    usoseg(int h, int w) :field(h, w) {}

    inline void set_range(int sy, int sx, int ty, int tx) {
        for (int y = sy; y < ty; ++y)
            for (int x = sx; x < tx; ++x)
                field(y, x) = 1;
    }
    inline void reset_range(int sy, int sx, int ty, int tx) {
        for (int y = sy; y < ty; ++y)
            for (int x = sx; x < tx; ++x)
                field(y, x) = 0;
    }
    inline int count_range(int sy, int sx, int ty, int tx) {
        int c = 0;
        for (int y = sy; y < ty; ++y)
            for (int x = sx; x < tx; ++x)
                c += field(y, x);
        return c;
    }
};

int analy_counter = 0;

struct SegmentTreeSP {
    using volume_t = long long;

    struct Node {
        struct LZ {
            bool enable;
        };

        unique_ptr<Node> childlen[2][2];
        LZ lazy;
        volume_t data;

        Node() {}

        inline void _emplace_childlen() {
            if (!childlen[0][0]) childlen[0][0].reset(new Node()), analy_counter++;
            if (!childlen[0][1]) childlen[0][1].reset(new Node()), analy_counter++;
            if (!childlen[1][0]) childlen[1][0].reset(new Node()), analy_counter++;
            if (!childlen[1][1]) childlen[1][1].reset(new Node()), analy_counter++;
        }

        inline void _apply_lazy() {
            if (lazy.enable) {
                lazy.enable = false;
                _emplace_childlen();
                childlen[0][0]->data = data / 4;
                childlen[0][0]->lazy.enable = true;
                childlen[0][1]->data = data / 4;
                childlen[0][1]->lazy.enable = true;
                childlen[1][0]->data = data / 4;
                childlen[1][0]->lazy.enable = true;
                childlen[1][1]->data = data / 4;
                childlen[1][1]->lazy.enable = true;
            }
        }

        inline void _setup_lazy() {
            lazy.enable = true;
        }

        void set_range(int sy, int sx, int ty, int tx, int top, int left, int bottom, int right) {
            if (ty <= top || bottom <= sy || tx <= left || right <= sx) return;
            if (sy <= top && bottom <= ty && sx <= left && right <= tx) {
                data = volume_t(right - left)*volume_t(bottom - top);
                _setup_lazy();
                return;
            }
            if (data == volume_t(right - left)*volume_t(bottom - top))
                return ;

            int centerx = (right + left) / 2;
            int centery = (top + bottom) / 2;

            _apply_lazy();
            _emplace_childlen();
            childlen[0][0]->set_range(sy, sx, ty, tx, top, left, centery, centerx);
            childlen[0][1]->set_range(sy, sx, ty, tx, top, centerx, centery, right);
            childlen[1][0]->set_range(sy, sx, ty, tx, centery, left, bottom, centerx);
            childlen[1][1]->set_range(sy, sx, ty, tx, centery, centerx, bottom, right);
            // merge
            data
                = childlen[0][0]->data
                + childlen[0][1]->data
                + childlen[1][0]->data
                + childlen[1][1]->data;
        }

        void reset_range(int sy, int sx, int ty, int tx, int top, int left, int bottom, int right) {
            if (ty <= top || bottom <= sy || tx <= left || right <= sx) return;
            if (sy <= top && bottom <= ty && sx <= left && right <= tx) {
                data = 0;
                _setup_lazy();
                return;
            }
            if (data == 0)
                return;

            int centerx = (right + left) / 2;
            int centery = (top + bottom) / 2;

            _apply_lazy();
            _emplace_childlen();
            childlen[0][0]->reset_range(sy, sx, ty, tx, top, left, centery, centerx);
            childlen[0][1]->reset_range(sy, sx, ty, tx, top, centerx, centery, right);
            childlen[1][0]->reset_range(sy, sx, ty, tx, centery, left, bottom, centerx);
            childlen[1][1]->reset_range(sy, sx, ty, tx, centery, centerx, bottom, right);
            // merge
            data
                = childlen[0][0]->data
                + childlen[0][1]->data
                + childlen[1][0]->data
                + childlen[1][1]->data;
        }


        volume_t count_range(int sy, int sx, int ty, int tx, int top, int left, int bottom, int right) {
            if (ty <= top || bottom <= sy || tx <= left || right <= sx) return 0;
            _apply_lazy();
            if (sy <= top && bottom <= ty && sx <= left && right <= tx) {
                return data;
            }
            if (data == volume_t(right - left)*volume_t(bottom - top))
                return volume_t(min(tx, right) - max(sx, left))*volume_t(min(ty, bottom) - max(sy, top));
            if (data == 0)
                return 0;

            int centerx = (right + left) / 2;
            int centery = (top + bottom) / 2;

            _emplace_childlen();
            return 
                childlen[0][0]->count_range(sy, sx, ty, tx, top, left, centery, centerx)
                + childlen[0][1]->count_range(sy, sx, ty, tx, top, centerx, centery, right)
                + childlen[1][0]->count_range(sy, sx, ty, tx, centery, left, bottom, centerx)
                + childlen[1][1]->count_range(sy, sx, ty, tx, centery, centerx, bottom, right);

        }

    };

    unique_ptr<Node> root;
    int height, width;

    SegmentTreeSP(int _h, int _w) :root(new Node()) {
        height = 4;
        width = 4;
        while (height < _h) height <<= 1;
        while (width < _w) width <<= 1;
        root->data = 0;
    }


    inline void set_range(int sy, int sx, int ty, int tx) {
        root->set_range(sy, sx, ty, tx, 0, 0, height, width);
    }

    inline void reset_range(int sy, int sx, int ty, int tx) {
        root->reset_range(sy, sx, ty, tx, 0, 0, height, width);
    }

    inline volume_t count_range(int sy, int sx, int ty, int tx) {
        return root->count_range(sy, sx, ty, tx, 0, 0, height, width);
    }
};



ll m, n, kei;

int main() {
    int N = 2000;

    usoseg uso(N,N);
    SegmentTreeSP seg(N, N);

    repeat(lop, 100000) {
        int t = rand_range(0, 2);
        int a = rand_range(0, N - 1);
        int b = rand_range(0, N - 1);
        int c = rand_range(0, N - 1);
        int d = rand_range(0, N - 1);
        // cin >> t >> a >> b >> c >> d;
        if (a == c) continue;
        if (a > c) swap(a, c);
        if (b == d) continue;
        if (b > d) swap(b, d);
        if (t == 0) {
            // cout << "SET   " << setw(4) << a << setw(4) << b << setw(4) << c << setw(4) << d << endl;
            uso.set_range(a, b, c, d);
            seg.set_range(a, b, c, d);
        }
        if (t == 1) {
            // cout << "RESET " << setw(4) << a << setw(4) << b << setw(4) << c << setw(4) << d << endl;
            uso.reset_range(a, b, c, d);
            seg.reset_range(a, b, c, d);
        }
        if (t == 2) {
            // cout << "COUNT " << setw(4) << a << setw(4) << b << setw(4) << c << setw(4) << d << endl;
            ll u = uso.count_range(a, b, c, d);
            ll s = seg.count_range(a, b, c, d);
            // cout << ">> " << make_pair(u,s) << endl;
            assert(u == s);
        }
        pair<int, int>();
    }

    return 0;
}