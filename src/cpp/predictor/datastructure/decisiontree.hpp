#ifndef SRC_CPP_PREDICTOR_DATASTRUCTURE_DECISIONTREE_HPP__
#define SRC_CPP_PREDICTOR_DATASTRUCTURE_DECISIONTREE_HPP__
// %=BEGIN DOC
// %title
// 決定木
//
// %overview
// 決定木
//
// %usage
//
// %require
// ```
#include <memory>
#include <functional>
using namespace std;
// ```
// %verified
//
// %references
//
// %=END DOC
// %=BEGIN CODE

template <typename input_t, typename output_t>
class DecisionTree {
  using comparator_t = function<bool(input_t)>;

  bool leaf_;
  comparator_t comparator_;
  output_t out_;
  unique_ptr<DecisionTree> childlen_[2];

 public:
  DecisionTree() : leaf_(true), comparator_(nullptr) {}

  inline DecisionTree& operator[](int i) { return *childlen_[i]; }
  inline bool leaf() { return leaf_; }

  inline void generate_leaf(output_t out) {
    leaf_ = true;
    childlen_[0].release();
    childlen_[1].release();
    out_ = out;
  }

  inline void generate_branch(comparator_t comparator) {
    leaf_ = false;
    childlen_[0].reset(new DecisionTree());
    childlen_[1].reset(new DecisionTree());
    comparator_ = comparator;
  }

  output_t eval(input_t val) { return leaf() ? out_ : childlen_[comparator_(val)]->eval(val); }
};
// %=END CODE
#endif  // SRC_CPP_PREDICTOR_DATASTRUCTURE_DECISIONTREE_HPP__
