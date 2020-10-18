#ifndef SRC_CPP_CONTAINER_LIKERMQ_BIT_HPP__
#define SRC_CPP_CONTAINER_LIKERMQ_BIT_HPP__
// %=BEGIN DOC
// %title
// Mo's algorithm
//
// %overview
// 以下の計算時間がtの時、区間クエリを時間O(tNsqrt(N))で処理できる。
// - 計算済み区間に隣接する要素1つを加えた区間の計算(add)
// - 計算済み区間の端の要素1つを削除した区間の計算(del)
// 抽象クラスとして実装しているので、実装する場合はこのクラスを継承して、
// add,delメソッドをオーバーライドする
//
// 区間を結合できるような場合、セグメント木が利用できるので、そちらを使う。
//
// %usage
//
// %require
// ```
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
// ```
// %verified
// TODO:
// https://atcoder.jp/contests/abc174/submissions/15639599
//
// %references
// https://snuke.hatenablog.com/entry/2016/07/01/000000
// %=END DOC
// %=BEGIN CODE

class MoAlgo {
 private:
  int n_, sqn_;
  std::vector<std::pair<int, int>> ranges_;
  bool mosCompare(int l, int r) const {
    return (ranges_[l].first / sqn_ != ranges_[r].first / sqn_)
               ? ranges_[l].first < ranges_[r].first
               : ranges_[l].second < ranges_[r].second;
  }

 protected:
  // 現在の状態に要素indexを追加
  virtual void add(int index) = 0;
  // 現在の状態から要素indexを削除
  virtual void del(int index) = 0;
  // i番目のクエリと一致した段階で呼ばれる
  virtual void query(int index) = 0;

 public:
  MoAlgo(int n) : n_(n), sqn_(std::sqrt(n)) {}
  void rangeReserve(int n) { ranges_.reserve(n); }
  // [b, e)
  void rangeEmplace(int b, int e) { ranges_.emplace_back(b, e); }
  void solve() {
    const int sq = ;
    const int q = ranges_.size();
    std::vector<int> qi(q);
    std::iota(qi.begin(), qi.end(), 0);
    std::sort(qi.begin(), qi.end(), mosCompare);
    int nl = 0, nr = 0;  // [nl,nr)
    for (int i : qi) {
      while (nl > ranges_[i].first)
        --nl, add(nl);
      while (nr < ranges_[i].second)
        add(nr), ++nr;
      while (nl < ranges_[i].first)
        del(nl), ++nl;
      while (nr > ranges_[i].second)
        --nr, del(nr);
      query(nl, nr);
    }
  }
};
// %=END CODE
#endif  // SRC_CPP_CONTAINER_LIKERMQ_BIT_HPP__
