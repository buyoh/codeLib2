// %=BEGIN DOC
// %title
// 二次元配列・テーブル
//
// %overview
//
// %usage
//
// %require
// ```
// ```
// %verified
// codingame-fall-challenge 2023
// %references
//
// %words
// field,table,2darray
// %=END DOC
// %=BEGIN CODE

#[derive(Clone)]
struct Table<T> {
    height: usize,
    width: usize,
    data: Vec<T>,
}

impl<T: Clone> Table<T> {
    fn new(height: usize, width: usize, default: T) -> Self {
        Self {
            height,
            width,
            data: vec![default; height * width],
        }
    }
    fn fill(&mut self, default: T) {
        self.data.fill(default);
    }
}

impl<T> Table<T> {
    // 関数名わからん
    fn new_generate<F: FnMut(usize, usize) -> T>(height: usize, width: usize, mut con: F) -> Self {
        let mut data = Vec::<T>::new();
        data.reserve(height * width);
        for y in 0..height {
            for x in 0..width {
                data.push(con(y, x));
            }
        }
        Self {
            height,
            width,
            data,
        }
    }
    fn get(&self, yx: (usize, usize)) -> Option<&T> {
        if 0 <= yx.0 && yx.0 < self.height && 0 <= yx.1 && yx.1 < self.width {
            Some(&self[yx])
        } else {
            None
        }
    }
    fn get_mut(&mut self, yx: (usize, usize)) -> Option<&mut T> {
        if 0 <= yx.0 && yx.0 < self.height && 0 <= yx.1 && yx.1 < self.width {
            Some(&mut self[yx])
        } else {
            None
        }
    }
    fn geti(&self, yx: (i32, i32)) -> Option<&T> {
        if 0 <= yx.0 && yx.0 < self.height as i32 && 0 <= yx.1 && yx.1 < self.width as i32 {
            Some(&self[yx])
        } else {
            None
        }
    }
    fn geti_mut(&mut self, yx: (i32, i32)) -> Option<&mut T> {
        if 0 <= yx.0 && yx.0 < self.height as i32 && 0 <= yx.1 && yx.1 < self.width as i32 {
            Some(&mut self[yx])
        } else {
            None
        }
    }

    fn write_fmt<FN: FnMut(&mut fmt::Formatter, &T) -> fmt::Result>(
        &self,
        f: &mut fmt::Formatter,
        mut cb: FN,
    ) -> fmt::Result {
        if let Err(e) = write!(f, "[{}x{}]", self.width, self.height) {
            return Err(e);
        }
        for y in 0..self.height {
            if let Err(e) = writeln!(f) {
                return Err(e);
            }
            for x in 0..self.width {
                if let Err(e) = cb(f, &self[(y, x)]) {
                    return Err(e);
                }
            }
        }
        writeln!(f)
    }
    fn eprintln<FN: FnMut(&T)>(&self, mut cb: FN) {
        // FIXME: うまい書き方
        eprintln!("[{}x{}]", self.width, self.height);
        for y in 0..self.height {
            eprintln!();
            for x in 0..self.width {
                cb(&self[(y, x)]);
            }
        }
        eprintln!()
    }
}

impl<T> ops::Index<(usize, usize)> for Table<T> {
    type Output = T;
    fn index(&self, yx: (usize, usize)) -> &Self::Output {
        &self.data[self.width * yx.0 + yx.1]
    }
}

impl<T> ops::IndexMut<(usize, usize)> for Table<T> {
    fn index_mut(&mut self, yx: (usize, usize)) -> &mut Self::Output {
        &mut self.data[self.width * yx.0 + yx.1]
    }
}

impl<T> ops::Index<(i32, i32)> for Table<T> {
    type Output = T;
    fn index(&self, yx: (i32, i32)) -> &Self::Output {
        &self.data[(self.width as i32 * yx.0 + yx.1) as usize]
    }
}

impl<T> ops::IndexMut<(i32, i32)> for Table<T> {
    fn index_mut(&mut self, yx: (i32, i32)) -> &mut Self::Output {
        &mut self.data[(self.width as i32 * yx.0 + yx.1) as usize]
    }
}

impl<T: fmt::Display> fmt::Debug for Table<T> {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        self.write_fmt(f, |f, t| write!(f, "{} ", t))
    }
}

// %=END CODE