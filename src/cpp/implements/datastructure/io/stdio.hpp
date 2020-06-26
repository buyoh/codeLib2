#ifndef SRC_CPP_IMPLEMENTS_DATASTRUCTURE_IO_STDIO_HPP__
#define SRC_CPP_IMPLEMENTS_DATASTRUCTURE_IO_STDIO_HPP__
// %=BEGIN
// %title
// getc,putcによる入出力実装．
//
// %overview
// _WIN64 OR _WIN32 環境なら _getc_nolock を，
// __GNUC__         環境なら getc_unlocked を，
// それ以外の環境なら getc を使う．(codeforcesでも使える)
// 実数には対応しない．対応したとしてもscanf,printfでごまかします．
//
// stringを読み込む際，変数をクリアしない．
// これを使って，入力値を文字で囲むことができる．
// string str = "#"; scanner >> str; str.push_back('#');
//
// FILE* 型の外部ファイル読み込みにも対応した．
// MaiScanner sc(fp); と書く．
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
// %require
// ```
#include <cstdio>
#include <string>
using namespace std;
// ```
// %verified
// http://codeforces.com/contest/71/submission/33012760
// https://csacademy.com/contest/archive/task/addition/
// https://yukicoder.me/submissions/221925
//
// %words
// io
// %references
// cLay
// %=END

#if defined(_WIN32) || defined(_WIN64)
#define getc_x _getc_nolock
#define putc_x _putc_nolock
#elif defined(__GNUC__)
#define getc_x getc_unlocked
#define putc_x putc_unlocked
#else
#define getc_x getc
#define putc_x putc
#endif
#define isvisiblechar(c) (0x21 <= (c) && (c) <= 0x7E)
class MaiScanner {
  FILE* fp_;

 public:
  inline MaiScanner(FILE* fp) : fp_(fp) {}
  template <typename T>
  void input_integer(T& var) noexcept {
    var = 0;
    T sign = 1;
    int cc = getc_x(fp_);
    for (; cc < '0' || '9' < cc; cc = getc_x(fp_))
      if (cc == '-')
        sign = -1;
    for (; '0' <= cc && cc <= '9'; cc = getc_x(fp_))
      var = (var << 3) + (var << 1) + cc - '0';
    var = var * sign;
  }
  inline int c() noexcept { return getc_x(fp_); }
  inline MaiScanner& operator>>(int& var) noexcept {
    input_integer<int>(var);
    return *this;
  }
  inline MaiScanner& operator>>(long long& var) noexcept {
    input_integer<long long>(var);
    return *this;
  }
  inline MaiScanner& operator>>(string& var) {
    int cc = getc_x(fp_);
    for (; !isvisiblechar(cc); cc = getc_x(fp_))
      ;
    for (; isvisiblechar(cc); cc = getc_x(fp_))
      var.push_back(cc);
    return *this;
  }
  template <typename IT>
  inline void in(IT begin, IT end) {
    for (auto it = begin; it != end; ++it)
      *this >> *it;
  }
};
class MaiPrinter {
  FILE* fp_;

 public:
  inline MaiPrinter(FILE* fp) : fp_(fp) {}
  template <typename T>
  void output_integer(T var) noexcept {
    if (var == 0) {
      putc_x('0', fp_);
      return;
    }
    if (var < 0)
      putc_x('-', fp_), var = -var;
    char stack[32];
    int stack_p = 0;
    while (var)
      stack[stack_p++] = '0' + (var % 10), var /= 10;
    while (stack_p)
      putc_x(stack[--stack_p], fp_);
  }
  inline MaiPrinter& operator<<(char c) noexcept {
    putc_x(c, fp_);
    return *this;
  }
  inline MaiPrinter& operator<<(int var) noexcept {
    output_integer<int>(var);
    return *this;
  }
  inline MaiPrinter& operator<<(long long var) noexcept {
    output_integer<long long>(var);
    return *this;
  }
  inline MaiPrinter& operator<<(char* str_p) noexcept {
    while (*str_p)
      putc_x(*(str_p++), fp_);
    return *this;
  }
  inline MaiPrinter& operator<<(const string& str) {
    const char* p = str.c_str();
    const char* l = p + str.size();
    while (p < l)
      putc_x(*p++, fp_);
    return *this;
  }
  template <typename IT>
  void join(IT begin, IT end, char sep = ' ') {
    for (bool b = 0; begin != end; ++begin, b = 1)
      b ? *this << sep << *begin : *this << *begin;
  }
};
MaiScanner scanner(stdin);
MaiPrinter printer(stdout);
#endif  // SRC_CPP_IMPLEMENTS_DATASTRUCTURE_IO_STDIO_HPP__
