#include "test/common/testutil.hpp"
#include "src/cpp/container/balancing/rbtree_multiset.hpp"

// 素朴な multimap: キーの昇順，同じキーは挿入順
using Naive = vector<pair<int, int>>;

int naive_lower(const Naive& v, int k) {
  return (int)(std::lower_bound(all(v), make_pair(k, INT_MIN)) - v.begin());
}
int naive_upper(const Naive& v, int k) {
  return (int)(std::lower_bound(all(v), make_pair(k + 1, INT_MIN)) - v.begin());
}

void test_random(int n_ops, int key_range) {
  RBTreeMultiSet<int, int> s;
  Naive v;
  int serial = 0;
  for (int lop = 0; lop < n_ops; ++lop) {
    int n = (int)v.size();
    int type = Rand::i(0, 9);
    int k = Rand::i(-1, key_range + 1);
    if (type <= 3) {
      int x = serial++;
      int i = s.insert(k, x);
      int ni = naive_upper(v, k);
      CHKEQ(ni, i);
      v.insert(v.begin() + ni, make_pair(k, x));
    } else if (type == 4) {
      bool ok = s.erase_one(k);
      int l = naive_lower(v, k), r = naive_upper(v, k);
      CHKEQ(l < r, ok);
      if (l < r)
        v.erase(v.begin() + l);
    } else if (type == 5) {
      int c = s.erase_all(k);
      int l = naive_lower(v, k), r = naive_upper(v, k);
      CHKEQ(r - l, c);
      v.erase(v.begin() + l, v.begin() + r);
    } else if (type == 6) {
      if (n > 0) {
        int i = Rand::i(0, n - 1);
        s.erase_at(i);
        v.erase(v.begin() + i);
      }
    } else if (type == 7) {
      int l = naive_lower(v, k), r = naive_upper(v, k);
      CHKEQ(l, s.lower_bound(k));
      CHKEQ(r, s.upper_bound(k));
      CHKEQ(r - l, s.count(k));
      CHKEQ(l < r ? l : -1, s.index_of(k));
      const int* p = s.find(k);
      CHK((p != nullptr) == (l < r));
      if (p)
        CHKEQ(v[l].second, *p);
    } else if (type == 8) {
      if (n > 0) {
        int i = Rand::i(0, n - 1);
        CHKEQ(v[i].first, s.key_at(i));
        CHKEQ(v[i].second, s.value_at(i));
        // 参照を通した書き換え
        s.value_at(i) += 1000000;
        v[i].second += 1000000;
      }
    } else {
      CHK(s.to_vector() == v);
    }
    CHKEQ((int)v.size(), s.size());
    CHK(s.empty() == v.empty());
    CHK(s.validate());
  }
  CHK(s.to_vector() == v);
}

void test_basic() {
  RBTreeMultiSet<int, string> s;
  CHK(s.empty());
  CHKEQ(0, s.insert(5, "a"));
  CHKEQ(1, s.insert(5, "b"));
  CHKEQ(0, s.insert(3, "c"));
  CHKEQ(3, s.insert(5, "d"));
  // {3: c, 5: a, 5: b, 5: d}
  CHKEQ(3, s.count(5));
  CHKEQ(1, s.index_of(5));
  CHKEQ(-1, s.index_of(4));
  CHK(*s.find(5) == "a");
  CHK(s.find(4) == nullptr);
  // %usage の例: キーが k の要素をすべて走査する
  string joined;
  for (int i = s.lower_bound(5), r = s.upper_bound(5); i < r; ++i)
    joined += s.value_at(i);
  CHK(joined == "abd");
  // 返したポインタは，その要素が削除されるまで有効
  string* pb = &s.value_at(2);
  CHK(s.erase_one(5));  // "a" を削除
  CHK(*pb == "b");
  s.insert(4, "e");
  CHK(*pb == "b");
  // 別のキーの erase_all / erase_at の後も有効
  string* pc = s.find(3);
  string* pe = s.find(4);
  CHKEQ(2, s.erase_all(5));
  CHK(*pc == "c");
  CHK(*pe == "e");
  s.insert(6, "f");
  s.erase_at(s.index_of(6));
  CHK(*pc == "c");
  CHK(*pe == "e");
  CHKEQ(0, s.erase_all(5));
  CHK(!s.erase_one(5));
  CHKEQ(2, s.size());
  CHK(s.validate());
  const RBTreeMultiSet<int, string>& cs = s;
  CHK(*cs.find(4) == "e");
  CHK(cs.value_at(0) == "c");
}

void test_performance() {
  const int n = 100000;
  RBTreeMultiSet<int, int> s;
  for (int i = 0; i < n; ++i)
    s.insert(i % 100, i);
  CHK(s.validate());
  CHKEQ(n / 100, s.count(42));
  long long acc = 0;
  for (int k = 0; k < 100; ++k)
    acc += s.erase_all(k);
  CHKEQ((long long)n, acc);
  CHK(s.empty());
}

int main() {
  test_basic();
  repeat(_, 3) {
    test_random(5000, 10);
    test_random(5000, 1000);
  }
  test_performance();
  return 0;
}
