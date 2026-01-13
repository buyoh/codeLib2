// %title
// Skip List
//
// %overview
// 通常リストはランダムアクセスに線形時間を要するが、
// 複数の要素をスキップして移動出来るようにすることで、
// およそ対数時間でランダムアクセスが可能になる。
// スキップ用のリンクを生成するかどうかは乱数によって決定されるため、
// なんとなく劣化版 Treap の雰囲気がある。
//
//
// %require
// ```
#include <memory>
// ```
// %usage
//
// %words
// %verified
// random test
// %references

#pragma GCC optimize("O3")
#include <bits/stdc++.h>

using namespace std;
using ll = long long int;

template <typename T>
struct SkipList {
  struct Node;
  struct Skip;
  struct Elem;

  struct Skip {
    unique_ptr<Skip> up;
    Skip* down;
    Node* node;  // TODO: 冗長。downとnodeを纏めれたら。でもその場合dynamic_castが…
    Skip* next;
    Skip* prev;
    int jump_next;  // TODO:

    inline explicit Skip(Node* _node, Skip* _down, Skip* _next = nullptr, Skip* _prev = nullptr) : up(), down(_down), node(_node), next(_next), prev(_prev) {}

    void detach() {
      if (up)
        up->detach();
      next->prev = prev;
      prev->next = next;
    }
  };
  struct Node {
    unique_ptr<Node> next;
    unique_ptr<Skip> up;
    Node* prev;

    // Node を SkipList から切り離す。upは開放しない。
    // 自分自身の所有権を返す。
    unique_ptr<Node> detach() {
      if (up)
        up->detach();
      unique_ptr<Node> self = move(prev->next);
      next->prev = prev;
      prev->next = move(next);
      return self;
    }

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
  Skip* headSkipTop_ptr;  // TODO: unnecessary?
  // Skip* tailSkipTop_ptr;
  //

  // TODO: portable
  static mt19937 randMT;
  static bool randBool() { return randMT() > mt19937::max() >> 1; }

  // private
  inline Skip& growHeadSkipTower() {
    if (headSkipTop_ptr) {
      headSkipTop_ptr->up = make_unique<Skip>(&head, headSkipTop_ptr);
      headSkipTop_ptr = &*headSkipTop_ptr->up;
    } else {
      head.up = make_unique<Skip>(&head);
      headSkipTop_ptr = &*head.up;
    }
    return *headSkipTop_ptr;
  }

  // private
  // Skip& growTailSkipTower() {
  //   return tailSkipTop_ptr ? *(tailSkipTop_ptr->up = make_unique<Skip>(tail_ptr)) : *(tailSkipTop_ptr = &*(tail_ptr->up = make_unique<Skip>(tail_ptr)));
  // }

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
    // tailSkipTop_ptr = nullptr;
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

  // nextの直前にvalueを挿入する
  // 挿入した要素のイテレータを返す
  iterator insert(iterator next, const T& value) {
    if (!next.ptr->prev)
      throw out_of_range("");
    Node* node = next.ptr->prev;
    auto nxt = move(node->next);
    node->next = make_unique<Elem>(value);
    node->next->next = move(nxt);
    createSkipTower(*(node->next));
    return iterator(&*(node->next));
  }

  // itr が指す要素を削除する
  // 削除した要素の手前の要素のイテレータを返す
  iterator erase(iterator itr) {
    Node* node = itr.ptr->prev;
    itr.ptr->detach();
    return iterator(node);
  }

  inline void push_front(const T& value) { insert(begin(), value); }
  inline void push_back(const T& value) { insert(end(), value); }
  inline void pop_front() { head.next->detach(); }
  inline void pop_back() { tail_ptr->prev->detach(); }
  inline T& front() { return static_cast<Elem&>(*head.next).value; }
  inline T& back() { return static_cast<Elem*>(tail_ptr->prev)->value; }

  inline SkipList() { clear(); }
};

//

int main() {
  return 0;
}
