## _CyberDevilHunter-Dracula_

# ゲーム説明
「サイバーデビルハンタードラクレア」はタイマンでボスラッシュを行っていく3Dアクションゲームです。<br>
プランナー3名、プログラマー3名、グラフィッカー名で製作されました。<br>
制作期間は2022/12/8～2023/4/19。ブラッシュアップ期間2023/4/30~<br>
プレイヤーはガード、ゲージを使った回復と強化を用いて敵の攻撃を捌きながらダメージを与えて倒します。<br>
3体のボスを倒すとゲームクリアです。<br>


# 開発環境
visualStudio 2022<br>
DxLib<br>
C++<br>
GitHub(https://github.com/Hibikinau/CyberDevilHunter-Dracula.git)<br>

# 操作方法
コントローラー(Xinput)対応<br>
Lスティック          : 移動<br>
Rスティック          : カメラ操作<br>
Rスティック押し込み  : ロックオン切り替え<br>
X                    : 弱攻撃<br>
Y                    : 強攻撃<br>
B　		     : 回避<br>
A　		     : 決定<br>
L１＋（A.X.Y.B）     : スキル3種<br>
R１　		     : ガード<br>
L１＋R１　	     : 覚醒<br>

# 実行方法
exeファイルを実行してください

ーーーーーーーーーーコードについてーーーーーーーーーー

appFrame 使用したフレームワークです

/ApplicationBase  gameMainクラスの基底クラスです
/CFile  CSVファイルの読み書きをするための関数まとめ
/modelManager  3Dモデルの制御をするための関数まとめ
/modeBase  モードの基底クラスです
/modeServer  モードを管理するためのクラスです
/ResourceServer  素材の読み込みとハンドルを制御するためのクラスです



CyberDevilHunter-Dracula ゲーム部分です

/char/bossKnight  騎士型ボスクラス
/char/bossLionman  ライオン型ボスクラス
/char/charBase  プレイヤークラス、ボスクラスの基底クラスです
/char/Lastboss  ラスボスのクラス
/char/player  プレイヤークラス

/mode/modeArtsGet  入れ替え技取得画面
/mode/modeBossChoose  ボス選択画面
/mode/modeEnd  エンディング画面
/mode/modeEquipmentSelection  入れ替え技選択画面
/mode/modeGame  インゲーム画面
/mode/modeGameOver  ゲームオーバー画面
/mode/modeMainMenu  メインメニュー画面
/mode/modeMenu  戦闘中に呼び出せるメニュー画面
/mode/modeResult  リザルト画面
/mode/modeSetting  設定画面
/mode/modeTitle  タイトル画面

/gameMain  ApplicationBaseクラスの関数を呼び出すなどしているクラスです

/save.csv  セーブデータ、各種設定などを入れています
