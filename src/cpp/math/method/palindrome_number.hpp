// %=BEGIN
// %title
// 回文数の数え上げ
//
// %overview
// 1以上val以下の整数で回文数が何個あるか数える．
// かなり甘い実装
// [TODO] 文字列に対しても実装する．
//
// %usage
// ll kaibunsu(ll val)
// %require
// ```
using ll = long long;
// ```
//
// %verified
// %references
// %=END

// 1以上10^i-1以下の整数
ll _kaibunsu_dec(int keta) {
  if (keta <= 1)
    return 9;
  ll r = 1;
  for (int i = 0; i < (keta + 1) / 2; ++i)
    r *= 10;
  return r - (r / 10) + _kaibunsu_dec(--keta);
}
// dfs列挙
ll _kaibunsu_dfs(ll low, ll high, ll kcl, ll kch, ll num = 0) {
  if (kcl > kch) {
    return (low <= num && num <= high);
  }
  ll result = 0;
  for (ll k = (kcl == 1); k <= 9; ++k) {
    result += _kaibunsu_dfs(low, high, kcl * 10, kch / 10, num + kcl * k + kch * k * (kcl != kch));
  }
  return result;
}

// 1以上val以下の整数で回文数が何個あるか
ll kaibunsu(ll val) {
  if (val <= 9)
    return val;
  ll dec = 1;
  int keta = 0;
  while (val >= dec) {
    dec *= 10;
    ++keta;
  }
  dec /= 10;

  return _kaibunsu_dfs(dec, val, 1, dec) + _kaibunsu_dec(--keta);
}

// 以下愚直．デバッグ用
// --------------------------------------------------------------------------------
//
bool iskaibun(int val) {
  if (val <= 9)
    return 1;
  int t, u;
  t = 0;
  u = val;
  while (u) {
    t = (t * 10) + (u % 10);
    u /= 10;
  }
  while (t) {
    if (val % 10 != t % 10)
      return false;
    val /= 10;
    t /= 10;
  }
  return true;
}
int hetakuso(int val) {
  int count = 0;
  while (val) {
    if (iskaibun(val--))
      ++count;
  }
  return count;
}