
# codeLib2

## What's this?
- 手法やアルゴリズムをまとめたチートシートです．
- Repository codeLib からファイルを再構築しました．

## codeLibとの違い
- 説明文などのフォーマットを統一．
- 探しやすいようにファイルを細分化．

## 使う
お遊びのプログラムに組み込むことを想定しています．大半のコードで入力値チェックを行いません．


## ライブラリじゃない
競技プログラミング界隈での『ライブラリ』とは，整形・整理した1つまたは複数のクラス・構造体・関数・メソッドを指しているような気がします．

## 注意
- 別の構造体を前提とするコードがあります(example: `maxflow`は`Flow`が必要．)．

```
#include "bits/stdc++.h"
using namespace std;
typedef long long int ll;
```

## テンプレート
```cpp
// %title
// タイトル
// 
// %overview
// ファイルの概要．
//
// %usage
// int func(int x);
// x    : 引数についての説明
// @ret : 返り値についての説明
//
// %verified
// 検証されたかどうか．
// 
// %references
// 参考資料など．
```

