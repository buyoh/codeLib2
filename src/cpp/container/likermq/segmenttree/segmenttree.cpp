// %title
// セグメント木
// 
// %overview
// 次のクエリを処理できる．時間計算量O(log_2(N))
// - 1つの要素を書き換える
// - 1つの要素に加算する
// - 1つの要素の値を求める
// - 区間の要素すべてを1つの値で埋める
// - 区間の要素すべてに加算する
// - 区間の要素の和を求める
// - 区間の要素の最大値を計算する
// 0-indexedで，[begin,end)．beginを含み，endを含まない．
// 空間計算量はO(4NX+NC)．X は sizeof(T)，C は sizeof(struct{int;bool;})．
// 何度も加減算を繰り返すと内部でオーバーフローを起こす可能性．
// インデックスは size_t ではなく int で扱う．
//
// %usage
//
// %verified
// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=3208770#1
// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=3208769#1
// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=3208768#1
// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=3208771#1
// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=3208772#1
// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=3208773#1
// http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=3208777#1
// 
// %references
// プログラミングコンテストチャレンジブック
// https://www.slideshare.net/kazumamikami1/ss-15160153

template<typename T>
//using T = ll;
class SegmentTree {
public:
    struct IdxVal {
        int index;
        T value;
        inline IdxVal(int _i = 0, const T& _v = T()) :index(_i), value(_v) { }
        inline void set(int i, const T& v) { index = i; value = v; }
        inline bool operator<(const T& t) const { return value < t; }
        inline bool operator==(const T& t) const { return value == t; }
        inline bool operator<(const IdxVal& iv) const { return value < iv.value || (value == iv.value && index < iv.index); }
        inline bool operator==(const IdxVal& iv) const { return index == iv.index && value == iv.value; }
    };

    enum struct Cmd{
        None, Add, Flat
    };

    struct Node {
        T sum, lazyValue;
        IdxVal max;
        Cmd lazyType;
    };

private:
    const int size_;
    vector<T> leaf_data_;
    vector<Node> node_data_;
    T* leaf_;
    Node* node_;

    void _resize() {
        leaf_data_.resize(size_);
        node_data_.resize(size_ - 1);

        leaf_ = &leaf_data_[0];
        node_ = &node_data_[0] - 1;
    }

    void _init() {
        for (int i = size_ - 1; 0 < i; --i) {
            int c = i << 1;
            if (size_ <= c)
                node_[i].max.set(c - size_, leaf_[c - size_]);
            else
                node_[i].max = node_[c].max;
            node_[i].lazyType = Cmd::None;
            node_[i].sum = node_[i].lazyValue = 0;
        }
    }
public:

    inline void fill(T val) {
        std::fill(leaf_data_.begin(), leaf_data_.end(), val);
        _init();
    }

    SegmentTree(int _n) :
        size_([](int _n) {int s = 8; while (s < _n) s <<= 1; return s; }(_n))
    {
        _resize();
        _init();
    }
    SegmentTree(int _n, const T& _fillVal) :
        size_([](int _n) {int s = 8; while (s < _n) s <<= 1; return s; }(_n))
    {
        _resize();
        fill(_fillVal);
    }

private:
    // lazyを適応する(子を呼ぶ前に呼ぶ)
    // width: ptrが担当するnodeの範囲の大きさ(ptr = 1 ならば size_)
    inline void _applyDown(int ptr, int width) {
        width >>= 1;
        //if (size_ <= ptr) return;
        if (node_[ptr].lazyType == Cmd::Add) {
            int c = ptr << 1;

            if (size_ <= c) {
                leaf_[c - size_] += node_[ptr].lazyValue;
                leaf_[c + 1 - size_] += node_[ptr].lazyValue;
            }
            else {
                node_[c].sum += node_[ptr].lazyValue * width;
                node_[c].max.value += node_[ptr].lazyValue;
                node_[c].lazyValue += node_[ptr].lazyValue;
                if (node_[c].lazyType == Cmd::None)
                    node_[c].lazyType = Cmd::Add;

                node_[c + 1].sum += node_[ptr].lazyValue * width;
                node_[c + 1].max.value += node_[ptr].lazyValue;
                node_[c + 1].lazyValue += node_[ptr].lazyValue;
                if (node_[c + 1].lazyType == Cmd::None)
                    node_[c + 1].lazyType = Cmd::Add;
            }

            node_[ptr].lazyType = Cmd::None;
            node_[ptr].lazyValue = 0;
        }
        else if (node_[ptr].lazyType == Cmd::Flat) {
            node_[ptr].lazyType = Cmd::None;
            int c = ptr << 1;

            if (size_ <= c) {
                leaf_[c - size_] = node_[ptr].lazyValue;
                leaf_[c + 1 - size_] = node_[ptr].lazyValue;
            }
            else {
                node_[c].sum = node_[ptr].lazyValue * width;
                node_[c].max.value = node_[ptr].lazyValue;
                node_[c].lazyValue += node_[ptr].lazyValue;
                node_[c].lazyType = Cmd::Flat;

                node_[c + 1].sum = node_[ptr].lazyValue * width;
                node_[c + 1].max.value = node_[ptr].lazyValue;
                node_[c + 1].lazyValue += node_[ptr].lazyValue;
                node_[c + 1].lazyType = Cmd::Flat;
            }

            node_[ptr].lazyType = Cmd::None;
            node_[ptr].lazyValue = 0;
        }
    }

