#ifndef SRC_CPP_MATH_METHOD_DIGITDP_HPP__
#define SRC_CPP_MATH_METHOD_DIGITDP_HPP__
// %=BEGIN DOC
// %title
// 桁 dp
//
// %overview
// 0 以上 h未満のうち 1 を含む値の数を求める。
// 桁 dpのたたき台として使う。
// %usage
//
// %require
// ```
#include <string>
using namespace std;
// ```
// %verified
//
// %words
// keta, ketadp
// %references
// %=END DOC
// %=BEGIN CODE

class DigitDPState {
 public:
  int total_[2];
  DigitDPState() { fill(total_, total_ + 2, 0); }
  static DigitDPState initialValue() {
    DigitDPState s;
    s.total_[0] = 1;
    return s;
  }
  void apply(const DigitDPState& src, int digit_idx, int digit_num) {
    // digit_idx: '上'からi番目
    bool one = digit_num == 1;
    total_[one] += src.total_[0];
    total_[1] += src.total_[1];
  }

  int answer() const { return total_[1]; }
};

DigitDPState digit2dp(const long long h) {
  const int m = 62;
  // [上からi桁目][h未満かどうか]
  static DigitDPState dp[m + 1][2];
  fill(dp[0], dp[m + 1], DigitDPState());
  dp[0][0] = DigitDPState::initialValue();
  for (int i = 0; i < m; ++i) {
    long long c = (h >> (m - i - 1)) & 1ll;
    for (int lower_than = 0; lower_than < 2; ++lower_than) {
      for (int v = 0; v < 2; ++v) {
        if (lower_than || v <= c) {
          dp[i + 1][lower_than || v < c].apply(dp[i][lower_than], i, v);
        }
      }
    }
  }
  return dp[m][1];
}

DigitDPState digit10strdp(const string& h) {
  const int km = 99;
  // [上からi桁目][h未満かどうか]
  static DigitDPState dp[km + 2][10];
  const int m = int(h.size());
  fill(dp[0], dp[km + 2], DigitDPState());
  dp[0][0] = DigitDPState::initialValue();
  for (int i = 0; i < m; ++i) {
    long long c = h[i] - '0';
    for (int lower_than = 0; lower_than < 2; ++lower_than) {
      for (int v = 0; v < 10; ++v) {
        if (lower_than || v <= c) {
          dp[i + 1][lower_than || v < c].apply(dp[i][lower_than], i, v);
        }
      }
    }
  }
  return dp[m][1];
}

// %=END CODE
#endif  // SRC_CPP_MATH_METHOD_DIGITDP_HPP__
