
#[derive(Debug, Clone, Copy)]
pub struct Vec2 {
    pub x: f64,
    pub y: f64,
}

impl Vec2 {
    pub fn new(x: f64, y: f64) -> Self {
        Vec2 { x, y }
    }

    pub fn zero() -> Self {
        Vec2 { x: 0.0, y: 0.0 }
    }

    pub fn dot(self, other: Self) -> f64 {
        self.x * other.x + self.y * other.y
    }

    pub fn cross(self, other: Self) -> f64 {
        self.x * other.y - self.y * other.x
    }
}

impl std::ops::Add for Vec2 {
    type Output = Self;
    fn add(self, other: Self) -> Self {
        Vec2::new(self.x + other.x, self.y + other.y)
    }
}

impl std::ops::Sub for Vec2 {
    type Output = Self;
    fn sub(self, other: Self) -> Self {
        Vec2::new(self.x - other.x, self.y - other.y)
    }
}

impl std::ops::Add<f64> for Vec2 {
    type Output = Self;
    fn add(self, other: f64) -> Self {
        Vec2::new(self.x + other, self.y + other)
    }
}

impl std::ops::Sub<f64> for Vec2 {
    type Output = Self;
    fn sub(self, other: f64) -> Self {
        Vec2::new(self.x - other, self.y - other)
    }
}

impl std::ops::Mul<f64> for Vec2 {
    type Output = Self;
    fn mul(self, other: f64) -> Self {
        Vec2::new(self.x * other, self.y * other)
    }
}

impl std::ops::Div<f64> for Vec2 {
    type Output = Self;
    fn div(self, other: f64) -> Self {
        Vec2::new(self.x / other, self.y / other)
    }
}

impl std::ops::AddAssign for Vec2 {
    fn add_assign(&mut self, other: Self) {
        self.x += other.x;
        self.y += other.y;
    }
}

impl std::ops::SubAssign for Vec2 {
    fn sub_assign(&mut self, other: Self) {
        self.x -= other.x;
        self.y -= other.y;
    }
}

impl std::ops::MulAssign<f64> for Vec2 {
    fn mul_assign(&mut self, other: f64) {
        self.x *= other;
        self.y *= other;
    }
}

impl std::ops::DivAssign<f64> for Vec2 {
    fn div_assign(&mut self, other: f64) {
        self.x /= other;
        self.y /= other;
    }
}

// Conversion implementations
impl From<(f64, f64)> for Vec2 {
    fn from(tuple: (f64, f64)) -> Self {
        Vec2::new(tuple.0, tuple.1)
    }
}

impl Into<(f64, f64)> for Vec2 {
    fn into(self) -> (f64, f64) {
        (self.x, self.y)
    }
}
