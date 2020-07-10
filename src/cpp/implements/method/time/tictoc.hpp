#ifndef SRC_CPP_IMPLEMENTS_METHOD_TIME_TICTOC_HPP__
#define SRC_CPP_IMPLEMENTS_METHOD_TIME_TICTOC_HPP__
// %=BEGIN DOC
// %title
// 時間計測用マクロ
//
// %overview
// ticで開始、tocで経過時間を返す。
// tic/tocという名称はmatlabで使われている。
//
// %usage
//
// %require
// ```
#include <chrono>
#include <ostream>
// ```
// %words
// time,tic,toc
// %verified
//
// %references
// %=END DOC
// %=BEGIN CODE
template <typename C = std::chrono::milliseconds>
class Timer {
  std::chrono::system_clock::time_point tp_;

 public:
  static inline auto now() { return std::chrono::system_clock::now(); }
  inline void tic() { tp_ = now(); }
  inline auto toc() const { return std::chrono::duration_cast<C>(now() - tp_).count(); }
  inline Timer() : tp_(now()) {}
};
inline std::ostream& operator<<(std::ostream& o, const Timer<>& t) {
  return o << (long long)t.toc();
}
// %=END CODE
#endif  // SRC_CPP_IMPLEMENTS_METHOD_TIME_TICTOC_HPP__
