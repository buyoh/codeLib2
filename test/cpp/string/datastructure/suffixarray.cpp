#include "src/cpp/string/datastructure/suffixarray.cpp"
#include "test/common/testutil.cpp"

int main() {
  int numtestcase;
  cin >> numtestcase;

  for (int count = 1; count <= numtestcase; count++) {
    cout << "# case " << count << endl;

    string source;
    int numquery;

    cin >> source;
    cout << source << endl;

    SuffixArray sais(source);
    sais.generate();

    cin >> numquery;
    for (int i = 0; i < numquery; i++) {
      string query;
      cin >> query;

      auto res = sais.find(query);
      int low = res.second.first;
      int high = res.second.second;

      string result(source.size(), ' ');
      for (; low <= high; low++) {
        result[sais.foundToIdx(low)] = '^';
      }
      cout << result << "| " << query << endl;
    }
  }

  return 0;
}