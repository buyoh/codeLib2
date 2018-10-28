// %title
// Binary Indexed Tree(単独加算，区間総和)
// 
// %overview
// 次のクエリを処理できる．
// - １つの要素を加算・減算する．
// - 区間の要素の総和を計算する．
// 1-indexedの実装に注意．
// 何度も加減算を繰り返すと内部でオーバーフローを起こす可能性．
//
// %usage
// Bitree<typename T>(int n)
// ; [1..n] の配列を確保する．
// T Bitree::sum(int r)
// ; [1..r]の範囲の値の和を求める．
// T Bitree::add(int idx, T val)
// ; idx の要素の値をval増やす
// ; 1-indexed．
//
// %verified
// 
// %references
// プログラミングコンテストチャレンジブック



template<typename T = ll>
class Bitree {
public:
    int size_;
    vector<T> data_;
    Bitree(int size_) :size_(size_), data_(size_+1) {}

    // 1..rの範囲の値の和を求める．
    T sum(int r) const {
        T s = 0;
        while (r) {
            s += data_[r];
            r -= r&-r; //  r&-r と書くことで最下のビットを得る
        }
        return s;
    }
    // l..rの範囲の値の和を求める
    inline T sum(int l, int r) const { return sum(r) - sum(l-1); }
    // idxの要素の値をval増やす
    void add(int idx, T val) {
        while (idx <= size_){
            data_[idx] += val;
            idx += idx&-idx;
        }
    }
};
