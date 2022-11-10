#ifndef SRC_CPP_IMPLEMENTS_DATASTRUCTURE_COMMANDCHAIN_HPP__
#define SRC_CPP_IMPLEMENTS_DATASTRUCTURE_COMMANDCHAIN_HPP__
// %=BEGIN DOC
// %title
// 前半を共有できるリスト
//
// %overview
// 落ちゲーのビームサーチなどでコマンドを記憶する時などにつかえる。
// 以下の機能を持つ。
// - 列の末尾に要素を追加する (定数時間)
// - 列を複製する (定数時間)
// - 列をstd::vectorに書き出す (要素数に比例する時間)
// デストラクタの実行時間については改善を行っていないので、注意。
// 競技プログラミング用途であれば、強制メモリリークも考えてみる。
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

// using T = int;
template <typename T>
class CommandChain {
 public:
  static shared_ptr<CommandChain> createEmpty() {
    static shared_ptr<CommandChain> p = shared_ptr<CommandChain>(new CommandChain());
    return p;
  }

  static shared_ptr<CommandChain> pushed(shared_ptr<CommandChain>& me, T cmd) {
    assert(me.get());
    auto new_chain = make_shared<CommandChain>();
    new_chain->prev_ = me;
    new_chain->node_ = std::move(cmd);
    return new_chain;
  }
  void dumpRefTo(vector<const T*>& out) const {
    if (prev_.get()) {
      prev_->dumpRefTo(out);
      // root does not have value
      out.push_back(&node_);
    }
  }
  vector<const T*> dumpRef() const {
    vector<const T*> out;
    dumpRefTo(out);
    return out;
  }

  void dumpTo(vector<T>& out) const {
    if (prev_.get()) {
      prev_->dumpTo(out);
      // root does not have value
      out.push_back(node_);
    }
  }
  vector<T> dump() const {
    vector<T> out;
    dumpTo(out);
    return out;
  }

  // TODO: make private...
  CommandChain() : prev_(), node_() {}
  // CommandChain(shared_ptr<CommandChain> &prev) : prev_(prev), node_() {}

 private:
  shared_ptr<CommandChain> prev_;
  T node_;
};

#endif  // SRC_CPP_IMPLEMENTS_DATASTRUCTURE_COMMANDCHAIN_HPP__
