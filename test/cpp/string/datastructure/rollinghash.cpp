#include "test/common/testutil.hpp"
#include "src/cpp/string/datastructure/rollinghash.hpp"

int main() {
  string str =
      "zuorhi viyantas was festsu ruor proi "
      "yuk dalfe suoivo swenne yat vu henvi nes "
      "sho fu briyu praffi stassui tsenva chies "
      "ien ryus sois nyat pyaro shennie fru ";
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
