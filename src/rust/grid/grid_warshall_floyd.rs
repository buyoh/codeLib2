// %=BEGIN DOC
// %title
// WarshallFloyd (グリッド用)
//
// %overview
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

struct WarShallFloydF {
    width: usize,
    height: usize,
    table: Fi<usize>,
}

impl WarShallFloydF {
    fn new(height: usize, width: usize) -> Self {
        Self {
            width,
            height,
            table: Fi::<usize>::new(width * height, width * height, usize::max_value()),
        }
    }

    fn solve(&mut self) {
        let n = self.width * self.height;
        for i in 0..n {
            for j in 0..n {
                for k in 0..n {
                    self.table[(j, k)] =
                        self.table[(j, k)].min(self.table[(j, i)] + self.table[(i, k)]);
                }
            }
        }
    }
}

impl ops::Index<((usize, usize), (usize, usize))> for WarShallFloydF {
    type Output = usize;
    fn index(&self, (yx0, yx1): ((usize, usize), (usize, usize))) -> &Self::Output {
        let p = self.width * yx0.0 + yx0.1;
        let q = self.width * yx1.0 + yx1.1;
        &self.table[(p, q)]
    }
}

impl ops::IndexMut<((usize, usize), (usize, usize))> for WarShallFloydF {
    fn index_mut(&mut self, (yx0, yx1): ((usize, usize), (usize, usize))) -> &mut Self::Output {
        let p = self.width * yx0.0 + yx0.1;
        let q = self.width * yx1.0 + yx1.1;
        &mut self.table[(p, q)]
    }
}

impl ops::Index<((i32, i32), (i32, i32))> for WarShallFloydF {
    type Output = usize;
    fn index(&self, (yx0, yx1): ((i32, i32), (i32, i32))) -> &Self::Output {
        let p = self.width as i32 * yx0.0 + yx0.1;
        let q = self.width as i32 * yx1.0 + yx1.1;
        &self.table[(p, q)]
    }
}

impl ops::IndexMut<((i32, i32), (i32, i32))> for WarShallFloydF {
    fn index_mut(&mut self, (yx0, yx1): ((i32, i32), (i32, i32))) -> &mut Self::Output {
        let p = self.width as i32 * yx0.0 + yx0.1;
        let q = self.width as i32 * yx1.0 + yx1.1;
        &mut self.table[(p, q)]
    }
}

// %=END CODE
