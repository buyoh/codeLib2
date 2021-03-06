#ifndef SRC_CPP_STRING_DATASTRUCTURE_ROLLINGHASH_HPP__
#define SRC_CPP_STRING_DATASTRUCTURE_ROLLINGHASH_HPP__
// %=BEGIN DOC
// %title
// ローリングハッシュ
//
// %overview
// 連続した部分文字列のハッシュをO(N)で求める．
// 2つのハッシュで撃墜をなるべく避ける．
//
// %usage
// pair<rh_t, rh_t> rollinghash::operator()(int begin, int end)
// 連続した部分文字列str[begin,end)のハッシュの組を取得する
//
// %require
// ```
#include <vector>
#include <string>
using namespace std;
// ```
// %verified
// csaで使ったはず
// %references
// プログラミングチャレンジブック
// %=END DOC
// %=BEGIN CODE

class RollingHash {
 public:
  using rh_t = unsigned long long;

 private:
  static const rh_t MD_1 = (rh_t)(1e9 + 21);
  static const rh_t PW = (rh_t)(1e9 + 7);
  static vector<rh_t> pow_table__[2];

 public:
  vector<rh_t> hashes_[2];

  RollingHash() {}
  RollingHash(const string& s) { build(s); }

  inline pair<rh_t, rh_t> operator()(int begin, int end) {
    return pair<rh_t, rh_t>(
        (hashes_[0][end] - hashes_[0][begin] * pow_table__[0][end - begin]),
        (hashes_[1][end] - (hashes_[1][begin] * pow_table__[1][end - begin]) % MD_1 + MD_1) % MD_1);
  }

  void build_pow(size_t size) {
    ++size;
    if (pow_table__[0].size() < size) {
      pow_table__[0].reserve(size);
      pow_table__[1].reserve(size);
      if (pow_table__[0].empty())
        pow_table__[0].push_back(1), pow_table__[1].push_back(1);
      for (size_t i = pow_table__[0].size(); i < size; ++i) {
        pow_table__[0].push_back(pow_table__[0].back() * PW);
        pow_table__[1].push_back(pow_table__[1].back() * PW % MD_1);
      }
    }
  }
  void build(const string& src) {
    size_t m = src.size() + 1;
    build_pow(m);
    hashes_[0].resize(m);
    hashes_[1].resize(m);
    hashes_[0][0] = hashes_[1][0] = 0;
    for (size_t i = 1; i < m; ++i) {
      rh_t c = src[i - 1];
      hashes_[0][i] = hashes_[0][i - 1] * PW + c;
      hashes_[1][i] = (hashes_[1][i - 1] * PW + c) % MD_1;
    }
  }
};
vector<RollingHash::rh_t> RollingHash::pow_table__[2];
// %=END CODE
#endif  // SRC_CPP_STRING_DATASTRUCTURE_ROLLINGHASH_HPP__
