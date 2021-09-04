#include "test/common/testutil.hpp"

#include "src/cpp/math/method/prime/util_prime_sieve.hpp"
#include "src/cpp/math/method/prime/primesieve.hpp"
#include "src/cpp/math/method/prime/millerrabin.hpp"

IsPrime<200017> is_prime;
PrimeList<17994> prime_list(is_prime);

Prime<200017> prime;

void test_is_prime() {
  for (int x = 1; x <= 200000; ++x) {
    const bool r_miller = isprime_MillerRabin(x);
    const bool r_is_prime = is_prime[x];
    const bool r_prime = prime.isPrime(x);
    CHKEQ(r_miller, r_is_prime);
    CHKEQ(r_miller, r_prime);
    CHKEQ(r_is_prime, r_prime);
  }
}

void test_prime_list() {
  auto it_is_prime = prime_list.begin();
  auto it_prime = prime.begin();
 for (int last = -1; last < 200000;) {
    const int r_is_prime = *it_is_prime;
    const int r_prime = *it_prime;
    CHKEQ(r_is_prime, r_prime);
    last = r_prime;
    ++it_is_prime;
    ++it_prime;
 }
}

void test_prime_division() {
  for (int x = 100000-2500; x <= 100000+2500; ++x) {
    auto r_is_prime = prime_list.division(x);
    auto r_prime = prime.division(x);
    CHKEQ(r_is_prime.size(), r_prime.size());
    for (auto p : r_is_prime) {
      auto it = r_prime.find(p.first);
      CHK(it != r_prime.end());
      CHKEQ(p.second, it->second);
    }
  }
}

int main() {
  test_is_prime();
  test_prime_list();
  test_prime_division();
  return 0;
}
