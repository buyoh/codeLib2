// %title
// GCD,LCM
// 
// %overview
// 最大公約数と最小公倍数
//
// %usage
// テンプレート型で実装
//
// %verified
// 
// %references

template<typename T> inline T gcd(T a, T b) { while(b){T t=a%b;a=b;b=t;}return a; }
template<typename T> inline T lcm(T a, T b) { return a*b/gcd(a,b); }
