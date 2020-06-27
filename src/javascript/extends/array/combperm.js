// %=BEGIN DOC
// %title
// 組合せ・順列列挙
// 
// %overview
// ジェネレータを返す．ジェネレータは，組合せ・順列を列挙する．
// ruby の Array#permutation，Array#combination に似た挙動．
// 高等なアルゴリズムは使わず，雑な再帰で実装．
//
// %usage
// for (let e of [1,2,3,4].combination(2)) console.log(e);
//
// %verified
// 
// %references
// https://qiita.com/komaji504/items/62a0f8ea43053e90555a
// %words
// shuffle
// %require
// 
// %=END DOC
// %=BEGIN CODE

Array.prototype.permutation = function* () {
    if (this.length <= 0) return;
    const n = this.length;
    const this_ = this;
    function* dfs(p) {
        if (p.length == n) {
            yield p.map(i => this_[i]);
            return;
        }
        for (let i = 0; i < n; ++i) {
            if (p.some(e => e == i)) continue;
            p.push(i);
            for (let r of dfs(p)) yield r;
            p.pop();
        }
    }
    for (let r of dfs([])) yield r;
};

Array.prototype.combination = function* (k) {
    if (this.length <= 0 || (this.length < k)) return;
    const n = this.length;
    const this_ = this;
    function* dfs(c, idx) {
        if (n - idx < k - c.length) return;
        if (idx == n) {
            if (c.length == k)
                yield c.map(i => this_[i]);
            return;
        }
        if (k > c.length) {
            c.push(idx);
            for (let r of dfs(c, idx + 1)) yield r;
            c.pop();
        }
        for (let r of dfs(c, idx + 1)) yield r;
    }
    for (let r of dfs([], 0)) yield r;
};
// %=END CODE
