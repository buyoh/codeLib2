// %=BEGIN DOC
// %title
// グラフ（隣接頂点）
//
// %overview
// 無向・有向どちらも表現可能。
// 辺に情報を持つことができる。
// ただし、無向辺を双方向の有向辺と表現するため、
// 無向辺に情報をもたせる場合は倍のリソースを消費する
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
struct Graph<T: Clone = ()>(Vec<Vec<(usize, T)>>);

impl<T: Clone> Graph<T> {
    fn to<'a>(&self, i: usize) -> &Vec<(usize, T)> {
        &(self.0)[i]
    }
}

struct GraphBuilder<T: Clone = ()>(Graph<T>);

impl<T: Clone> GraphBuilder<T> {
    fn new(n: usize) -> Self {
        Self(Graph::<T>(vec![Vec::<(usize, T)>::new(); n]))
    }
    fn connect(&mut self, u: usize, v: usize, item: T) {
        (self.0 .0)[u].push((v, item.clone()));
        (self.0 .0)[v].push((u, item));
    }
    fn connect_d(&mut self, src: usize, dst: usize, item: T) {
        (self.0 .0)[src].push((dst, item));
    }
    fn build(self) -> Graph<T> {
        self.0
    }
}
// %=END CODE
