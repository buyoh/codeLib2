# codeLib2

## What's this?

- 手法やアルゴリズムをまとめたチートシートです。
- ドキュメントはこちら https://buyoh.github.io/codeLib2

## 注意

- プロトタイププログラムに組み込むことを想定しています。大半のコードで入力値チェックを行いません。
- 別の構造体を前提とするコードがあります(example: `maxflow`は`Flow`が必要．)

## ドキュメント(codeLib2-htmldoc)

https://github.com/buyoh/codeLib2-htmldoc

- この repository を検索しやすくするために作ったものです。
- https://buyoh.github.io/codeLib2 に同様のものがあります
- redux, webpack の勉強を兼ねて作成した雑な整理ツールです

## スクリプト

いくつかのツールを含んでいます。

### autotest

テストを実行します。`-j` は、並列にテストを行うジョブの数を指定します。

```
ruby tools/autotest.rb -j 4
```

特定の正規表現にマッチするファイルのみテストを行うこともできます。

```
ruby tools/autotest.rb --filter ".*graph.*"
```

### buildtest

各ファイルがコンパイル可能かどうか検査します。
`autotest` と同様に `-j` `--filter` オプションが利用可能です。

```
ruby tools/buildtest.rb -j 4
```

### include-guard-appender

新しくhppを追加した時にインクルードガードを勝手に挿入してもらうツールです。

```
ruby tools/include-guard-appender.rb
```

### refactor

整形出来ているかどうかチェックします。いわゆる `Beautify` です。

```
ruby tools/refactor.rb
```

勝手に修正しても良い場合は、`--inplace`を追加します。

```
ruby tools/refactor.rb --inplace
```

## 開発

### テンプレート

```cpp
// %=BEGIN DOC
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
// %require
// 必要な別のスニペットや記述
// ```
#include <vector>
using namespace std;
#include "src/cpp/graph/datastructure/graph.cpp"
// ```
//
// %verified
// 検証されたかどうか．
// 
// %references
// 参考資料など．
// 
// %words
// キーワード
// 
// %tags
// nolint, (wip: 未実装), (unverified: 未検証), (deprecated: 非推奨)
// %=END DOC

// %=BEGIN CODE
template<typename T, typename U>
U plus(const T& t1, const T& t2) {
  return t1 + t2;
}
// %=END CODE
```

## なぜcodelib '2' なのか

1を書き直したから
