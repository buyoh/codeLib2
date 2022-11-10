#ifndef SRC_CPP_IMPLEMENTS_DATASTRUCTURE_COMMANDCHAIN2_HPP__
#define SRC_CPP_IMPLEMENTS_DATASTRUCTURE_COMMANDCHAIN2_HPP__
// %=BEGIN DOC
// %title
// 前半を共有できるリスト(2bit)
//
// %overview
// CommandChain派生。2bitに制限する変わりに効率を良くしたもの。
// コマンドが前後左右だけ、などであれば、こちらが早い可能性がある。
// チューニングは不完全なのでもう少し頑張りたい
//
// %usage
// %require
// ```
#include <memory>
#include <vector>
using namespace std;
// ```
// %verified
// %references
// %words
// command
// %=END DOC
// %=BEGIN CODE

class CommandChain {
 public:
  using Direction = int;  // 2bit
  struct Node {
    Node() : value(0), len(0) {}
    void push(Direction dir) {
      value |= (uint64_t)(dir & 3) << (uint64_t)len;
      len += 2;
    }
    bool full() const { return len >= 64; }
    void dump(vector<int>& out) const {
      for (uint64_t p = 0; p < uint64_t(len); p += 2)
        out.push_back((value >> p) & 3);
    }
    int last() const { return (value >> uint64_t(len - 2)) & 3; }

    uint64_t value;
    int len = 0;
  };

  static shared_ptr<CommandChain> createEmpty() { return make_shared<CommandChain>(root()); }

  static shared_ptr<CommandChain> pushed(shared_ptr<CommandChain>& me, Direction cmd) {
    // assert(me.get());
    if (me->node_.full()) {
      auto new_chain = make_shared<CommandChain>(me);
      new_chain->node_.push(cmd);
      return new_chain;
    } else {
      auto copied_chain = make_shared<CommandChain>(*me);
      copied_chain->node_.push(cmd);
      return copied_chain;
    }
  }

  int last() const { return node_.last(); }

  void dumpTo(vector<int>& out) const {
    if (prev_.get()) {
      prev_->dumpTo(out);
      // root does not have value
      node_.dump(out);
    }
  }
  vector<int> dump() const {
    vector<int> out;
    dumpTo(out);
    return out;
  }

  // TODO: make private...
  CommandChain(shared_ptr<CommandChain>& prev) : prev_(prev), node_() {}

 private:
  // friend shared_ptr<CommandChain>;
  static shared_ptr<CommandChain>& root() {
    static shared_ptr<CommandChain> e(new CommandChain());
    return e;
  };

  shared_ptr<CommandChain> prev_;
  Node node_;
  CommandChain() : prev_(), node_() {}
};

#endif  // SRC_CPP_IMPLEMENTS_DATASTRUCTURE_COMMANDCHAIN2_HPP__
