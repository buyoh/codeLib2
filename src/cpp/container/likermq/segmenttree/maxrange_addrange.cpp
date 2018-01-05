// %title
// セグメント木(区間加算，区間最大，単独書換)
// 
// %overview
// 次のクエリを処理できる．
// - １つの要素を書き換える．
// - 区間の要素すべてに加算する．
// - 区間の要素の最大値を計算する．
// 0-indexedで，[begin,end)．beginを含み，endを含まない．
// 何度も加減算を繰り返すと内部でオーバーフローを起こす可能性．
// インデックスは size_t ではなく int で扱う．[TODO]
// O(log^2N)．最大値のみの機能なら出来そう．[TODO]
//
// %usage
// SegmentTree<typename T>(int n)
// ; [0,n) の配列を確保する．
// T SegmentTree::get_val(int idx)
// ; idx の要素の値を取得する．
// void SegmentTree::set_val(int idx, T e)
// ; idx の要素の値をeに書き換える．
// void SegmentTree::add_valrange(int begin, int end, T e)
// ; 区間[begin,end)にeを加算する
// void SegmentTree::get_maxrange(int idx, T e)
// ; 区間[begin,end)の最大値を計算する
// void SegmentTree::get_maxrangeIdx(int idx, T e)
// ; 区間[begin,end)の最大値が存在する要素のインデックスを取得する
//
// %verified
// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2521389#1 (setval, getminrange)
// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=2521434#1 (addvalrange, getval)
// https://yukicoder.me/submissions/227863 (addvalrange, getminrange)
// 
// %references
// プログラミングコンテストチャレンジブック
// https://www.slideshare.net/kazumamikami1/ss-15160153




template<typename T>
//typedef ll T;
class SegmentTree {
    int size_;
    vector<T> valdata_;
    vector<T> addtree_;
    vector<T> maxtree_;

    int _geti(const vector<T>& v, int idx) const {
        if (size_ - 1 <= idx) return (idx - size_ + 1);
        // if (idx < 0) return idx;
        return v[idx];
    }
public:

    SegmentTree(int n) {
        size_ = 8;
        while (size_ < n) size_ <<= 1;
        maxtree_.resize(size_);
        valdata_.resize(size_);
        addtree_.resize(size_);
        flat();
    }

    // tree初期化
    void flat() {
        for (int i = size_ - 2; 0 <= i; --i) {
            int c = 2 * i + 1;
            if (size_ - 1 <= c) {
                maxtree_[i] = c - size_ + 1;
            }
            else {
                maxtree_[i] = maxtree_[c];
            }
        }
    }

    // addtreeを0にして，valdataに加算する．
    // void apply(T sum = 0, int ptr = 0) {
    //     if (size_ - 1 <= ptr) {
    //         valdata_[ptr - size_ + 1] += sum;
    //     }
    //     else {
    //         sum += addtree_[ptr];
    //         apply(sum, ptr * 2 + 1);
    //         apply(sum, ptr * 2 + 2);
    //         addtree_[ptr] = 0;
    //     }
    // }

    void fill(T val) {
        std::fill(valdata_.begin(), valdata_.end(), val);
        std::fill(addtree_.begin(), addtree_.end(), 0);
        flat();
    }

    // T operator[](int index) const {
    //     
    // }

    T get_val(int index) const {
        T sum = valdata_[index];
        index += size_ - 1;
        while (0 < index) {
            index = (index - 1) / 2; // 親へ移動
            sum += addtree_[index];
        }
        return sum;
    }

    void set_val(int index, const T e) {
        valdata_[index] = e;

        for (int i = index + size_ - 1; 0 < i;) {
            i = (i - 1) / 2; // 親へ移動
            valdata_[index] -= addtree_[i];
        }
        index += size_ - 1;
        while (0 < index) {
            index = (index - 1) / 2; // 親へ移動
            int c1 = index * 2 + 1;
            int c2 = index * 2 + 2;
            int i1, i2;
            i1 = _geti(maxtree_, c1);
            i2 = _geti(maxtree_, c2);
            maxtree_[index] = get_val(i1) > get_val(i2) ? i1 : i2;
        }
    }

    void _add_valrange(int begin, int end, const T e, int ptr, int rangebegin, int rangeend) {

        if (rangeend <= begin || end <= rangebegin) return;
        if (begin <= rangebegin && rangeend <= end) {
            if (size_ - 1 <= ptr)
                valdata_[ptr - size_ + 1] += e;
            else
                addtree_[ptr] += e;
            return;
        }

        int rangemid = (rangebegin + rangeend) / 2;
        _add_valrange(begin, end, e, ptr * 2 + 1, rangebegin, rangemid);
        _add_valrange(begin, end, e, ptr * 2 + 2, rangemid, rangeend);

        int i1, i2;
        i1 = _geti(maxtree_, ptr * 2 + 1);
        i2 = _geti(maxtree_, ptr * 2 + 2);
        maxtree_[ptr] = get_val(i1) > get_val(i2) ? i1 : i2;
    }

    int _get_maxrangeIdx(int begin, int end, int ptr, int rangebegin, int rangeend) const {

        if (rangeend <= begin || end <= rangebegin) return begin; // note:範囲外なら適当な範囲内のindexを返す
        if (begin <= rangebegin && rangeend <= end) return _geti(maxtree_, ptr);

        int rangemid = (rangebegin + rangeend) / 2;
        int l = _get_maxrangeIdx(begin, end, ptr * 2 + 1, rangebegin, rangemid);
        int r = _get_maxrangeIdx(begin, end, ptr * 2 + 2, rangemid, rangeend);

        if (get_val(l) >= get_val(r))
            return l;
        else
            return r;
    }

    inline void add_valrange(int begin, int end, const T e) {
        _add_valrange(begin, end, e, 0, 0, size_);
    }

    inline int get_maxrangeIdx(int begin, int end) const {
        return _get_maxrangeIdx(begin, end, 0, 0, size_);
    }

    inline T get_maxrange(int begin, int end) {
        return get_val(_get_maxrangeIdx(begin, end, 0, 0, size_));
    }
};
