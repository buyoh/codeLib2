// %=BEGIN DOC
// %title
// rand(AtCoder)
//
// %overview
// 競技用。手軽に乱数を取りたい時。
// 競技は大抵シングルスレッドなので unsafe。
// crate randがインストールされている環境のみ。
// codingameでは使える。
//
// %usage
// シャッフルは `vec.shuffle(get_global_rng());`
//
// %require
// ```
// ```
// %verified
//
// %references
// %=END DOC
// %=BEGIN CODE

extern crate rand;
use rand::{Rng, SeedableRng};
static mut RNG: Option<Box<dyn rand::RngCore>> = None;

fn get_global_rng() -> &'static mut dyn rand::RngCore {
    unsafe {
        if let Some(rng) = &mut RNG {
            rng
        } else {
            RNG = Some(Box::new(rand::rngs::StdRng::seed_from_u64(8901016 as u64)));
            get_global_rng()
        }
    }
}

fn rnd<
    T: rand::distributions::uniform::SampleRange<U>,
    U: rand::distributions::uniform::SampleUniform,
>(
    range: T,
) -> U {
    unsafe { get_global_rng().gen_range(range) }
}

// %=END CODE
