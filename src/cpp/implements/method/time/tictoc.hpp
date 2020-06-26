#ifndef SRC_CPP_IMPLEMENTS_METHOD_TIME_TICTOC_HPP__
#define SRC_CPP_IMPLEMENTS_METHOD_TIME_TICTOC_HPP__
// %=BEGIN
// %title
// 時間計測用マクロ．
//
// %overview
//
//
// %usage
// void tic()
// ;計測開始
// void toc()
// ;ticからtocまでに掛かった時間を表示する
// TIME
// ;chrono::system_clock::now()
// MILLISEC
// ;TIMEの差分をミリ秒に変換
//
// %require
// ```
#include <chrono>
#include <iostream>
// ```
// %words
// time,tic,toc
// %verified
//
// %references
// %=END

#define TIME std::chrono::system_clock::now()
#define MILLISEC(t) (std::chrono::duration_cast<std::chrono::milliseconds>(t).count())
namespace {
std::chrono::system_clock::time_point ttt;
inline void tic() {
  ttt = TIME;
}
inline void toc() {
  std::clog << "TIME : " << MILLISEC(TIME - ttt) << '\n';
}
}  // namespace
#endif  // SRC_CPP_IMPLEMENTS_METHOD_TIME_TICTOC_HPP__
