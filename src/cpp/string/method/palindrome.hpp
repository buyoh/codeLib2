#ifndef SRC_CPP_STRING_METHOD_PALINDROME_HPP__
#define SRC_CPP_STRING_METHOD_PALINDROME_HPP__
// %=BEGIN DOC
// %title
// 回文判定
//
// %overview
// コピペと手打ちどっちが速いかな
//
// %usage
//
// %require
// ```
#include <string>
using namespace std;
// ```
// %verified
//
// %references
//
// %=END DOC
// %=BEGIN CODE

bool iskaibun(const string& str) {
  for (int left = 0, right = (int)str.size() - 1; left < right; ++left, --right)
    if (str[left] != str[right])
      return false;
  return true;
}
// %=END CODE
#endif  // SRC_CPP_STRING_METHOD_PALINDROME_HPP__
