// %title
// 幾何(点と線と円)
// 
// %overview
// 点と線と円に関するクラスとメソッド．
// 距離と交点のみ．
//
// %usage
//
// %verified
// 目視
// %references
// http://shogo82148.github.io/homepage/memo/geometry/circle-cross.html
// %words
// kika,vector
// %require
// 
// 

class Point{
    /**
     * 
     * @param {number} x 
     * @param {number} y 
     */
    constructor(x, y){
        this.x = x;
        this.y = y;
    }

    /**
     * 
     * @param {number} t
     */
    multiply(t){
        return new Point(t*this.x, t*this.y);
    }
    /**
     * 
     * @param {Point} p 
     */
    plus(p){
        return new Point(this.x+p.x, this.y+p.y);
    }
    /**
     * 
     * @param {Point} p 
     */
    minus(p){
        return new Point(this.x-p.x, this.y-p.y);
    }
    /**
     * 
     * @param {Point} p 
     */
    distance(p){
        return Math.hypot(this.x-p.x, this.y-p.y);
    }
    /**
     * x*x+y*y
     */
    square(){
        return this.x*this.x+this.y*this.y;
    }
    /**
     * 
     * @param {Point} p 
     */
    dot(p){
        return this.x*p.x + this.y*p.y;
    }
    /**
     * 
     * @param {Point} p 
     */
    cross(p){
        return this.x*p.y - p.x*this.y; 
    }
}


class Circle{
    /**
     * 
     * @param {Point} p 中心点
     * @param {number} rad 半径
     */
    constructor(p, rad){
        this.p = p;
        this.rad = rad;
    }
    
    /**
     * 直線と円の交点
     * 交点が存在しない場合，[null,null]
     * @param {Line} li 
     * @returns {[Point,Point]}
     */
    intersectionLine(li){
        return li.intersectionCircle(this);
    }

    /**
     * 円と円の交点
     * 交点が存在しない場合，[null,null]
     * @param {Circle} ci 
     * @returns {[Point,Point]}
     */
    intersectionCircle(ci){
        const a = (this.p.square()-ci.p.square()-this.rad*this.rad+ci.rad*ci.rad)/2;
        return Line.equation(this.p.x-ci.p.x,this.p.y-ci.p.y, -a).intersectionCircle(this);
    }
}


class Line{
    /**
     * pos+t*vel を満たす直線．
     * |vel| < EPS の時，0除算が発生することがある．
     * @param {Point} pos
     * @param {Point} vel
     */
    constructor(pos, vel){
        this.pos = pos;
        this.vel = vel;
    }
    /**
     * p1,p2を通る直線を生成
     * p1.distance(p2) < EPS の時，0除算が発生することがある．
     * @param {Point} p1 
     * @param {Point} p2 
     */
    static path(p1, p2){
        const lp = p1, lv = p2.plus(p1.multiply(-1));
        return new Line(lp, lv);
    }

    /**
     * ax + by + c = 0 を満たす直線を生成
     * |a| < EPS, |b| < EPS の時，0除算が発生する．
     * @param {number} a 
     * @param {number} b 
     * @param {number} c 
     */
    static equation(a, b, c){
        if (Math.abs(a) < Math.abs(b))
            return new Line(new Point(0, -c/b), new Point(b, -a));
        else
            return new Line(new Point(-c/a, 0), new Point(b, -a));
    }

    /**
     * t をパラメータとする直線上の点 pos+t*vel を求める．
     * @param {number} t 
     */
    point(t){
        return this.pos.plus(this.vel.multiply(t));
    }

    /**
     * 点と直線の距離
     * 直線上で点に最寄りの座標
     * @param {Point} p
     * @returns {{distance: number, nearest: Point}}
     */
    distancePoint(p){
        const lp = this.pos, lv = this.vel;

        const den = lv.square();

        return {
            distance: Math.abs(lv.cross(p.minus(lp)))/Math.sqrt(den),
            nearest: this.point(lv.dot(p.minus(lp))/den)
        }
    }

    /**
     * 直線と円の交点
     * 交点が存在しない場合，[null,null]
     * @param {Circle} ci 
     * @returns {[Point,Point]}
     */
    intersectionCircle(ci){
        const lp = this.pos, lv = this.vel;

        const d = lv.x*(ci.p.y-lp.y)-lv.y*(ci.p.x-lp.x);
        const den = lv.x*lv.x+lv.y*lv.y;
        const det = (den)*ci.rad*ci.rad-d*d;
        if (det < 0) return [null, null];

        const num = lv.x*(ci.p.x-lp.x)+lv.y*(ci.p.y-lp.y);
        const detsq = Math.sqrt(det);
        return [this.point((num+detsq)/den), this.point((num-detsq)/den)];
    }

    /**
     * 直線と直線の交点
     * 存在しない場合はnull
     * @param {Line} li
     * @returns {Point} 
     */
    intersectionLine(li){
        const num = -this.vel.cross(li.pos.minus(this.pos));
        const den = this.vel.cross(li.vel);
        if (den == 0) return null;
        return li.point(num/den);
    }

}
