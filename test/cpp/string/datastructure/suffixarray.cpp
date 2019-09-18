
#include "test/common/testutil.cpp"
#include "src/cpp/string/datastructure/suffixarray.cpp"

namespace Local {

class SuffixArray {
  const char* data;
  int size;
  std::vector<const char*> sa;
  string keeper;

 public:
  SuffixArray() {}
  SuffixArray(const string& d) {
    data = d.c_str();
    size = d.size();
  }
  SuffixArray(string&& d) {
    keeper = move(d);
    data = keeper.c_str();
    size = keeper.size();
  }

  void build_1() {
    int i;
    sa.resize(size);

    for (i = 0; i < size; i++) {
      sa[i] = data + i;
    }

    sort(sa.begin(), sa.end(), [](const char* l, const char* r) { return 0 > strcmp(l, r); });  // TODO: sais
  }
  void build_2() {
    sa.resize(size);

    for (int i = 0; i < size; i++) {
      sa[i] = data + i;
    }
    sort(sa.begin(), sa.end(), [](const char* l, const char* r) { return *l < *r; });

    vector<int> rank(size * 2, -1);
    vector<int> tmp(size * 2, -1);

    rank[sa[0] - data] = 0;
    for (int i = 1; i < size; ++i)
      rank[sa[i] - data] = rank[sa[i - 1] - data] + (*sa[i - 1] < *sa[i]);
    for (int d = 1; d < size; d *= 2) {
      auto compare = [this, d, &rank](const char* l, const char* r) {
        return rank[l - data] != rank[r - data] ? rank[l - data] < rank[r - data] : rank[l + d - data] < rank[r + d - data];
      };
      sort(sa.begin(), sa.end(), compare);
      tmp[sa[0] - data] = 0;
      for (int i = 1; i < size; ++i)
        tmp[sa[i] - data] = tmp[sa[i - 1] - data] + compare(sa[i - 1], sa[i]);
      for (int i = 0; i < size; ++i)
        rank[i] = tmp[i];
    }
  }
  pair<bool, pair<int, int>> find(const string& keyword) {
    auto n = keyword.size();

    if (n == 0)
      return make_pair(false, make_pair(0, -1));

    int low = 0;
    int high = sa.size() - 1;

    for (int i = 0; i < n; ++i) {
      int ng, ok;
      ng = low - 1;
      ok = high;
      while (ok - ng > 1) {
        int m = (ok + ng) / 2;
        if (keyword[i] <= *(sa[m] + i))
          ok = m;
        else
          ng = m;
      }
      low = ok;

      ng = high + 1;
      ok = low;
      while (ng - ok > 1) {
        int m = (ok + ng) / 2;
        if (keyword[i] >= *(sa[m] + i))
          ok = m;
        else
          ng = m;
      }
      high = ok;

      if (low > high || keyword[i] != *(sa[low] + i) || keyword[i] != *(sa[high] + i))
        return make_pair(false, make_pair(0, -1));
    }
    return make_pair(true, make_pair(low, high));
  }

  inline int foundToIdx(int found) { return sa[found] - data; }
};

}  // namespace Local

using VS = vector<string>;
using VI = vector<int>;
using VII = vector<int>;

const vector<tuple<string, VS, VI>> testcases = {
    tuple<string, VS, VI>{"abcde",
                          VS{
                              "bcd",
                              "abd",
                          },
                          VI{
                              1,
                              -1,
                              -1,
                          }},
    tuple<string, VS, VI>{"aaaa",
                          VS{
                              "a",
                          },
                          VI{
                              3,
                              2,
                              1,
                              0,
                              -1,
                          }},
    tuple<string, VS, VI>{"aabbccabc",
                          VS{
                              "ab",
                              "abc",
                              "ac",
                          },
                          VI{
                              1,
                              6,
                              -1,
                              6,
                              -1,
                              -1,
                          }},
    tuple<string, VS, VI>{"abracadabra",
                          VS{
                              "ada",
                              "bra",
                              "ab",
                              "a",
                              "abracadabra",
                          },
                          VI{
                              5,
                              -1,
                              8,
                              1,
                              -1,
                              7,
                              0,
                              -1,
                              10,
                              7,
                              0,
                              3,
                              5,
                              -1,
                              0,
                              -1,
                          }},
    tuple<string, VS, VI>{"sumomomomomomomomonouti",
                          VS{
                              "momo",
                              "momomo",
                              "mo",
                          },
                          VI{
                              2, 4, 6, 8, 10, 12, 14, -1, 2, 4, 6, 8, 10, 12, -1, 2, 4, 6, 8, 10, 12, 14, 16, -1,
                          }},
};

