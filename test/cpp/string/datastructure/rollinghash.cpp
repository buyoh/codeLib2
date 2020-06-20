#include "test/common/testutil.hpp"
#include "src/cpp/string/datastructure/rollinghash.cpp"

int main() {
  string str = "abracadabra";
  int n = str.size();

  RollingHash rh(str);

  repeat(i, n) {
    iterate(j, i, n) {
      iterate(k, j, n) {
        int m = k - j + 1;
        // cout << str.substr(i, m) << " <=> " << str.substr(j, m) << '\n';
        CHKEQ((str.substr(i, m) == str.substr(j, m)), (rh(i, i + m) == rh(j, j + m)));
      }
    }
  }

  return 0;
}
