#include "test/common/testutil.hpp"
#include "src/cpp/container/likermq/mosalgo.hpp"

constexpr int kN = 1000;
constexpr int kQ = 1000;
constexpr int kColor = 50;
vector<int> dat;

void check(int b, int e, int actual) {
  vector<int> bucket(kColor);
  iterate(i, b, e) { bucket[dat[i]] = 1; }
  int expected = 0;
  for (auto e : bucket)
    expected += e;
  CHKEQ(expected, actual);
}

class Mofu : public MoAlgo {
  vector<int> bucket_;
  int cnt_ = 0;

 protected:
  void init() override {
    bucket_.resize(kColor);
    fill(all(bucket_), 0);
  }
  void add(int i) override {
    if (!bucket_[dat[i]]++)
      ++cnt_;
  }
  void del(int i) override {
    if (!--bucket_[dat[i]])
      --cnt_;
  }
  void query(int i) override { check(ranges_[i].first, ranges_[i].second, cnt_); }

 public:
  Mofu(int n) : MoAlgo(n) {}
};

int main() {
  Mofu mof(kN);

  dat.resize(kN);
  repeat(i, kN) dat[i] = Rand::i(0, kColor - 1);

  mof.rangeReserve(kQ);
  repeat(i, kQ) {
    int b = Rand::i<int>(0, kN - 1);
    int e = Rand::i<int>(1, kN);
    if (b > e)
      swap(b, e);
    if (b == e)
      ++e;
    mof.rangeEmplace(b, e);
  }

  mof.solve();
  return 0;
}