void check_case() {
  for (auto& testcase : testcases) {
    SuffixArray suffixa(get<0>(testcase));
    suffixa.build();
    auto& ex = get<2>(testcase);
    int ptr = 0;
    for (auto& q : get<1>(testcase)) {
      auto p = suffixa.find(q);
      for (int i = p.second.first; i <= p.second.second; ++i) {
        CHKEQ(ex[ptr++], suffixa.foundToIdx(i));
      }
      CHKEQ(ex[ptr++], -1);
    }
  }
}

void check_build() {
  const int N = 100;
  repeat(counter, 10000) {
    string str;
    repeat(i, N) { str.push_back(Rand::i('a', 'c')); }
    Local::SuffixArray l1(str);
    Local::SuffixArray l2(str);
    SuffixArray t(str);

    l1.build_1();
    l2.build_2();
    t.build();

    repeat(i, N) {
      CHKEQ(l1.foundToIdx(i), l2.foundToIdx(i));
      CHKEQ(l2.foundToIdx(i), t.foundToIdx(i));
    }
  }
}

int main() {
  check_case();
  check_build();
  return 0;
}

// create testcases
// int main() {
//     int ncase;
//     cin >> ncase;
//     for (int i = 0; i < ncase; ++i) {
//         string source;
//         cin >> source;
//         cout << "tuple<string, VS, VI>{\"" << source << "\", VS{";

//         SuffixArray suffixa(source);
//         suffixa.build();

//         int q;
//         cin >> q;
//         vector<int> answers;
//         for (int j = 0; j < q; ++j) {
//             string query;
//             cin >> query;
//             cout << '"' << query << '"' << ',';
//             auto res = suffixa.find(query);
//             int low = res.second.first;
//             int high = res.second.second;

//             for(; low <= high; low++){
//                 answers.push_back(suffixa.foundToIdx(low));
//             }
//             answers.push_back(-1);
//         }
//         cout << "}, VI{";
//         for (auto e : answers) cout << e << ',';
//         cout << "}}," << endl;
//     }
//     return 0;
// }

// analysis
// int main2(){
//     int n, m;

//     int numtestcase;
//     cin >> numtestcase;

//     for (int count=1;count<=numtestcase;count++){
//         cout << "# case " << count << endl;

//         string source;
//         int numquery;

//         cin >> source;

//         SuffixArray sais(source);
//         sais.build();

//         cin >>numquery;
//         for (int i=0;i<numquery;i++){
//             string query;
//             cin >> query;

//             auto res = sais.find(query);
//             int low = res.second.first;
//             int high = res.second.second;

//             string result(source.size(),' ');
//             for(; low <= high; low++){
//                 result[sais.foundToIdx(low)] = '^';
//             }
//             cout << result << "| " << query << endl;

//         }
//     }

//     return 0;

// }

/*
------------------
STDIN
------------------
3
aabbccabc
3
ab
abc
ac

abracadabra
5
ada
bra
ab
a
abracadabra

sumomomomomomomomonouti
3
momo
momomo
mo

------------------
STDOUT
------------------
# case 1
aabbccabc
 ^    ^  | ab
      ^  | abc
         | ac
# case 2
abracadabra
     ^     | ada
 ^      ^  | bra
^      ^   | ab
^  ^ ^ ^  ^| a
^          | abracadabra
# case 3
sumomomomomomomomonouti
  ^ ^ ^ ^ ^ ^ ^        | momo
  ^ ^ ^ ^ ^ ^          | momomo
  ^ ^ ^ ^ ^ ^ ^ ^      | mo

*/