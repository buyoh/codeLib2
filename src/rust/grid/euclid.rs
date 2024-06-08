// %=BEGIN DOC
// %title
// ユークリッド座標
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
// %=END DOC
// %=BEGIN CODE

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
struct Point {
    x: i32,
    y: i32,
}

impl ops::Add for Point {
    type Output = Self;
    fn add(self, rhs: Self) -> Self {
        Point {
            x: self.x + rhs.x,
            y: self.y + rhs.y,
        }
    }
}

impl ops::Sub for Point {
    type Output = Self;
    fn sub(self, rhs: Self) -> Self {
        Point {
            x: self.x - rhs.x,
            y: self.y - rhs.y,
        }
    }
}

impl ops::Mul<i32> for Point {
    type Output = Self;
    fn mul(self, rhs: i32) -> Self {
        Point {
            x: self.x * rhs,
            y: self.y * rhs,
        }
    }
}

impl ops::Div<i32> for Point {
    type Output = Self;
    fn div(self, rhs: i32) -> Self {
        Point {
            x: self.x / rhs,
            y: self.y / rhs,
        }
    }
}

impl ops::AddAssign for Point {
    fn add_assign(&mut self, rhs: Self) {
        *self = *self + rhs;
    }
}

impl ops::SubAssign for Point {
    fn sub_assign(&mut self, rhs: Self) {
        *self = *self - rhs;
    }
}

impl ops::MulAssign<i32> for Point {
    fn mul_assign(&mut self, rhs: i32) {
        *self = *self * rhs;
    }
}

impl ops::DivAssign<i32> for Point {
    fn div_assign(&mut self, rhs: i32) {
        *self = *self / rhs;
    }
}

impl Point {
    fn new(x: i32, y: i32) -> Self {
        Point { x, y }
    }

    fn zero() -> Self {
        Self { x: 0, y: 0 }
    }

    fn is_zero(&self) -> bool {
        self.x == 0 && self.y == 0
    }

    fn hypot2(&self, rhs: Self) -> f64 {
        let dx = (self.x - rhs.x) as f64;
        let dy = (self.y - rhs.y) as f64;
        dx.hypot(dy)
    }

    fn hypot(&self) -> f64 {
        (self.x as f64).hypot(self.y as f64)
    }

    fn moved_to(&self, destination: Self, distance: i32) -> Self {
        let d = destination - *self;
        if d.is_zero() {
            *self
        } else {
            let r = (distance as f64 / d.hypot()).min(1.0);
            *self + Point::new((d.x as f64 * r) as i32, (d.y as f64 * r) as i32)
        }
    }
}

#[derive(Clone, Debug)]
struct Rect {
    left: i32,
    top: i32,
    right: i32,
    bottom: i32,
}

impl Rect {
    fn new(left: i32, top: i32, right: i32, bottom: i32) -> Self {
        Self {
            left,
            top,
            right,
            bottom,
        }
    }

    fn intersection(&self, rhs: &Self) -> Option<Self> {
        let left = self.left.max(rhs.left);
        let top = self.top.max(rhs.top);
        let right = self.right.min(rhs.right);
        let bottom = self.bottom.min(rhs.bottom);
        if left <= right && top <= bottom {
            Some(Self::new(left, top, right, bottom))
        } else {
            None
        }
    }

    fn dist(&self, point: &Point) -> i32 {
        if point.x < self.left {
            if point.y < self.top {
                (*point - Point::new(self.left, self.top)).hypot() as i32
            } else if point.y > self.bottom {
                (*point - Point::new(self.left, self.bottom)).hypot() as i32
            } else {
                self.left - point.x
            }
        } else if point.x > self.right {
            if point.y < self.top {
                (*point - Point::new(self.right, self.top)).hypot() as i32
            } else if point.y > self.bottom {
                (*point - Point::new(self.right, self.bottom)).hypot() as i32
            } else {
                point.x - self.right
            }
        } else {
            if point.y < self.top {
                self.top - point.y
            } else if point.y > self.bottom {
                point.y - self.bottom
            } else {
                0
            }
        }
    }
}
// %=END CODE