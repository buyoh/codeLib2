// %=BEGIN DOC
// %title
// 二次元グリッド
//
// %overview
// work in progress
// %usage
//
// %require
// ```
// ```
// %verified
//
// %references
//
// %=END DOC
// %=BEGIN CODE
struct Field<T> {
    h: usize,
    w: usize,
    dat: Vec<T>,
}

impl<T: Clone> Field<T> {
    fn new(h: usize, w: usize, zero: T) -> Self {
        Self {
            h,
            w,
            dat: vec![zero; w * h as usize],
        }
    }
    fn fill(&mut self, val: T) {
        self.dat.fill(val);
    }
    fn atc(&self, y: i32, x: i32) -> &T {
        &self.dat[(y % self.h as i32) as usize * self.w + (x % self.w as i32) as usize]
    }
    fn atc_mut(&mut self, y: i32, x: i32) -> &mut T {
        &mut self.dat[(y % self.h as i32) as usize * self.w + (x % self.w as i32) as usize]
    }
}

impl<T> std::ops::Index<(usize, usize)> for Field<T> {
    type Output = T;
    fn index(&self, yx: (usize, usize)) -> &Self::Output {
        &self.dat[yx.0 * self.w + yx.1]
    }
}
impl<T> std::ops::IndexMut<(usize, usize)> for Field<T> {
    fn index_mut(&mut self, yx: (usize, usize)) -> &mut Self::Output {
        &mut self.dat[yx.0 * self.w + yx.1]
    }
}
// %=END CODE
