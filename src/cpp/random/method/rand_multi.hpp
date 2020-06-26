// %=BEGIN
// %title
// 1からnまでの乱数をm個重複せずランダムに選ぶ
//
// %overview
// 1からnまでの乱数をm個重複せずランダムに選ぶ．
// このアルゴリズムは空間計算量O(m)，時間計算量O(m)で済む（ハッシュ操作をO(1)とする）．
// 簡単な実装方法として
// - 重複しないm個を取り出すまで乱数を引く (n>>m なら衝突しにくいので高速)
// - iotaしてshuffleしてeraseする（空間O(n)，時間O(n)）
// があるが，計算量的には両者のどちらよりも優れていることが分かる．
// m/nの比でアルゴリズムを選択するハイブリッド実装でも良さそうである．
//
// [TODO] rand_int の除去
//
// %usage
// void pick_multirand(int n, int m, vector<int>& out)
// ; 1からnまでの乱数をm個重複せずランダムに選びoutにpush_backする
// inline int RandChooser::operator()(RANDOM &rd)
// rd : 乱数生成器．
// ; 乱数生成する．
// inline int RandChooser::disable(int idx)
// ; 整数idxを次回以降の乱数生成で選ばれないように設定する．
// ; 既にdisableした値をもう一度disableする操作はinvalid．
//
// %require
// ```
#include <vector>
#include <unordered_map>
using namespace std;
// rand_int の実装
int rand_int(int, int);
// ```
// %verified
//
// %references
//
// %=END

//
void pick_multirand(int n, int m, vector<int>& out) {
  out.clear();
  unordered_map<int, int> memo;
  if (n < m)
    m = n;
  for (int low = 1; low <= m; ++low) {
    int r = rand_int(low, n);
    if (!memo[r]) {
      out.emplace_back(r);
    } else {
      out.emplace_back(memo[r]);
    }
    int e = memo[low];
    memo[r] = e ? e : low;
  }
}
