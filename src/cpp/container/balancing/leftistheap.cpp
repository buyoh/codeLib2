// %title
// LeftistHeap
// 
// %overview
// 優先度付きキューに加えて，2つのHeapを併合出来る機能を持つ．
// std::priority_queueとは異なって，最小値が取り出される仕様に注意．
// size()は無いので，必要ならメンバ変数を1つ加えてあれこれ．
// 時間計算量は O(log_2(N))．リスト実装なので，配列実装のHeapと比べて低速かもしれない．
//
// %usage
//
// %verified
// 乱数
// 
// %references
// http://hos.ac/blog/#blog0001


template<typename T>
// using T = int;
class LeftistHeap {
    struct Node {
        T value;
        unique_ptr<Node> left, right;
        int dist = 0;
        inline Node(const T& _value) :value(_value) { }
        template<typename ...Args>
        inline Node(Args&&... args) : value(args...) { }
    };
    static inline unique_ptr<Node>&& meldNode(unique_ptr<Node>&& heap1, unique_ptr<Node>&& heap2) {
        if (!heap1) return move(heap2);
        if (!heap2) return move(heap1);
        if (!(heap1->value < heap2->value)) heap1.swap(heap2);
        heap1->right = meldNode(move(heap1->right), move(heap2));
        if (!heap1->left || heap1->left->dist < heap1->right->dist)
            heap1->left.swap(heap1->right);
        heap1->dist = heap1->right ? heap1->right->dist + 1 : 1;
        return move(heap1);
    }
    unique_ptr<Node> root;

public:
    LeftistHeap() {}

    inline bool empty() const { return !root; }
    inline const T& top() const { return root->value; }
    inline void push(const T& val) {
        root = meldNode(move(root), make_unique<Node>(val));
    }
    template<typename ...Args>
    inline void emplace(Args&&... args) {
        root = meldNode(move(root), make_unique<Node>(args...));
    }
    inline void pop() {
        root = meldNode(move(root->left), move(root->right));
    }
    inline void meld(LeftistHeap&& another) {
        root = meldNode(move(root), move(another.root));
    }
};
