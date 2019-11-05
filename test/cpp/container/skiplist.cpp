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

    inline explicit Skip(Node* _node, Skip* _next = nullptr, Skip* _prev = nullptr) : up(), node(_node), next(_next), prev(_prev) {}

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
    inline explicit Elem(const T& _value = T()) : Node(), value(_value) {}
  };

  struct iterator {
   private:
    Node* ptr;

    inline explicit iterator(Node* _ptr) : ptr(_ptr) {}

   public:
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
  Skip *headSkipTop_ptr, *tailSkipTop_ptr;  // TODO: unnecessary?

  //

  // TODO: portable
  static mt19937 randMT;
  static bool randBool() { return randMT() > mt19937::max() >> 1; }

  // private
  inline Skip& growHeadSkipTower() {
    return headSkipTop_ptr ? *(headSkipTop_ptr->up = make_unique<Skip>(&head)) : *(headSkipTop_ptr = &*(head.up = make_unique<Skip>(&head)));
  }

  // private
  Skip& growTailSkipTower() {
    return tailSkipTop_ptr ? *(tailSkipTop_ptr->up = make_unique<Skip>(tail_ptr)) : *(tailSkipTop_ptr = &*(tail_ptr->up = make_unique<Skip>(tail_ptr)));
  }

  void createSkipTowerMore(Skip& skip) {
    if (randBool())
      return;
    skip.up = make_unique<Skip>(skip.node);

    // to find same height neighbor skip...
    Skip *sFoward, *sBackUp, *sFowardUp;
    // move to increment-direction
    sFoward = &skip;
    while (sFoward->next) {
      sFoward = sFoward->next;
      if (sFoward->up)
        break;
    }
    // not found -> sFoward is TAIL
    if (!(sFoward->up))
      sFoward->up = make_unique<Skip>(tail_ptr);
    sFowardUp = &*sFoward->up;
    // immediately, we can find sBack = sFoward->up->prev
    if (sFowardUp->prev)
      sBackUp = sFowardUp->prev;
    // not found -> sBack is HEAD
    else
      sBackUp = &growHeadSkipTower();
    // connect
    sBackUp->next = &*skip.up;
    skip.up->next = sFowardUp;
    sFowardUp->prev = &*skip.up;
    skip.up->prev = sBackUp;

    createSkipTowerMore(*skip.up);
  }

  // private
  void createSkipTower(Node& node) {
    if (randBool())
      return;
    // to find same height neighbor skip...
    Node* sFoward;
    Skip *sBackUp, *sFowardUp;
    // move to increment-direction
    sFoward = &node;
    while (sFoward->next) {
      sFoward = &*sFoward->next;
      if (sFoward->up)
        break;
    }
    // not found -> sFoward is TAIL
    if (!(sFoward->up))
      sFoward->up = make_unique<Skip>(tail_ptr);
    sFowardUp = &*sFoward->up;
    // immediately, we can find sBack = sFoward->up->prev
    if (sFowardUp->prev)
      sBackUp = sFowardUp->prev;
    // not found -> sBack is HEAD
    else
      sBackUp = &growHeadSkipTower();
    // connect
    sBackUp->next = &*node.up;
    node.up->next = sFowardUp;
    sFowardUp->prev = &*node.up;
    node.up->prev = sBackUp;

    createSkipTowerMore(*node.up);
  }

  void clear() {
    head.next = make_unique<Node>();
    head.up.reset();
    tail_ptr = head.next.get();
    headSkipTop_ptr = nullptr;
    tailSkipTop_ptr = nullptr;
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

  inline SkipList() { clear(); }
};

//

int main() {
  return 0;
}
