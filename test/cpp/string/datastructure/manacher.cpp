#include "test/common/testutil.hpp"
#include "src/cpp/string/datastructure/manacher.hpp"

template <typename ITR>
bool isKaibun(ITR begin, ITR back) {
  while (begin < back) {
    if (*begin != *back)
      return false;
    ++begin;
    --back;
  }
  return true;
}

int main() {
  string str =
      "O-oooooooooo AAAAE-A-A-I-A-U- JO-oooooooooooo AAE-O-A-A-U-U-A- E-eee-ee-eee "
      "AAAAE-A-E-I-E-A- JO-ooo-oo-oo-oo EEEEO-A-AAA-AAAA";
  int n = str.size();
  Manacher manatee(str);
  iterate(i, 0, n) {
    iterate(j, i, n) {
      auto it = str.begin();
      CHKEQ(isKaibun(it + i, it + j), manatee(i, j));
    }
  }
  return 0;
}
