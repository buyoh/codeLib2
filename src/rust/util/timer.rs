// %=BEGIN DOC
// %title
// 時間計測
//
// %overview
//
// %usage
// MaiTimer::new() で計測開始、
// millisecondsで計測開始時点からの時刻を取得。
//
// %require
// ```
// ```
// %verified
// yukicoder online judge
//
// %references
//
// %=END DOC
// %=BEGIN CODE

#[derive(Clone)]
struct MaiTimer(time::SystemTime);

impl MaiTimer {
    fn new() -> Self {
        Self(time::SystemTime::now())
    }

    fn milliseconds(&self) -> i64 {
        match time::SystemTime::now().duration_since(self.0) {
            Ok(z) => z.as_millis() as i64,
            Err(e) => {
                eprintln!("MaiTimer: err: {:?}", e);
                0
            }
        }
    }
}
// %=END CODE
