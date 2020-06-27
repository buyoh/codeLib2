#include "test/common/testutil.hpp"
#include "test/common/graph_generator.hpp"
#include "src/cpp/graph/method/flow/dinic.hpp"

using G = DGraphF;

function<pair<DGraphF, int>()> testcases[] = {[]() {
                                                G g(2);
                                                g.connect(0, 1, 10);
                                                return make_pair(g, 10);
                                              },
                                              []() {
                                                G g(6);
                                                g.connect(4, 0, 2);
                                                g.connect(0, 1, 1);
                                                g.connect(1, 5, 2);
                                                g.connect(0, 2, 1);
                                                g.connect(2, 5, 1);
                                                g.connect(4, 3, 1);
                                                g.connect(4, 1, 1);
                                                return make_pair(g, 3);
                                              }};

int main() {
  for (auto t : testcases) {
    auto p = t();
    auto graph = move(p.first);
    auto expec = move(p.second);
    int n = graph.size();
    vector<int> res;
    dinic(graph, res, n - 2, n - 1);
    auto flow = res[n - 1];
    CHKEQ(expec, flow);
  }
  return 0;
}