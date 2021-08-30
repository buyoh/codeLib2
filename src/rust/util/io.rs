// %=BEGIN DOC
// %title
// 標準入力マクロ(競技プログラミング用)
//
// %overview
// JavaのScannerのように値をBufReadから読み取る。
// 数値型・文字列・配列・タプル・タプルの配列に対応。
// バイト単位で読み込むので、インタラクティブ問題にも多分対応出来る。
// タプルを直接渡せない為、専用の記法を用意している。
//
// %usage
// let cin = std::io::stdin().lock();
// let n = scan!(cin => i32);
// let msg = scan!(cin => String);
// let v = scan!(cin => i32 * n);
// let (a, b) = scan!(cin => i32, i32);
// let edges = scan!(cin => i32, i32 * n);
//
// %require
// ```
// ```
// %verified
// https://yukicoder.me/submissions/650278
//
// %references
//
// %=END DOC
// %=BEGIN CODE
fn take_token<R: std::io::BufRead>(cin: &mut R) -> String {
    cin.bytes()
        .map(|c| c.unwrap() as char)
        .skip_while(|c| c.is_whitespace())
        .take_while(|c| !c.is_whitespace())
        .collect::<String>()
}
#[allow(unused)]
macro_rules! scan {
    ($io:expr => $t:ty) => (take_token(&mut $io).parse::<$t>().unwrap());
    ($io:expr => $t:tt * $n:expr) => ((0..$n).map(|_| scan!($io => $t)).collect::<Vec<_>>());
    ($io:expr => $($t:tt),*) => (($(scan!($io => $t)),*));
    ($io:expr => $($t:tt),* * $n:expr) => ((0..$n).map(|_| ($(scan!($io => $t)),*)).collect::<Vec<_>>());
}
// %=END CODE
