// %=BEGIN DOC
// %title
// XorShift による 一様分布乱数
//
// %overview
// 乱数を生成する。
// C++STL風に書いてしまったが、広く使われている rand Crate とはインターフェースが異なり、
// DistoとGeneratorの依存関係が逆。早いうちに書き直したい…
//
// %usage
//
// %require
// ```
// ```
// %verified
//
// %references
// https://ja.wikipedia.org/wiki/Xorshift
// https://qiita.com/hatoo@github/items/652b81e8e83b0680bc0a
// %=END DOC
// %=BEGIN CODE
struct IntegerUniformDistoribution {
    end: u64,
    lim: u64,
}
impl IntegerUniformDistoribution {
    #[allow(dead_code)]
    // [0, high)
    fn new(end: u64) -> Self {
        assert!(end != 0);
        Self {
            end,
            lim: u64::MAX / (end - 1) * (end - 1),
        }
    }
    #[inline]
    #[allow(dead_code)]
    fn f(&self, rng: &mut Xorshift) -> u64 {
        loop {
            let r = rng.next();
            if r <= self.lim {
                return r % self.end;
            }
        }
    }
}

struct IntegerUniformDistoributionI32 {
    begin: i32,
    du64: IntegerUniformDistoribution,
}
impl IntegerUniformDistoributionI32 {
    #[allow(dead_code)]
    // [0, high)
    fn new(end: i32) -> Self {
        Self::new2(0, end)
    }
    // [begin, high)
    fn new2(begin: i32, end: i32) -> Self {
        assert!(begin < end);
        Self {
            begin,
            du64: IntegerUniformDistoribution::new((end - begin) as u64),
        }
    }
    #[inline]
    #[allow(dead_code)]
    fn f(&self, rng: &mut Xorshift) -> i32 {
        self.du64.f(rng) as i32 + self.begin
    }
}

#[allow(dead_code)]
struct Xorshift {
    seed: u64,
}
impl Xorshift {
    #[allow(dead_code)]
    pub fn new_with_seed(seed: u64) -> Self {
        Self { seed: seed } // 88172645463325252
    }
    #[inline]
    #[allow(dead_code)]
    pub fn next(&mut self) -> u64 {
        self.seed = self.seed ^ (self.seed << 13);
        self.seed = self.seed ^ (self.seed >> 7);
        self.seed = self.seed ^ (self.seed << 17);
        self.seed
    }
    #[inline]
    #[allow(dead_code)]
    // [0.0,1.0]
    // https://qiita.com/hatoo@github/items/652b81e8e83b0680bc0a
    pub fn randf(&mut self) -> f64 {
        use std::mem;
        const UPPER_MASK: u64 = 0x3FF0000000000000;
        const LOWER_MASK: u64 = 0xFFFFFFFFFFFFF;
        let tmp = UPPER_MASK | (self.next() & LOWER_MASK);
        let result: f64 = unsafe { mem::transmute(tmp) };
        result - 1.0
    }
}

#[test]
fn test_generate_randomized_value() {
    let mut rng = Xorshift::new_with_seed(88172645463325252);
    let u = IntegerUniformDistoributionI32::new2(-500, 500);
    let mut bucket = [0; 1000];
    for _ in 0..(2000 * 1000) {
        let v = u.f(&mut rng);
        assert!(-500 <= v && v < 500, "{} in [-500, 500)", v);
        bucket[(v + 500) as usize] += 1;
    }
    for (i, cnt) in bucket.iter().enumerate() {
        assert!(1800 < *cnt && *cnt < 2200, "{} ~ 2000, idx={}", *cnt, i);
    }
}
// %=END CODE
