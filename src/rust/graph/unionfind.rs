// %=BEGIN DOC
// %title
// UnionFind
//
// %overview
// same 等の取得関数であっても mut を要求する。
// 頂点を拡張したいときは、parent_num.push(-1) をする。
//
// %usage
//
// %require
// ```
// ```
// %verified
// codingame-fall-challenge
// %references
//
// %=END DOC
// %=BEGIN CODE

#[derive(Clone)]
struct UnionFind {
    // negative: num in the component
    // positive: parent idx
    parent_num: Vec<i32>,
}

impl UnionFind {
    fn new(n: usize) -> Self {
        Self {
            parent_num: vec![-1; n],
        }
    }
    fn root(&mut self, i: usize) -> usize {
        if self.parent_num[i] < 0 {
            i
        } else {
            let p = self.parent_num[i] as usize;
            let r = self.root(p);
            self.parent_num[i] = r as i32;
            r
        }
    }
    fn count(&mut self, i: usize) -> usize {
        if self.parent_num[i] < 0 {
            (-self.parent_num[i]) as usize
        } else {
            let r = self.root(i);
            self.count(r)
        }
    }
    fn same(&mut self, i: usize, j: usize) -> bool {
        self.root(i) == self.root(j)
    }
    fn connect(&mut self, i: usize, j: usize) -> bool {
        let ri = self.root(i);
        let rj = self.root(j);
        let ci = -self.parent_num[ri];
        let cj = -self.parent_num[rj];
        if ri != rj {
            if cj < ci {
                self.parent_num[ri] += self.parent_num[rj];
                self.parent_num[rj] = ri as i32;
            } else {
                self.parent_num[rj] += self.parent_num[ri];
                self.parent_num[ri] = rj as i32;
            }
            true
        } else {
            false
        }
    }
}

// %=END CODE
