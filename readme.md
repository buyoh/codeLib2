
# codeLib2

## What's this?
- 手法やアルゴリズムをまとめたチートシートです。

## codeLibとの違い
- 説明文などのフォーマットを統一
- 探しやすいようにファイルを細分化

## 使う
プロトタイププログラムに組み込むことを想定しています。大半のコードで入力値チェックを行いません。

## ライブラリとは
一般に『ライブラリ』とは、整形・整理した1つまたは複数のクラス・構造体・関数・メソッドを指します

## 注意
- 別の構造体を前提とするコードがあります(example: `maxflow`は`Flow`が必要．)

```
#include "bits/stdc++.h"
using namespace std;
using ll = int64_t; // long long
```

## document

- sinatraによる雑な整理ツールを作っています
- http://codelib.ukibune.blue に同様のものがあります

### setup

- カレントディレクトリを `tools` に移動してから、
- `bundle install`

失敗する場合は、おそらく必要なアプリケーションがインストールされていない。
ubuntuの場合、
- `sudo apt install ruby ruby-dev gem sqlite3 libsqlite3-dev`

### 実行

- カレントディレクトリを `tools` に移動してから、
- `bundle exec ruby server.rb`

## 開発

### シーケンス

現状は最低限の規定のみ。

- masterからブランチを作成
  - test, src ならブランチ名は`src-lang-algorithmNameHere`
  - tools ならブランチ名は`tools-hoge`
- 実装する
- pull requestする
- masterにsquashする

### テンプレート

空行なしで行頭にコメント行文字を入れると、メタ情報として認識されます。

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
// %words
// キーワード
// %require
// 必要な別のスニペット
// %tags
// nolint, (wip:未実装), (unverified:未実装)
```

### test

```
ruby tools/autotest.rb
```
