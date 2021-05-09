// %=BEGIN DOC
// %title
// 座標
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
#[derive(Clone, Copy)]
struct Pt {
    y: i32,
    x: i32,
}

impl Pt {
    fn dist(&self) -> i32 {
        self.y.abs() + self.x.abs()
    }
}

impl std::ops::Add<Pt> for Pt {
    type Output = Pt;
    fn add(self, rhs: Pt) -> Pt {
        Pt {
            y: self.y + rhs.y,
            x: self.x + rhs.x,
        }
    }
}
impl std::ops::AddAssign<Pt> for Pt {
    fn add_assign(&mut self, rhs: Pt) {
        self.y += rhs.y;
        self.x += rhs.x;
    }
}
impl std::ops::Sub<Pt> for Pt {
    type Output = Pt;
    fn sub(self, rhs: Pt) -> Pt {
        Pt {
            y: self.y - rhs.y,
            x: self.x - rhs.x,
        }
    }
}
// %=END CODE
