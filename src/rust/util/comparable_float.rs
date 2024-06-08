// %=BEGIN DOC
// %title
// 浮動小数点の比較
//
// %overview
// PartialEq のみを定義した型に対して Ord を実装する。
// 浮動小数点は NaN を含むため比較可能でないが、競プロ等の用途では NaN は扱わないことが多い。
// MinNonNan を使い、 NaN を含むときは panic させ、それ以外は通常の比較を行うことで実装を簡略化する。
//
// %usage
// let Some(x) = v.iter().min_by_key(|x| MinNonNan(*x))
//
// %require
// ```
// ```
// %verified
// codingame-fall-challenge 2023
// %references
//
// %=END DOC
// %=BEGIN CODE

// Compareable for f64
#[derive(PartialEq, PartialOrd)]
struct MinNonNan<T>(T);

impl<T: PartialEq> Eq for MinNonNan<T> {}

impl<T: PartialOrd> Ord for MinNonNan<T> {
    fn cmp(&self, other: &MinNonNan<T>) -> std::cmp::Ordering {
        self.0.partial_cmp(&other.0).unwrap() // panic for NaN
    }
}
// %=END CODE
