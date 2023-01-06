// %=BEGIN DOC
// %title
// rand
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

static mut RNG: Option<rand::rngs::StdRng> = None;

fn get_global_rng() -> &'static mut rand::rngs::StdRng {
    unsafe {
        if let Some(rng) = &mut RNG {
            rng
        } else {
            RNG = Some(rand::rngs::StdRng::seed_from_u64(8901016 as u64));
            get_global_rng()
        }
    }
}

fn rnd<T: rand::distributions::uniform::SampleUniform>(low: T, high: T) -> T {
    unsafe { rand::distributions::Uniform::new(low, high).sample(get_global_rng()) }
}

// %=END CODE
