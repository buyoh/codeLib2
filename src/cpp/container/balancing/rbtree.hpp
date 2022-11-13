

// Incomplete!
using T = int;  // TODO: bugfix
class RedBlackTree {
 public:
  // note; rootは黒
  struct Node {
    T value;
    unique_ptr<Node> left, right;
    int8_t black;

    inline Node(const T& _value) : value(_value), black(0) {}
    inline Node(T&& _value) : value(_value), black(0) {}

    inline bool isRed() const noexcept { return black == 0; }
    inline bool isBlack() const noexcept { return black == 1; }
    inline void pushBlack() {
      --(this->black);
      ++(left->black);
      ++(right->black);
    }
    inline void pullBlack() {
      ++(this->black);
      --(left->black);
      --(right->black);
    }

    bool include(const T& v) const {
      if (value == v)
        return true;
      else if (v < value)
        return left ? left->include(v) : false;
      else
        return right ? right->include(v) : false;
    }

    // 制約を満たしているかどうかassertする．
    // 葉から根への経路に含まれる黒の個数が返る
    int checkProperty() {
      // 性質0: 色はvalid
      assert(black == 0 || black == 1);

      // 性質1：葉から根への経路には黒が同じ個数個存在
      int l = left ? left->checkProperty() : -1;
      int r = right ? right->checkProperty() : -1;
      assert(l == -1 || r == -1 || l == r);

      // 性質2：赤は隣接しない
      if (isRed()) {
        assert(!left || left->isBlack());
        assert(!right || right->isBlack());
      }

      // 性質3：leftが黒ならrightも黒
      if ((bool)left && (bool)right)
        assert(left->isRed() || right->isBlack());

      return l == -1 && r == -1 ? 0 : l == -1 ? r : l;
    }

    static inline void rotateLeft(unique_ptr<Node>& node) {
      auto& ref = node->right->left;
      node.swap(node->right);
      node.swap(ref);
    }
    static inline void rotateRight(unique_ptr<Node>& node) {
      auto& ref = node->left->right;
      node.swap(node->left);
      node.swap(ref);
    }
    static inline void flipLeft(unique_ptr<Node>& node) {
      swap(node->black, node->right->black);
      rotateLeft(node);
    }
    static inline void flipRight(unique_ptr<Node>& node) {
      swap(node->black, node->left->black);
      rotateRight(node);
    }

    static inline void insertFixup(unique_ptr<Node>& grand,
                                   unique_ptr<Node>& parent,
                                   unique_ptr<Node>& node) {
      if (!parent) {
        node->black = 1;
        return;
      }
      if (parent->left->isBlack())
        flipLeft(parent);
      if (parent->isRed()) {
        if (grand->right->isBlack())
          flipRight(grand);
        else
          grand->pushBlack();
      }
    }

    static bool insert(unique_ptr<Node>& grand,
                       unique_ptr<Node>& parent,
                       unique_ptr<Node>& node,
                       const T& newValue) {
      if (!node) {
        node = make_unique<Node>(newValue);
        return true;
      }
      if (node->value < newValue) {
        if (!insert(parent, node, node->left, newValue))
          return false;
      } else if (node->value == newValue)
        return false;
      else {
        if (!insert(parent, node, node->right, newValue))
          return false;
      }
      insertFixup(grand, parent, node);
      return true;
    }

    static inline void eraseFixup(unique_ptr<Node>& parent, unique_ptr<Node>& node) {
      // parent, nodeはポインタの参照なので，参考文献とは差がある
      if (!node || node->isRed())
        return;
      if (!parent) {
        node->black = 1;
        return;
      }
      if (parent->left->isRed()) {
        // case1
        flipRight(parent);
        eraseFixup(parent->right, parent->right->right);
      } else if (parent->left == node) {
        // case2
        parent->pullBlack();
        flipLeft(parent);
        if (parent->left->right->isRed()) {
          rotateLeft(parent->left);
          flipRight(parent);
          parent->pushBlack();
          if (parent->right->right->isRed()) {
            flipLeft(parent->right);
          }
        }
      } else {
        // case3
        parent->pullBlack();
        flipRight(parent);
        if (parent->right->left->isRed()) {
          rotateRight(parent->right);
          flipLeft(parent);
          parent->pushBlack();
        } else {
          if (parent->left->isRed()) {
            parent->left->pushBlack();
          } else {
            flipLeft(parent);
          }
        }
      }
    }

    static void removeFindingMinDFS(unique_ptr<Node>& rem, unique_ptr<Node>& curr) {
      if (!(curr->left)) {
        // node->rightをrootとする部分木のうち最小値ノードを探す
        // curr := 最小値ノードを持つunique_ptr
        auto removed = move(rem);
        rem = move(curr);
        curr = move(rem->right);
        rem->left = move(removed->left);
        rem->right = move(removed->right);
        rem->black += removed->black;  // special
      } else
        removeFindingMinDFS(rem, curr->left);
      // eraseFixup(curr); // note: いらない？
    }

    static inline void remove(unique_ptr<Node>& node) {
      // note: nodeはfixupされていない
      assert((bool)node);
      if (!(node->right)) {
        auto removed = move(node);
        node = move(removed->left);
        return;
      }
      // node->rightをrootとする部分木のうち最小値ノードを探す
      // curr := 最小値ノードを持つunique_ptrのポインタ
      removeFindingMinDFS(node, node->right);
      return;
    }

    static bool erase(unique_ptr<Node>& parent, unique_ptr<Node>& node, const T& targetValue) {
      if (!node)
        return false;
      if (node->value == targetValue) {
        remove(node);
      } else if (targetValue < node->value) {
        if (!erase(node, node->left, targetValue))
          return false;
      } else {
        if (!erase(node, node->right, targetValue))
          return false;
      }
      eraseFixup(parent, node);
      return true;
    }
  };

  unique_ptr<Node> root;

  RedBlackTree() {}

  inline bool insert(const T& x) {
    unique_ptr<Node> none;
    return Node::insert(none, none, root, x);
  }

  inline bool include(const T& x) const { return root ? root->include(x) : false; }

  inline bool erase(const T& x) {
    if (root) {
      unique_ptr<Node> none;
      return Node::erase(none, root, x);
    }
    return false;
  }

  inline void checkProperty() {
    if (root)
      root->checkProperty();
  }
};

int main() {
  vector<int> li = {1, 2, 3, 4, 5, 6, 7, 8};

  do {
    RedBlackTree s2;

    for (auto e : li) {
      s2.insert(e);
      s2.checkProperty();
    }
    for (auto e : li) {
      s2.erase(e);
      s2.checkProperty();
    }

  } while (next_permutation(ALL(li)));

  return 0;
}
s