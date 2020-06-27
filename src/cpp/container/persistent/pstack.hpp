#ifndef SRC_CPP_CONTAINER_PERSISTENT_PSTACK_HPP__
#define SRC_CPP_CONTAINER_PERSISTENT_PSTACK_HPP__
// %=BEGIN DOC
// %title
// 永続スタック
//
// %overview
// 通常のスタックの機能に加えて，undoを持つ．
// undoした操作をundo出来る．
//
// %usage
// void undo(int count)
// ; count回数もとに戻る
// vector<value_t> get()
// ; 現在のスタックの状態を取得する
//
// %require
// ```
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;
// ```
//
// %verified
// 乱数
//
// %references
// https://www.slideshare.net/qnighy/re-15443018
// %words
// persistent,stack,eizoku
// %require
//
// %=END DOC
// %=BEGIN CODE

struct PersistentStack {
  using value_t = int;
  struct Node {
    enum Type { PUSH, POP, NONE };
    Type type;
    value_t val;
    int next_idx;
    Node(Type t = PUSH, value_t v = 0) : type(t), val(v) {}
  };

  vector<Node> history;

  PersistentStack() { history.emplace_back(Node::Type::NONE); }

  inline void push(value_t val) {
    Node node = Node(Node::Type::PUSH, val);
    node.next_idx = history.size() - 1;
    history.push_back(node);
  }
  inline void pop() {
    Node node = Node(Node::Type::POP);
    node.next_idx = history.size() - 1;
    history.push_back(node);
  }

  inline void undo(int count) {
    int n = history.size();
    assert(0 <= (n - 1 - count));
    history.push_back(history[n - 1 - count]);
  }

  inline vector<value_t> get() {
    Node* p = &(history.back());

    vector<value_t> res;
    int popcount = 0;
    while (p->type != Node::Type::NONE) {
      if (p->type == Node::Type::PUSH) {
        if (popcount > 0)
          --popcount;
        else
          res.push_back(p->val);
      } else if (p->type == Node::Type::POP) {
        ++popcount;
      }
      p = &(history[p->next_idx]);
    }
    reverse(res.begin(), res.end());
    return res;
  }
};
// %=END CODE
#endif  // SRC_CPP_CONTAINER_PERSISTENT_PSTACK_HPP__
