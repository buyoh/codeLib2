#ifndef SRC_CPP_CONTAINER_LIKERMQ_SEGMENTTREE_HATOPOPPOS_HPP__
#define SRC_CPP_CONTAINER_LIKERMQ_SEGMENTTREE_HATOPOPPOS_HPP__
// %=BEGIN DOC
// %title
// 遅延セグメント木(bitset風味)(sparse)
//
// %overview
// 次のクエリを処理できる．
// - 区間の要素をすべて1にする
// - 区間の要素をすべて0にする．
// - 1になっている区間の要素の個数を求める．
// 0-indexedで，[begin,end)．beginを含み，endを含まない．
// 反転クエリも書ける。
//
// %usage
//
// %require
// ```
#include <memory>
using namespace std;
using ll = long long;
// ```
// %verified
// https://yukicoder.me/submissions/251514
// 速度が出てない
//
// %references
//
//
// %=END DOC
// %=BEGIN CODE

struct SegmentTreeS {
  struct Node {
    ll val_;
    bool lazy_ = false;
    unique_ptr<Node> bottom[2];

    Node(ll v, bool lz = false) : val_(v), lazy_(lz) {}

    inline void setup_bottom() {
      if (!bottom[0])
        bottom[0].reset(new Node(0));
      if (!bottom[1])
        bottom[1].reset(new Node(0));
    }

    inline void apply_lazy() {
      bottom[0]->lazy_ = true;
      bottom[0]->val_ = val_ / 2;
      bottom[1]->lazy_ = true;
      bottom[1]->val_ = val_ / 2;
    }

    void set_range(ll begin, ll end, ll left, ll right) {
      if (end <= left || right <= begin)
        return;  // out of range
      if (begin <= left && right <= end) {
        val_ = right - left;
        lazy_ = true;
        return;
      }
      setup_bottom();
      if (lazy_)
        apply_lazy();

      ll center = (right + left) / 2;
      bottom[0]->set_range(begin, end, left, center);
      bottom[1]->set_range(begin, end, center, right);
      val_ = (bottom[0]->val_ + bottom[1]->val_);
    }

    void reset_range(ll begin, ll end, ll left, ll right) {
      if (end <= left || right <= begin)
        return;  // out of range
      if (begin <= left && right <= end) {
        val_ = right - left;
        lazy_ = true;
        return;
      }
      setup_bottom();
      if (lazy_)
        apply_lazy();

      ll center = (right + left) / 2;
      bottom[0]->reset_range(begin, end, left, center);
      bottom[1]->reset_range(begin, end, center, right);
      val_ = (bottom[0]->val_ + bottom[1]->val_);
    }

    ll count_range(ll begin, ll end, ll left, ll right) {
      if (end <= left || right <= begin)
        return 0;
      if (begin <= left && right <= end)
        return val_;
      if (val_ == 0 || val_ == right - left)
        return val_;
      ll center = (right + left) / 2;
      return bottom[0]->count_range(begin, end, left, center) +
             bottom[1]->count_range(begin, end, center, right);
    }
  };

  ll width;
  Node root;

  SegmentTreeS(ll w) : root(0, false) {
    width = 8;
    while (width < w)
      width <<= 1;
  }

  inline void set_range(ll begin, ll end) { root.set_range(begin, end, 0, width); }
  inline void reset_range(ll begin, ll end) { root.reset_range(begin, end, 0, width); }
  inline ll count_range(ll begin, ll end) { return root.count_range(begin, end, 0, width); }
};
// %=END CODE
#endif  // SRC_CPP_CONTAINER_LIKERMQ_SEGMENTTREE_HATOPOPPOS_HPP__
