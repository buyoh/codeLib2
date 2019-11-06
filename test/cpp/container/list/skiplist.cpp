#include "test/common/testutil.cpp"
#include "src/cpp/container/list/skiplist.cpp"
#include "src/cpp/container/array/standard.cpp"

int main() {
  Container<int> container;
  SkipList<int> slist1, slist2;

  repeat(cnt, 1000) {
    int type = Rand::i(0, 1);
    if (type == 0) {
      auto i = Rand::i<int>(0, container.data_.size());
      container.insert(i, cnt);
      slist1.insert(i, cnt);
      slist2.insert(slist2.get(i), cnt);
    }
  }

  return 0;
}