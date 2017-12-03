// %title
// 素因数分解
// 
// %overview
// 素因数分解やるだけ．
// 素数情報を必要としない．
// 計算量が落としきれていないかもしれない．[TODO]
//
// %usage
// void prime_division(ll num, ARRAY& result)
// result : 
// num    :
// resultの型はvector<ll>かmap<ll,ll>
//
// %verified
// 
// %references


template<typename ARRAY>
void prime_division(ARRAY& result, ll num) {
    ll _num = num;
    while (num % 2 == 0) {
        num /= 2;
        ++result[2];
    }
    for (ll i = 3; 1 < num && i*i <= _num; i += 2) {
        while (num % i == 0) {
            num /= i;
            ++result[i];
        }
    }
}
