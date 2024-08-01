# 第02回レポート

名前：大坪雄也 \
学籍番号: 21TI019 \
提出日：2024/08/01

[ファイルの説明]
<rootディレクトリ>
- src/: ソースファイルが含まれているディレクトリ. コンパイル時には生成されたオブジェクトファイルもここにできる.
- include/: ヘッダーファイルが含まれているディレクトリ.
- Makefile: ゲームプログラムの生成、削除を行うMakefile.
- readme.txt: このファイル.
- .gitignore: gitignoreファイル.

＜srcディレクトリ＞
- `main.cpp`: main関数と初期化などが書かれたファイル.
- `display.cpp`: windowへの描画についての処理が書かれたファイル. 
- `keyboard.cpp`: keyが入力された時のイベントの処理が書かれたファイル.
- `lightInit.cpp`: 光源の初期設定が書かれたファイル. 
- `map.cpp`: mapの描画について書かれたファイル.
- `reshape.cpp`: windowサイズ変更の際の処理が書かれたファイル.
- `specialKey.cpp`: 

- server.c: チャットプログラムのサーバーサイド
- utils.c: client.c, server.cの両方で使用する関数をまとめたファイル

＜ヘッダーファイル＞
- report01.h: プログラム内で使用する関数のプロトタイプやマクロ, 構造体などを定義している

＜その他のファイル＞
- Makefile: client, serverを作成するためのMakefile
- readme.md: このファイル

[実行プログラムの説明]
- client: チャットプログラムのクライアントサイド
- server: チャットプログラムのサーバーサイド

[コンパイル及び実行方法]

＜コンパイル方法＞
すでにclientとserverの両方が存在している場合は`make re`.
少なくともどちらかが存在していない時は, `make`又は`make all`又は`make server(client)`又は`make re`.

＜実行方法＞

`./server <port番号>`を行った後に, `./client localhost <port番号>`で実行できる.
文字列を入力してEnter keyを入力することで, 入力した文字列を相手に送信することができる.
`quit`を入力(もしくはCtrl+dを押下)し, 相手がプロンプトの指示に従ってEnter keyを入力するとプログラムが正常に終了する.

＜実行例＞
```txt
%> ./server 3000
Connection is established.
[tips] you can set the new name by "set-name:<new_name>"
> hello
> from [visiter]: yeah
> from [taro]: my name is taro
> set-name:taro2
now your name is "taro2"
> my name is taro too
> nice to meet you
> こんにちは
> from [taro]: こんにちは
> from [taro]: quit
please press enter
%>
```
```txt
%>./client localhost 3000
[tips] you can set the new name by "set-name:<new_name>"
> from [visiter]: hello
> yeah
> set-name:taro
now your name is "taro"
> my name is taro
> from [taro2]: my name is taro too
> from [taro2]: nice to meet you
> from [taro2]: こんにちは
> こんにちは
> quit
from [taro2]:
> %
%>
```
[工夫点]
- clientとserverで処理が異なるのはソケットを接続するなどの通信の準備段階だけであったので, 実際の送受信の処理を完全に共通化させたこと.

- 受信するとき, `from [user_name]`がメッセージの頭につくようにし, 誰から送信されたのかがわかるようにした.
- `set-name:<new_name>`で表示名を変更できるようにした.

[考察]
- Makefileにて, もともとオブジェクトファイルを生成してから実行ファイルを作成する本来のMakefileの使用法に即して書いていたが, utils.cがserver, clientの両方の材料になっていたためにmakeでのコンパイルがうまくいかなかった. Makefileにて複数の生成物を作るときに, 共通のター ゲットが存在すると一工夫必要なことが分かった.
- 送受信どちらも別のスレッドを用いており送受信の処理でどちらも待ち状態になるため, `quit`でどちらも一気に終了させることができなかった. 他スレッドからスレッドを終了させる方法があればもっときれいなプログラムになると思った.

[参考資料]
無し%