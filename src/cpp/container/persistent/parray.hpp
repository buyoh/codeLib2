#ifndef SRC_CPP_CONTAINER_PERSISTENT_PARRAY_HPP__
#define SRC_CPP_CONTAINER_PERSISTENT_PARRAY_HPP__
// %=BEGIN
// %title
// 永続配列
//
// %overview
// 通常の固定長配列の機能に加えて，undoを持つ．
// undoした操作をundo出来る．
//
// %usage
// void undo(int count)
// ; count回数もとに戻る
// vector<value_t> get()
// ; 現在の配列を取得する
//
// %require
// ```
#include <vector>
#include <cassert>
using namespace std;
// ```
//
// %verified
// [TODO]
//
// %references
//
// %words
// persistent,stack,eizoku
// %tag
// wip
//
// %=END

struct PersistentArray {
  using value_t = int;
  struct Node {
    enum Type { UPDATE, NONE };
    Type type;
    int index;
    value_t val;
    int next_idx;
    Node(Type t = Type::NONE, int i = 0, value_t v = 0) : type(t), index(i), val(v) {}
  };

  int n;
  vector<Node> history;

  PersistentArray(int _size) : n(_size) { history.emplace_back(Node::Type::NONE); }

  inline void update(int index, value_t val) {
    Node node = Node(Node::Type::UPDATE, val);
    node.next_idx = history.size() - 1;
    history.push_back(node);
  }

  inline void undo(int count) {
    int n = history.size();
    assert(0 <= (n - 1 - count));
    history.push_back(history[n - 1 - count]);
  }

  inline vector<value_t> get() {
    int it = history.size() - 1;

    vector<int> wrote(n);
    vector<value_t> res(n);

    while (history[it].type != Node::Type::NONE) {
      const Node& node = history[it];
      if (node.type == Node::Type::UPDATE) {
        if (!wrote[node.index]) {
          wrote[node.index] = true;
          res[node.index] = node.val;
        }
      }
      it = history[it].next_idx;
    }

    return res;
  }
};
#endif  // SRC_CPP_CONTAINER_PERSISTENT_PARRAY_HPP__
