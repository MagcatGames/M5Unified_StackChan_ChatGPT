# M5Unified_StackChan_ChatGPT

@robo8080さんのChatGPT API搭載AIｽﾀｯｸﾁｬﾝを一部改造しました。

アプリ等を使わずWiFi可能端末からブラウザ経由でWiFi接続の初期設定が出来ます。

### 使い方 ###

・まずはrobo8080さんのgithubから最新版のAIｽﾀｯｸﾁｬﾝをビルド出来るようにする。

・ここのソースから"StackChan_CaptivePortal.h"と"StackChan_CaptivePortal.cpp"を自身のsrcフォルダにコピーする。

・自身のM5Unified_StackChan_ChatGPT.inoを開き、"#include <StackChan_CaptivePortal.h>"を追加する。

・変数の宣言をしておく "StackChan_CaptivePortal sccp;"

・void setup() 関数の始まりに以下を追記する。（CaptivePortalモードの開始）
// ======== //
if(!sccp.HasKey()){
    M5.begin();
    M5.Lcd.setTextSize(2);
    M5.Lcd.println("We have started the Captive Portal mode. \n\nPlease connect to StackChanAP using WiFi.");
    sccp.Begin();
    return;
}
SSID = sccp.GetSSID().c_str();
PASSWORD = sccp.GetPassword().c_str();
// ======== //


・void setup() 関数の "M5.begin(cfg);" の次に以下を追記する。（設定失敗した時のリセット用）
// ======== //
if(M5.BtnA.isPressed()){
    M5.Lcd.setTextSize(2);
    M5.Lcd.println("Settings has been cleared. Please wait for 5 sec to restart.");
    sccp.Clear();
    delay(3000);
    ESP.restart();
    return;
}
// ======== //


・void setup() 関数の "M5.Lcd.print("Connecting");" の前に以下を追記する。（リセット方法の表示）
// ======== //
M5.Lcd.println("To clear Settings. hold A and restart.");
// ======== //


・void loop() 関数の始まりに以下を追記する。
// ======== //
if(!sccp.HasKey()){
    sccp.Update();
    delay(10);
    return;
}
// ======== //

以上で設定完了です。
上手くいかない場合は、ここの"M5Unified_StackChan_ChatGPT.ino"を参考にしてみて下さい。