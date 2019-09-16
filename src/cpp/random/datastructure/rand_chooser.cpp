// %title
// 禁止可能な一様分布の乱数
//
// %overview
// 一部の整数を取り出せないようにできるような，一様分布の乱数を生成する．
// invalidな引数を与えた場合の動作は未定義．
// 空間計算量 O(size) に注意する．
//
// %usage
// RandChooser(size_t size)
// ; [0, size-1] の範囲の整数の乱数を生成するようなインスタンスの生成
// inline int RandChooser::operator()(RANDOM &rd)
// rd : 乱数生成器．
// ; 乱数生成する．
// inline int RandChooser::disable(int idx)
// ; 整数idxを次回以降の乱数生成で選ばれないように設定する．
// ; 既にdisableした値をもう一度disableする操作はinvalid．
//
// %verified
//
// %references
//

class RandChooser {
  size_t size;
  size_t left_size;
  vector<int> swapper;
  vector<int> unswapper;

 public:
  RandChooser(size_t s) : size(s), left_size(s), swapper(s), unswapper(s) {
    for (int i = 0; i < s; ++i) {
      swapper[i] = unswapper[i] = i;
    }
  }
  inline size_t left() { return left_size; }

  template <typename RANDOM>
  inline int operator()(RANDOM& rd) {
    uniform_int_distribution<int> rand_int(0, left_size - 1);
    return swapper[rand_int(rd)];
  }

  inline int disable(int idx) {
    swap(swapper[unswapper[idx]], swapper[left_size - 1]);
    unswapper[swapper[unswapper[idx]]] = unswapper[idx];
    return --left_size;
  }
};