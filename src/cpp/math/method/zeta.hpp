// %=BEGIN
// %title
// 高速ゼータ変換（メビウス逆変換）
//
// %overview
// ゼータ変換は，集合関数f(2^S)=Nを
// g(X) = sum{X⊆Y}(f(Y))
// に変換する．
// プログラミング上では，f,gは多くの場合，
// 関数ではなく配列として表現される．
// 高速ゼータ変換は，愚直に計算すると4^Nになるところ，
// N2^Nで計算するシンプルなDPである．
//
//
// %usage
// vector<value_t> zeta_transform(int n, vector<value_t> func);
// vector<value_t> mebius_transform(int n, vector<value_t> func)
// ; 集合はビットで表現．
// ; func.size() >= (1 << n) を満たす．
// %require
// ```
#include <vector>
using namespace std;
// ```
//
// %verified
// unverified!!!
//
// %references
// https://topcoder.g.hatena.ne.jp/iwiwi/20120422/1335065228
// https://naoyat.hatenablog.jp/entry/zeta-moebius
// 講義資料
// %words
// zeta, mebius
//
// %=END

using value_t = int;
vector<value_t> zeta_transform(int n, vector<value_t> func) {
  for (int i = 0; i < n; ++i)
    for (int b = 0; b < (1 << n); ++b)
      if (~b & (1 << i))
        func[b] += func[b | (1 << i)];
  return func;
}

using value_t = int;
vector<value_t> mebius_transform(int n, vector<value_t> func) {
  for (int i = 0; i < n; ++i)
    for (int b = 0; b < (1 << n); ++b)
      if (~b & (1 << i))
        func[b] -= func[b | (1 << i)];
  return func;
}
