// %title
// getchar,putcharによる入出力実装．
// 
// %overview
// _WIN64 OR _WIN32 環境なら _getchar_nolock を，
// __GNUC__         環境なら getchar_unlocked を，
// それ以外の環境なら getchar を使う．(codeforcesでも使える)
// 実数には対応しない．対応したとしてもscanf,printfでごまかします．
//
// stringを読み込む際，変数をクリアしない．
// これを使って，入力値を文字で囲むことができる．
// string str = "#"; scanner >> str; str.push_back('#');
//
// %usage
// scanner >> var
// scanner.in(IT begin, IT end)
// printer << var
// printer.join(IT begin, IT end, char separator)
// ; unsigned long long には対応していない．
// ; printer << (ll)(v.size()) << '\n' とするか，
// ; printer.output_integer<unsigned long long>(var_ull); と書く(こちらの書き方は保証外)．
//
// %verified
// http://codeforces.com/contest/71/submission/33012760
// https://csacademy.com/contest/archive/task/addition/
// https://yukicoder.me/submissions/221925
// 
// %words
// io
// %require
// %references
// cLay

#if defined(_WIN32) || defined(_WIN64)
#define getchar_unlocked _getchar_nolock
#define putchar_unlocked _putchar_nolock
#elif __GNUC__
#else
#define getchar_unlocked getchar
#define putchar_unlocked putchar
#endif
#define isvisiblechar(c) (0x21<=(c)&&(c)<=0x7E)
namespace {
    class MaiScanner {
    public:
        template<typename T> void input_integer(T& var) {
            var = 0; T sign = 1;
            int cc = getchar_unlocked();
            for (; cc<'0' || '9'<cc; cc = getchar_unlocked())
                if (cc == '-') sign = -1;
            for (; '0' <= cc&&cc <= '9'; cc = getchar_unlocked())
                var = (var << 3) + (var << 1) + cc - '0';
            var = var*sign;
        }
        inline int c() { return getchar_unlocked(); }
        inline MaiScanner& operator>>(int& var) { input_integer<int>(var); return *this; }
        inline MaiScanner& operator>>(long long& var) { input_integer<long long>(var); return *this; }
        inline MaiScanner& operator>>(string& var) {
            int cc = getchar_unlocked();
            for (; !isvisiblechar(cc); cc = getchar_unlocked());
            for (; isvisiblechar(cc); cc = getchar_unlocked())
                var.push_back(cc);
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
        template<typename IT> void join(IT begin, IT end, char sep='\n') { for (auto it = begin; it != end; ++it) *this << *it << sep; }
    };
}
MaiScanner scanner;
MaiPrinter printer;
