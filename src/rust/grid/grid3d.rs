// %=BEGIN DOC
// %title
// 三次元グリッド
//
// %overview
// Pt は二次元グリッドのものと同一
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

type PUnit = i32;

#[derive(Debug, Clone, Copy, PartialEq, PartialOrd)]
struct Pt {
    y: PUnit,
    x: PUnit,
}

impl fmt::Display for Pt {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        // println!("{}", 12);
        // println!("{:?}", 12); // debug
        write!(f, "({},{})", self.x, self.y)
    }
}

impl ops::Neg for Pt {
    type Output = Self;

    fn neg(self) -> Self::Output {
        Self {
            y: -self.y,
            x: -self.x,
        }
    }
}

impl ops::Add for Pt {
    type Output = Self;

    fn add(self, rhs: Self) -> Self::Output {
        Self {
            y: self.y + rhs.y,
            x: self.x + rhs.x,
        }
    }
}

impl ops::AddAssign for Pt {
    fn add_assign(&mut self, rhs: Self) {
        self.y += rhs.y;
        self.x += rhs.x;
    }
}

impl ops::Sub for Pt {
    type Output = Self;

    fn sub(self, rhs: Self) -> Self::Output {
        Self {
            y: self.y - rhs.y,
            x: self.x - rhs.x,
        }
    }
}

impl Pt {
    fn newp(p: (i32, i32)) -> Self {
        Pt { y: p.0, x: p.1 }
    }

    fn zero() -> Self {
        Pt { y: 0, x: 0 }
    }

    fn is_zero(&self) -> bool {
        self.y == 0 && self.x == 0
    }

    fn mul(&self, rhs: PUnit) -> Self {
        Self {
            y: self.y * rhs,
            x: self.x * rhs,
        }
    }

    fn contain_rect(&self, left_up: Pt, right_down: Pt) -> bool {
        left_up.x <= self.x
            && self.x <= right_down.x
            && left_up.y <= self.y
            && self.y <= right_down.y
    }
}

// ------------------------------------

#[derive(Clone)]
struct Fi3d<T> {
    height: usize,
    width: usize,
    depth: usize,
    data: Vec<T>,
}

impl<T: Clone> Fi3d<T> {
    fn new(height: usize, width: usize, depth: usize, default: T) -> Self {
        Self {
            height,
            width,
            depth,
            data: vec![default; height * width * depth],
        }
    }
}
impl<T> Fi3d<T> {
    fn get(&self, yxd: (usize, usize, usize)) -> Option<&T> {
        if 0 <= yxd.0 && yxd.0 < self.height && 0 <= yxd.1 && yxd.1 < self.width && 0 <= yxd.2
            && yxd.2 < self.depth
        {
            Some(&self[yxd])
        } else {
            None
        }
    }
    fn get_mut(&mut self, yxd: (usize, usize, usize)) -> Option<&mut T> {
        if 0 <= yxd.0 && yxd.0 < self.height && 0 <= yxd.1 && yxd.1 < self.width && 0 <= yxd.2
            && yxd.2 < self.depth
        {
            Some(&mut self[yxd])
        } else {
            None
        }
    }
    fn geti(&self, yxd: (i32, i32, i32)) -> Option<&T> {
        if 0 <= yxd.0 && yxd.0 < self.height as i32 && 0 <= yxd.1 && yxd.1 < self.width as i32
            && 0 <= yxd.2 && yxd.2 < self.depth as i32
        {
            Some(&self[yxd])
        } else {
            None
        }
    }
    fn geti_mut(&mut self, yxd: (i32, i32, i32)) -> Option<&mut T> {
        if 0 <= yxd.0 && yxd.0 < self.height as i32 && 0 <= yxd.1 && yxd.1 < self.width as i32
            && 0 <= yxd.2 && yxd.2 < self.depth as i32
        {
            Some(&mut self[yxd])
        } else {
            None
        }
    }
}

impl<T> ops::Index<(usize, usize, usize)> for Fi3d<T> {
    type Output = T;
    fn index(&self, yxd: (usize, usize, usize)) -> &Self::Output {
        &self.data[self.width * self.height * yxd.0 + self.width * yxd.1 + yxd.2]
    }
}

impl<T> ops::IndexMut<(usize, usize, usize)> for Fi3d<T> {
    fn index_mut(&mut self, yxd: (usize, usize, usize)) -> &mut Self::Output {
        &mut self.data[self.width * self.height * yxd.0 + self.width * yxd.1 + yxd.2]
    }
}

impl<T> ops::Index<(i32, i32, i32)> for Fi3d<T> {
    type Output = T;
    fn index(&self, yxd: (i32, i32, i32)) -> &Self::Output {
        &self.data[(self.width as i32 * self.height as i32 * yxd.0
            + self.width as i32 * yxd.1
            + yxd.2) as usize]
    }
}

impl<T> ops::IndexMut<(i32, i32, i32)> for Fi3d<T> {
    fn index_mut(&mut self, yxd: (i32, i32, i32)) -> &mut Self::Output {
        &mut self.data[(self.width as i32 * self.height as i32 * yxd.0
            + self.width as i32 * yxd.1
            + yxd.2) as usize]
    }
}


// %=END CODE
