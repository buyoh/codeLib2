
#include <initializer_list>

class CharConverter {
  int IdxAlphaL;
  int IdxAlphaS;
  int IdxNumber;
  int IdxOther;
  std::initializer_list<char> Li;

  inline char otherwise(char c) {
    char i = 0;
    for (auto o : Li) {
      if (c == o)
        return i;
      ++i;
    }
    return i;
  }

 public:
  inline char operator[](char c) {
    return 'A' <= c && c <= 'Z'
               ? IdxAlphaL - 'A' + c
               : 'a' <= c && c <= 'z' ? IdxAlphaS - 'a' + c
                                      : '0' <= c && c <= '9' ? IdxNumber - '0' : otherwise(c);
    ;
  }
};
