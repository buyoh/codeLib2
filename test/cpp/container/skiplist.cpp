#pragma GCC optimize("O3")
#include <bits/stdc++.h>

using namespace std;
using ll = long long int;

using T = int;
struct SkipList {
  struct Node;
  struct Skip;
  struct Elem;

  struct Skip {
    unique_ptr<Skip> up;
    Node* node;
    Skip* next;
    Skip* prev;

    inline Skip(Node* _node, Skip* _next = nullptr, Skip* _prev = nullptr) : up(), node(_node), next(_next), prev(_prev) {}

    void detach() {
      if (up) {
        up->detach();
        up.reset();
      }
      prev->next = next;
      next->prev = prev;
    }
  };
  struct Node {
    unique_ptr<Node> next;
    unique_ptr<Skip> up;
    Node* prev;

    virtual ~Node() {}

    inline Node() : next(), up(), prev() {}

    // template <class... Args>
    // inline Node(Args&&... args)
    //   :value(forward<Args>(args)...), next(), up(), prev(_prev) {}
  };

  struct Elem : public Node {
    T value;
    inline Elem(const T& _value = T()) : Node(), value(_value) {}
  };

  struct iterator {
   private:
    Node* ptr;

   public:
    inline iterator(Node* _ptr) : ptr(_ptr) {}

    inline T& operator*() { return static_cast<Elem*>(ptr)->value; }
    inline T* operator->() const {  // we don't need implement without const
      return &(static_cast<Elem*>(ptr)->value);
    }
    inline iterator& operator++() {
      if (ptr->next)
        ptr = ptr->next.get();
      return *this;
    }
    inline iterator& operator--() {
      if (ptr->prev)
        ptr = ptr->prev;
      return *this;
    }
    inline bool operator==(const iterator& i) noexcept { return ptr == i.ptr; }
    inline bool operator!=(const iterator& i) noexcept { return ptr != i.ptr; }

    friend SkipList;
  };

  //

  Node head, *tail_ptr;

  //

  // TODO: portable
  static mt19937 randMT;
  static inline bool randBool() { return randMT() > mt19937::max() >> 1; }

  // private
  Skip& growHeadSkipTower() {
    unique_ptr<Skip>* s = &(head.up);
    while (*s)
      s = &((*s)->up);
    return *(*s = make_unique<Skip>(&head));
  }

  // private
  Skip& growHeadTailTower() {
    unique_ptr<Skip>* s = &(tail_ptr->up);
    while (*s)
      s = &((*s)->up);
    return *(*s = make_unique<Skip>(tail_ptr));
  }

  // private
  void createSkipTower(unique_ptr<Skip>& uskip, Node* root) {
    if (randBool())
      return;
    uskip = make_unique<Skip>(root);
    Skip* ptr = uskip.get();
    // before

    /*

    うーん設計が破綻してきたような。綺麗じゃない。
    今の設計だと、1段目のskip作成が特殊実装になってしまう。全隣接の要素を持つクラスはSkipではないので
    Skipにすると、Skipの所有者は上だけか右だけにしか伸ばさない実装にしたいので、全部繋げられない。
    全リンクのSkip層を用意すれば楽だが、冗長。


    */
  }

  void clear() {
    head.next = make_unique<Node>();
    head.up.reset();
    tail_ptr = head.next.get();
  }

  inline iterator begin() { return iterator(head.next.get()); }

  inline iterator end() { return iterator(tail_ptr); }

  // TODO: faster
  iterator operator[](int i) {
    Node* ptr = head.next.get();
    while (--i >= 0 && ptr->next) {
      ptr = ptr->next.get();
    }
    return iterator(ptr);
  }

  iterator insert(iterator prev, const T& value) {
    Node* node = prev.ptr;
    if (!node->next)
      throw out_of_range("");
    auto nxt = move(node->next);
    node->next = make_unique<Elem>(value);
    node->next->next = move(nxt);
    createSkipTower(*(node->next));
    return iterator(&*(node->next));
  }

  inline SkipList() {}
};

//

int main() {
  return 0;
}
