#include "test/common/testutil.hpp"
#include "src/cpp/math/method/diffangle.hpp"

int main() {
  assert(diffangle(0.0, 0.0, 10.0) == 0.0);
  assert(diffangle(0.0, 1.0, 10.0) == 1.0);
  assert(diffangle(1.0, 0.0, 10.0) == -1.0);
  assert(diffangle(0.0, 9.0, 10.0) == -1.0);
  assert(diffangle(9.0, 0.0, 10.0) == 1.0);
  assert(diffangle(1.0, 9.0, 10.0) == -2.0);
  assert(diffangle(9.0, 1.0, 10.0) == 2.0);
  assert(diffangle(0.0, 5.0, 10.0) == 5.0);
  assert(diffangle(0.0, 4.0, 10.0) == 4.0);
  assert(diffangle(0.0, 6.0, 10.0) == -4.0);
  return 0;
}
