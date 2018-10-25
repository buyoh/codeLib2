// %title
// さいころの回転
// 
// %overview
// 無理やり感のある実装
// 
// さいころのローカル座標軸はvecsで取得出来る．
// これを使って，1はどの方向を向いているか？等の情報が得られる．
// 
// vecsはフィールドですが，readonlyとして扱ってね．
//
// %usage
//
// %verified
// todo
// %references
// 
// %words
// dice,sai
// %require
// 
// 


class Dice{

    constructor(){
        this.vecs = [
            [1, 0, 0],
            [0, 1, 0],
            [0, 0, 1]
        ];
    }

    /**
     * @param {1|-1} sign 1 or -1
     */
    rotateZ(sign = 1){
        sign = sign < 0 ? -1 : 1;
        for (let i=0; i<3; ++i)
            this.vecs[i] = [sign*this.vecs[i][1], -sign*this.vecs[i][0], this.vecs[i][2]];
    }

    /**
     * @param {1|-1} sign 1 or -1
     */
    rotateY(sign = 1){
        sign = sign < 0 ? -1 : 1;
        for (let i=0; i<3; ++i)
            this.vecs[i] = [sign*this.vecs[i][2], this.vecs[i][1], -sign*this.vecs[i][0]];
    }

    /**
     * @param {1|-1} sign 1 or -1
     */
    rotateX(sign = 1){
        sign = sign < 0 ? -1 : 1;
        for (let i=0; i<3; ++i)
            this.vecs[i] = [this.vecs[i][0], sign*this.vecs[i][2], -sign*this.vecs[i][1]];
    }
}
