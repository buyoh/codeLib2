#include "test/common/testutil.hpp"
#include "src/cpp/container/balancing/treap_set.hpp"

void test_random() {
  TreapSet<int, int> tp;
  map<int, int> mp;

  for (int lop = 0; lop < 20000; ++lop) {
    int k = Rand::i(1, 500);
    int type = Rand::i(0, 3);
    if (type == 0) {
      bool erased = tp.erase(k);
      CHKEQ((bool)mp.erase(k), erased);
    } else if (type == 1) {
      int* p = tp.find(k);
      auto it = mp.find(k);
      CHKEQ(it != mp.end(), p != nullptr);
      if (p)
        CHKEQ(it->second, *p);
    } else {
      int v = Rand::i(0, 5000);
      tp[k] = v;
      mp[k] = v;
    }
    CHKEQ((int)mp.size(), tp.size());
  }
  const TreapSet<int, int>& ctp = tp;
  for (int k = 0; k <= 501; ++k) {
    const int* p = ctp.find(k);
    auto it = mp.find(k);
    CHKEQ(it != mp.end(), p != nullptr);
    if (p)
      CHKEQ(it->second, *p);
  }
}

// 空の木に最初に入れたキーが正しいこと
void test_first_key() {
  TreapSet<int, long long> tp;
  CHK(tp.empty());
  tp[42] = 7;
  CHK(tp.find(0) == nullptr);
  CHK(tp.find(42) != nullptr);
  CHKEQ(7LL, *tp.find(42));
  CHKEQ(1, tp.size());
  CHK(!tp.erase(0));
  CHK(tp.erase(42));
  CHK(tp.empty());
  CHK(!tp.erase(42));
}

void test_non_int_key() {
  TreapSet<string, int> tp;
  tp["b"] = 2;
  tp["a"] = 1;
  tp["c"] += 3;
  CHKEQ(3, tp.size());
  CHKEQ(1, *tp.find("a"));
  CHKEQ(3, *tp.find("c"));
  CHK(tp.find("d") == nullptr);
}

void test_move() {
  TreapSet<int, int> s1;
  for (int i = 0; i < 10; ++i)
    s1[i] = i;
  TreapSet<int, int> s2(move(s1));
  CHKEQ(0, s1.size());
  CHKEQ(10, s2.size());
  TreapSet<int, int> s3;
  s3 = move(s2);
  CHKEQ(0, s2.size());
  CHKEQ(10, s3.size());
  CHKEQ(9, *s3.find(9));
}

void test_performance() {
  const int n = 50000;
  TreapSet<int, int> tp;
  for (int i = 0; i < n; ++i)
    tp[i] = i;
  CHKEQ(n, tp.size());
  for (int i = 0; i < n; ++i)
    CHK(tp.erase(i));
  CHK(tp.empty());
}

int main() {
  test_random();
  test_first_key();
  test_non_int_key();
  test_move();
  test_performance();
  return 0;
}
