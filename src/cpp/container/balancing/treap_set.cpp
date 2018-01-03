// %title
// Treap(set)
// 
// %overview
// Treapとは，次を満たす確率的平衝二分探索木である．
// - keyについてみると，木は二分探索木になっている．
// - priority(randomized)についてみると，木はHeapになっている．
// 
// 実装したTreapは，map(set)のような機能を持つだけ．
// split/mergeも出来ない．
// 
// 
// keyはユニーク＆これ以上Nodeを追加しないという条件を加えれば区間クエリは書けそう．
// 例えば『キーがx以下のNodeにyを加算する』は，treap[x]とtreap[x].childlen[0]に対してLazy実装で実現できる．
//
// %usage
// value_t& Treap::operator[](key_t key)
// ; keyに対応するvalの参照を得る．無いなら作る．
// unique_ptr<TreapNode>& Treap::find(key_t key)
// ; keyに対応するvalのスマートポインタの参照を得る．
// ; keyに対応するvalが存在しないなら，空のスマートポインタの参照を得る．
// void Treap::erase(key_t key)
// ; keyを削除する．
//
// %verified
// 
// %references
// https://www.slideshare.net/iwiwi/2-12188757
// http://www.prefield.com/algorithm/container/treap.html

struct TreapNode {
    typedef int key_t;
    typedef long long value_t;
    typedef mt19937_64 randomizer_t;
    static const key_t init_key_ = 0;
    static const value_t init_value_ = 0;
    static randomizer_t rnd;

    unique_ptr<TreapNode> childlen[2];
    key_t key;
    value_t value;
    randomizer_t::result_type priority;

    TreapNode(key_t k = init_key_, value_t v = init_value_) :key(k), value(v), priority(rnd() | 1) {}
    TreapNode(key_t k, value_t v, randomizer_t::result_type p) :key(k), value(v), priority(p) {}

    //inline Node& operator[](size_t i) { return *childlen[i & 1]; }
    //inline bool has(size_t i) { return (bool)childlen[i & 1]; }
};
TreapNode::randomizer_t TreapNode::rnd = TreapNode::randomizer_t();



class Treap : public unique_ptr<TreapNode> {
public:
    using key_t = TreapNode::key_t;
    using value_t = TreapNode::value_t;
    static const key_t init_key_ = TreapNode::init_key_;
    static const value_t init_value_ = TreapNode::init_value_;

    Treap() {}
    Treap(TreapNode* p):unique_ptr<TreapNode>(p){}


    // base.childlen[swap_for]がbaseの位置に来るように回転させる．
    static void _rotate(unique_ptr<TreapNode>& base, size_t swap_for) {
        swap_for &= 1;
        unique_ptr<TreapNode> swf = move(base->childlen[swap_for]);
        base->childlen[swap_for] = move(swf->childlen[swap_for ^ 1]);
        swf->childlen[swap_for ^ 1] = move(base);
        base = move(swf);
    }

    // thisをrootとする部分木にkをkeyとするNodeが無ければ作る，あれば返す．
    inline TreapNode& _touch(unique_ptr<TreapNode>& node, key_t k) {
        if (k == node->key)
            return *node;
        size_t i = node->key < k;
        if (node->childlen[i]) {
            auto& r = _touch(node->childlen[i], k);
            if (node->priority > node->childlen[i]->priority)
                _rotate(node, i);
            return r;
        }
        else {
            node->childlen[i].reset(new TreapNode(k));
            return *(node->childlen[i]);
        }
    }

    // map[]と同じ
    value_t& operator[](key_t key) {
        if (!(*this)) {
            reset(new TreapNode());
            return (*this)->value;
        }
        else {
            return _touch(*this, key).value;
        }
    }

    // keyが等しいnodeを探す
    unique_ptr<TreapNode>& _find(unique_ptr<TreapNode>& ptr, key_t key) {
        return (!ptr || ptr->key == key) ? ptr : _find(ptr->childlen[ptr->key < key], key);
    }

    // keyが等しいnodeを探す
    // 存在しないなら空のunique_ptrの参照が返る
    inline unique_ptr<TreapNode>& find(key_t key) {
        return _find(*this, key);
    }


    // ptrを削除する．
    void _erase(unique_ptr<TreapNode>& ptr) {
        if (!ptr->childlen[0] && !ptr->childlen[1]) {
            ptr.release();
        }
        else {
            bool i = ptr->childlen[0] ? 0 : ptr->childlen[1] ? 1 : ptr->childlen[0]->priority > ptr->childlen[1]->priority;
            _rotate(ptr, i);
            _erase(ptr->childlen[i ^ 1]);
        }
    }

    // keyを持つnodeを削除する
    inline void erase(key_t key) {
        unique_ptr<TreapNode>& node = _find(*this, key);
        if (node) _erase(node);
    }

};


// validate

// mt19937_64 randdev(8901016);
// inline int rand_range(int l, int h) {
//     return uniform_int_distribution<int>(l, h)(randdev);
// }
// 
// int main() {
// 
//     Treap tp;
//     map<int, int> mp;
// 
//     unordered_set<int> erased;
// 
//     for (int lop = 0; lop < 200000; ++lop) {
//         int k = rand_range(1, 5000);
//         int v = rand_range(0, 5000);
//         if (v == 0) {
//             tp.erase(k);
//             mp.erase(k);
//             erased.insert(k);
//         }
//         else {
//             tp[k] = v;
//             mp[k] = v;
//             if (erased.count(k)) erased.erase(k);
//         }
//     }
// 
//     for (auto p : mp) {
//         assert(tp[p.first] == p.second);
//     }
//     for (auto e : erased) {
//         assert(bool(tp.find(e)) == bool(mp.count(e)));
//     }
// 
//     return 0;
// }