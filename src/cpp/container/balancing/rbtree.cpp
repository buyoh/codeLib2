
// child < parentを満たす．
class RBTree {
 public:
  typedef int value_t;
  static const value_t init_value = 0;

  struct Node {
    unique_ptr<Node> childlen[2];
    value_t value;
    bool color;  // 1-> red
    int rank;
    Node(value_t v = init_value, bool c = 0, int r = 0) : value(v), color(c), rank(r) {}
    inline Node& operator[](size_t i) { return *childlen[i & 1]; }
    inline bool has(size_t i) { return (bool)childlen[i & 1]; }
    inline void create(size_t i) { childlen[i & 1].reset(new Node()); }
  };

  unique_ptr<Node> root;

  RBTree() {}
  RBTree(value_t v, bool c = 0) : root(new Node(v, c)) {}

  void push(value_t val) {}

  Node* _merge(Node* a, Node* b) {}

  // another を this に加える．
  // another は空になる．
  void merge(RBTree& another) {
    if (!another.root)
      return;
    if (!root) {
      root.swap(another.root);
      return;
    }
  }
};