    // lazyを適応する(子を呼んだ後に呼ぶ)
    inline void _applyUp(int ptr) {
        if (size_ <= ptr << 1) {
            int i = (ptr << 1) - size_;
            if (leaf_[i] < leaf_[i + 1])
                node_[ptr].max.set(i + 1, leaf_[i + 1]);
            else
                node_[ptr].max.set(i, leaf_[i]);
            node_[ptr].sum = leaf_[i] + leaf_[i + 1];
        }
        else {
            if (node_[(ptr << 1)].max.value < node_[(ptr << 1) + 1].max.value)
                node_[ptr].max = node_[(ptr << 1) + 1].max;
            else
                node_[ptr].max = node_[(ptr << 1)].max;
            node_[ptr].sum = node_[ptr << 1].sum + node_[(ptr << 1) + 1].sum;
        }
    }

    // _downward_applyDown(ptr1): leaf[ptr1]の値を正しくする．
    void _downward_applyDown(int ptr1, int width = 1) {
        if (1 < ptr1) _downward_applyDown(ptr1 >> 1, width << 1);
        _applyDown(ptr1, width);
    }


    void _setValueRange(int begin, int end, T val, int ptr, int rangebegin, int rangeend) {

        if (rangeend <= begin || end <= rangebegin) return; // note:範囲外
        if (begin <= rangebegin && rangeend <= end) {
            if (size_ <= ptr) {
                leaf_[rangebegin] = val;
            }
            else {
                node_[ptr].sum = val * (rangeend - rangebegin);
                node_[ptr].max.value = val;
                node_[ptr].lazyType = Cmd::Flat;
                node_[ptr].lazyValue = val;
            }
            return;
        }

        _applyDown(ptr, rangeend - rangebegin);

        _setValueRange(begin, end, val, ptr << 1, rangebegin, (rangebegin + rangeend) >> 1);
        _setValueRange(begin, end, val, (ptr << 1) + 1, (rangebegin + rangeend) >> 1, rangeend);

        _applyUp(ptr);
    }

    void _addValueRange(int begin, int end, T val, int ptr, int rangebegin, int rangeend) {

        if (rangeend <= begin || end <= rangebegin) return; // note:範囲外
        if (begin <= rangebegin && rangeend <= end) {
            if (size_ <= ptr) {
                leaf_[rangebegin] += val;
            }
            else {
                node_[ptr].sum += val * (rangeend - rangebegin);
                node_[ptr].max.value += val;
                node_[ptr].lazyValue += val;
                if (node_[ptr].lazyType == Cmd::None)
                    node_[ptr].lazyType = Cmd::Add;
            }
            return;
        }

        _applyDown(ptr, rangeend - rangebegin);

        _addValueRange(begin, end, val, ptr << 1, rangebegin, (rangebegin + rangeend) >> 1);
        _addValueRange(begin, end, val, (ptr << 1) + 1, (rangebegin + rangeend) >> 1, rangeend);

        _applyUp(ptr);
    }

    T _getSumRange(int begin, int end, int ptr, int rangebegin, int rangeend) {
        if (rangeend <= begin || end <= rangebegin) return 0; // note:範囲外
        if (begin <= rangebegin && rangeend <= end) {
            if (size_ <= ptr)
                return leaf_[rangebegin];
            else
                return node_[ptr].sum;
        }

        _applyDown(ptr, rangeend - rangebegin);

        return _getSumRange(begin, end, ptr << 1, rangebegin, (rangebegin + rangeend) >> 1)
            + _getSumRange(begin, end, (ptr << 1) + 1, (rangebegin + rangeend) >> 1, rangeend);
    }

    IdxVal _getMaxRange(int begin, int end, int ptr, int rangebegin, int rangeend) {
        if (rangeend <= begin || end <= rangebegin) return IdxVal(-1); // note:範囲外
        if (begin <= rangebegin && rangeend <= end) {
            if (size_ <= ptr)
                return IdxVal(rangebegin, leaf_[rangebegin]);
            else
                return node_[ptr].max;
        }

        _applyDown(ptr, rangeend - rangebegin);

        auto l = _getMaxRange(begin, end, ptr << 1, rangebegin, (rangebegin + rangeend) >> 1);
        auto r = _getMaxRange(begin, end, (ptr << 1) + 1, (rangebegin + rangeend) >> 1, rangeend);

        if (l.index == -1) return move(r);
        if (r.index == -1) return move(l);
        return l.value < r.value ? move(r) : move(l);
    }

public:
    // [index] の値を求める
    inline T getValue(int index) {
        _downward_applyDown((index + size_) >> 1);
        return leaf_[index];
    }

    // [index] の値を書き換える
    inline T setValue(int index, T val) {
        _downward_applyDown((index + size_) >> 1);
        T diff = val - leaf_[index];
        leaf_[index] = val;

        for (int ptr = (index + size_); 1 < ptr;) {
            ptr >>= 1;
            node_[ptr].sum -= diff;
            _applyUp(ptr);
        }
    }

    // [index] の値に加算する
    inline T addValue(int index, T val) {
        _downward_applyDown((index + size_) >> 1);
        leaf_[index] += val;

        for (int ptr = (index + size_); 1 < ptr;) {
            ptr >>= 1;
            node_[ptr].sum += val;
            _applyUp(ptr);
        }
    }

    // 区間[begin,end)全てにvalをセットする
    inline void setValueRange(int begin, int end, T val) {
        _setValueRange(begin, end, val, 1, 0, size_);
    }

    // 区間[begin,end)に一様にvalを加算する
    inline void addValueRange(int begin, int end, T val) {
        _addValueRange(begin, end, val, 1, 0, size_);
    }

    // 区間[begin,end)の和を求める
    inline T getSumRange(int begin, int end) {
        return _getSumRange(begin, end, 1, 0, size_);
    }

    // 区間[begin,end)の最大値とその位置を求める
    inline IdxVal getMaxRange(int begin, int end) {
        return _getMaxRange(begin, end, 1, 0, size_);
    }
};

