# pets-r2

# 必要ライブラリの導入

```
cd lib
git clone git@github.com:mathertel/RotaryEncoder.git

git clone git@github.com:tzapu/WiFiManager.git
git clone git@github.com:me-no-dev/AsyncTCP.git
git clone git@github.com:me-no-dev/ESPAsyncWebServer.git
git clone git@github.com:bblanchon/ArduinoJson.git -b v5.13.5

```

# 動かし方
1. 本体のバッテリー充電をする
   1. 充電用のACアダプターがあるので それを利用する 
2. マイコン用バッテリーの充電をする
   1. 白いスマホバッテリー を USB Micro Bケーブルで充電
3. 本体の電源を入れる
4. マイコンの電源を入れる
   1. 白いスマホバッテリー と USBを白いケーブルで繋ぐ
5. PETS の 電源をいれて かごに設置する
6. PETS の 回路設定をして 開始ぼたんを押す
7. 開始と同時に 本体も動き始める
8. バッテリー切れ等で　動作がオカシイ場合 本体の電源を切って バッテリー交換後に手順１から
   

# 備考
1. 前後, 左右旋回 は 乗っている人の体重によって距離がかわる
2. 本体バッテリーが切れてくると 片側だけ回転するようになるので充電が必要
3. PETS 自身の動作方向が回転方向が違う場合があるので 机の上で動かす等して確認が必要
   1. 端子の接触不良
   2. 乾電池の残陽不足


   
