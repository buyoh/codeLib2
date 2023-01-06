// %=BEGIN DOC
// %title
// ダイクストラ / Dijkstra
//
// %overview
//
// %usage
//
// %require
// ```
// ```
// %verified
// rand
//
// %references
//
// %=END DOC
// %=BEGIN CODE
fn fill_dijkstra<Func: FnMut(usize, usize) -> bool>(
    mut dist: Fi<usize>,
    mut is_available: Func,
) -> Fi<usize> {
    let mut que = VecDeque::<(usize, usize)>::new();
    for y in 0..(dist.height) {
        for x in 0..(dist.width) {
            if dist[(y, x)] == 0 {
                que.push_back((y, x));
            }
        }
    }

    while let Some(yx1) = que.pop_front() {
        let c1 = dist[yx1];
        for v in K_ADJACENTS {
            let yx2 = ((yx1.0 as i32 + v.0) as usize, (yx1.1 as i32 + v.1) as usize);
            let c2 = if let Some(c2) = dist.get_mut(yx2) {
                c2
            } else {
                continue;
            };
            if *c2 <= c1 + 1 || !is_available(yx2.0, yx2.1) {
                continue;
            }
            *c2 = c1 + 1;
            que.push_back(yx2);
        }
    }
    dist
}

// %=END CODE
