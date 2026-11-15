#include "test/common/testutil.hpp"
#include "src/cpp/container/balancing/rbtree_set.hpp"

// map 上での順位
template <class M, class K>
int naive_lower(const M& mp, const K& k) {
  return (int)distance(mp.begin(), mp.lower_bound(k));
}
template <class M, class K>
int naive_upper(const M& mp, const K& k) {
  return (int)distance(mp.begin(), mp.upper_bound(k));
}

void test_random() {
  RBTreeSet<int, int> st;
  map<int, int> mp;

  for (int lop = 0; lop < 20000; ++lop) {
    int k = Rand::i(-5, 505);
    int type = Rand::i(0, 7);
    if (type == 0) {
      bool erased = st.erase(k);
      CHKEQ((bool)mp.erase(k), erased);
    } else if (type == 1) {
      int v = Rand::i(0, 5000);
      bool inserted = st.insert(k, v);
      CHKEQ(mp.insert(make_pair(k, v)).second, inserted);
    } else if (type == 2) {
      int v = Rand::i(0, 5000);
      st[k] = v;
      mp[k] = v;
    } else if (type == 3) {
      int* p = st.find(k);
      auto it = mp.find(k);
      CHKEQ(it != mp.end(), p != nullptr);
      if (p)
        CHKEQ(it->second, *p);
    } else if (type == 4) {
      CHKEQ(naive_lower(mp, k), st.lower_bound(k));
      CHKEQ(naive_upper(mp, k), st.upper_bound(k));
    } else if (type == 5) {
      auto it = mp.find(k);
      CHKEQ(it == mp.end() ? -1 : (int)distance(mp.begin(), it), st.index_of(k));
    } else if (type == 6) {
      if (!mp.empty()) {
        int i = Rand::i(0, (int)mp.size() - 1);
        auto it = mp.begin();
        advance(it, i);
        CHKEQ(it->first, st.key_at(i));
        CHKEQ(it->second, st.value_at(i));
        st.value_at(i) += 1;
        it->second += 1;
      }
    } else {
      auto v = st.to_vector();
      using KV = vector<pair<int, int>>;
      CHK(v == KV(all(mp)));
    }
    CHKEQ((int)mp.size(), st.size());
    CHK(st.validate());
  }
}

void test_empty() {
  RBTreeSet<int, int> st;
  CHK(st.empty());
  CHKEQ(0, st.lower_bound(3));
  CHKEQ(0, st.upper_bound(3));
  CHKEQ(-1, st.index_of(3));
  CHK(!st.erase(3));
  CHK(st.find(3) == nullptr);
  CHK(st.validate());
  st[42] = 1;
  CHK(st.find(0) == nullptr);
  CHKEQ(0, st.lower_bound(42));
  CHKEQ(1, st.upper_bound(42));
  CHKEQ(0, st.index_of(42));
  CHKEQ(1, st.lower_bound(100));
  CHKEQ(0, st.upper_bound(-100));
}

void test_non_int_key() {
  RBTreeSet<string, int> st;
  st["b"] = 2;
  st["a"] = 1;
  st["c"] += 3;
  CHKEQ(3, st.size());
  CHK(st.key_at(1) == "b");
  CHKEQ(1, st.lower_bound("aa"));
  CHKEQ(2, st.upper_bound("b"));
  CHK(st.validate());
}

void test_performance() {
  const int n = 50000;
  RBTreeSet<int, int> st;
  for (int i = 0; i < n; ++i)
    st.insert(i, i);
  CHK(st.validate());
  CHKEQ(n, st.size());
  long long acc = 0;
  for (int i = 0; i < n; ++i)
    acc += st.lower_bound(i) == i;
  CHKEQ((long long)n, acc);
  for (int i = 0; i < n; ++i)
    CHK(st.erase(i));
  CHK(st.empty());
  CHK(st.validate());
}

int main() {
  test_empty();
  test_non_int_key();
  test_random();
  test_performance();
  return 0;
}
