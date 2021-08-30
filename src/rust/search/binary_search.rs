// %=BEGIN DOC
// %title
// 二分探索(整数)
//
// %overview
//
//
// %usage
//
// %require
// ```
// ```
// %verified
// rand
//
// %references
//
// %=END DOC
// %=BEGIN CODE
fn binary_search<F: Fn(i64) -> bool>(ok_val: i64, ng_val: i64, check: F) -> i64 {
    let mut ok = ok_val;
    let mut ng = ng_val;
    while (ok - ng).abs() > 1 {
        let m = (ok + ng) / 2;
        if check(m) {
            ok = m;
        } else {
            ng = m;
        }
    }
    ok
}
// %=END CODE
