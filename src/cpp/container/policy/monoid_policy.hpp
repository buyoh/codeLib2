#ifndef SRC_CPP_CONTAINER_POLICY_MONOID_POLICY_HPP__
#define SRC_CPP_CONTAINER_POLICY_MONOID_POLICY_HPP__
// %=BEGIN DOC
// %title
// モノイドの Policy（インターフェース）
//
// %overview
// データ構造（セグメント木，平衡二分木など）に渡すモノイドの定義を，
// 純粋仮想関数によるインターフェースとして与える．
// データ構造は Policy P を型引数に取り，is_base_of で継承を検査する．
//
// Policy の実装には次を要求する．
// - public 継承し，下記の関数を public で override する．final を付けることを推奨する
// - 状態を持たない（デフォルト構築して使う）
// - テンプレートの Policy では基底が依存型になるため，
//   value_type（と operator_type）を再宣言する（monoid_policy_impl_*.hpp を参照）
// C++17 を想定しているため，private 継承でも is_base_of の検査は通ってしまう．
// その場合，value_type 等を再宣言しないとデータ構造から参照できない．
//
// データ構造側は P().P::operate(a, b) のように修飾付きで呼び出し，
// 仮想関数呼び出しのコストを避ける．
//
// - MonoidPolicy<Value>
//   - value_type operate(const value_type& a, const value_type& b) const
//     ; 結合的な演算（可換は仮定しない）
//   - value_type unit() const
//     ; operate の単位元
// - LazyMonoidPolicy<Value, Operator> : MonoidPolicy<Value>
//   作用素（遅延評価）を加えたもの．MonoidPolicy を要求するデータ構造にも渡せる．
//   - value_type mapping(const operator_type& f, const value_type& x) const
//     ; f を x に作用させる．空でない区間の集約値・要素にのみ適用される
//   - operator_type composition(const operator_type& f, const operator_type& g) const
//     ; f∘g（g を先に適用する）
//   - operator_type id() const
//     ; 恒等作用素
// - GroupPolicy<Value> : MonoidPolicy<Value>
//   逆元を加えたもの（群）．区間の値を前置和の差で求めるデータ構造（BIT など）で使う．
//   MonoidPolicy を要求するデータ構造にも渡せる．
//   - value_type inverse(const value_type& a) const
//     ; operate(a, inverse(a)) == operate(inverse(a), a) == unit()
//
// 可換性・冪等性などの性質は型で表せないため，各データ構造のドキュメントで要求する．
//
// %usage
// // 区間最小値（作用なし）
// struct Min final : MonoidPolicy<int> {
//   value_type operate(const value_type& a, const value_type& b) const override {
//     return min(a, b);
//   }
//   value_type unit() const override { return numeric_limits<value_type>::max(); }
// };
//
// // データ構造側の検査
// template <class P>
// class Container {
//   using value_type = typename P::value_type;
//   static_assert(is_base_of<MonoidPolicy<value_type>, P>::value, "");
//   static value_type operate_(const value_type& a, const value_type& b) {
//     return P().P::operate(a, b);
//   }
// };
//
// 実装例は monoid_policy_impl_*.hpp を参照．
//
// %require
// ```
// ```
// %verified
//
// %references
//
// %words
// monoid, group, policy, lazy
// %=END DOC
// %=BEGIN CODE

template <class Value>
struct MonoidPolicy {
  using value_type = Value;
  // 結合的な演算（可換は仮定しない）
  virtual value_type operate(const value_type& a, const value_type& b) const = 0;
  // operate の単位元
  virtual value_type unit() const = 0;

 protected:
  ~MonoidPolicy() = default;
};

template <class Value, class Operator>
struct LazyMonoidPolicy : MonoidPolicy<Value> {
  using value_type = Value;
  using operator_type = Operator;
  // f を x に作用させる．空でない区間の集約値・要素にのみ適用される
  virtual value_type mapping(const operator_type& f, const value_type& x) const = 0;
  // f∘g（g を先に適用する）
  virtual operator_type composition(const operator_type& f, const operator_type& g) const = 0;
  // 恒等作用素
  virtual operator_type id() const = 0;

 protected:
  ~LazyMonoidPolicy() = default;
};

template <class Value>
struct GroupPolicy : MonoidPolicy<Value> {
  using value_type = Value;
  // operate(a, inverse(a)) == operate(inverse(a), a) == unit()
  virtual value_type inverse(const value_type& a) const = 0;

 protected:
  ~GroupPolicy() = default;
};

// %=END CODE
#endif  // SRC_CPP_CONTAINER_POLICY_MONOID_POLICY_HPP__
