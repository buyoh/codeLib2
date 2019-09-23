#include "test/common/testutil.cpp"
#include "src/cpp/container/array/intervalheap.cpp"

void testrand() {
  IntervalHeap<int> ih;
  multiset<int> que;

  repeat(l, 1000000) {
    int cmd = Rand::i(0, 5);
    if (cmd == 0 || cmd >= 3) {
      int v = Rand::i(1, 999999);
      ih.push(v);
      que.insert(v);
      // clog << "insert" << v << endl;
    } else if (cmd == 1) {
      if (que.size() > 0) {
        ih.popmin();
        que.erase(que.begin());
      }
      // clog << "popmin" << endl;
    } else if (cmd == 2) {
      if (que.size() > 0) {
        ih.popmax();
        que.erase(--que.end());
      }
      // clog << "popmax" << endl;
    }
    if (que.size() > 0) {
      if (ih.min() != *que.begin())
        abort();
      if (ih.max() != *que.rbegin())
        abort();
    }
  }
}

int main(){
    testrand();
    return 0;
}