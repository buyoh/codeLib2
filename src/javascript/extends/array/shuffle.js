// %=BEGIN DOC
// %title
// shuffle (Fisher–Yates)
// 
// %overview
// 配列をshuffleする
// 配列はコピーされず書き換えられる
//
// %usage
// [1,2,3].shuffle()
//
// %verified
// 
// %references
// https://qiita.com/komaji504/items/62a0f8ea43053e90555a
// %words
// shuffle
// %require
// 
// %=END DOC
// %=BEGIN CODE


Array.prototype.shuffle = function () {
    for (let i = this.length - 1; i > 0; --i) {
        const r = Math.floor(Math.random() * (i + 1));
        const t = this[i];
        this[i] = this[r];
        this[r] = t;
    }
    return this;
};
// %=END CODE
