// %title
// Treap(segment)(many)
// 
// %overview
// Treapとは，次を満たす確率的平衝二分探索木である．
// - keyについてみると，木は二分探索木になっている．
// - priority(randomized)についてみると，木はHeapになっている．
// 
// 実装したTreapは，配列っぽいインターフェースを持つ．split/mergeベースで記述．
// 
// 
// 区間クエリは書けそう．
// 例えば『キーがx以下のNodeにyを加算する』は，treap[x]とtreap[x].childlen[0]に対してLazy実装で実現できる．
//
// %usage
// void Treap::concat(Treap& another)
// ; thisの末尾にanotherを結合して，anotherを空にする．
// Treap Treap::split(int size)
// ; thisの前からsize個を切り出す．
// void Treap::insert(int index, value_t val)
// ; index番目の要素の手前にvalを挿入する．
// void Treap::erase(int index)
// ; index番目の要素を削除する．
// unique_ptr<TreapNode>& Treap::find(int index)
// ; index番目の要素のスマートポインタの参照を得る．
// ; index番目の要素が存在しないなら，空のスマートポインタの参照を得る．
// value_t& Treap::operator[](int index)
// ; index番目の要素に対応するvalの参照を得る．無いなら爆発する．
//
// %verified
// 
// %references
// https://www.slideshare.net/iwiwi/2-12188757
// http://www.prefield.com/algorithm/container/treap.html

