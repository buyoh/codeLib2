#include "test/common/testutil.hpp"
#include "test/common/graph_generator.hpp"
#include "src/cpp/graph/datastructure/wrapper/2sat.hpp"

class Simple2satSolver {
  int n_;
  vector<array<int, 2>> clauses_;

 public:
  Simple2satSolver(int _n) : n_(_n) {}
  void push(int a, int b) { clauses_.push_back({a, b}); }
  template <typename VEC>
  bool correct(const VEC& b) {
    bool perfect = true;
    for (const auto& c : clauses_) {
      bool ok = false;
      for (auto e : c) {
        ok |= (e > 0 && b[e - 1]) || (e < 0 && !b[-e - 1]);
      }
      if (!ok) {
        perfect = false;
        break;
      }
    }
    return perfect;
  }
  vector<int> choice() {
    repeat(bit, 1 << n_) {
      bitset<300> b = bit;
      if (correct(b)) {
        vector<int> res(n_);
        repeat(i, n_) res[i] = b[i];
        return res;
      }
    }
    return vector<int>();
  }
  const decltype(clauses_)& clauses() const { return clauses_; }
};

int main() {
  // constexpr int n = 6;
  // constexpr int m = 20;
  constexpr int n = 3;
  constexpr int m = 6;
  constexpr int nq = 100;
  repeat(_, nq) {
    Simple2satSolver sat1(n);
    Sat2 sat2(n);
    repeat(i, m) {
      int a = Rand::i(-n, n - 1);
      int b = Rand::i(-n, n - 1);
      if (a >= 0)
        ++a;
      if (b >= 0)
        ++b;
      sat1.push(a, b);
      sat2.push(a, b);
    }
    auto ans1 = sat1.choice();
    auto ans2 = sat2.solve();
    CHKEQ(ans1.empty(), ans2.empty());
    if (!ans2.empty()) {
      CHK(sat1.correct(ans2));
    }
  }
  return 0;
}
