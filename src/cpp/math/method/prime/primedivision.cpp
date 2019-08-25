// %title
// 素因数分解
//
// %overview
// 素因数分解やるだけ．
// 素数情報を必要としない．
// 計算量は O(HlogH),H=sqrt(N)
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

map<ll, int> primeDivision(ll _num) {
  map<ll, int> result;
  ll num = _num;
  while ((num & 1) == 0) {
    num /= 2;
    ++result[2];
  }
  for (ll i = 3; 1 < num && i <= _num; i += 2) {
    while (num / i * i == num) {
      num /= i;
      ++result[i];
    }
  }
  return result;
}
