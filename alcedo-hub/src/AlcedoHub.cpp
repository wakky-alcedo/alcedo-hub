#include "AlcedoHub.h"
#include "Config.h"

// コンストラクタで各モジュールにピン番号等を渡す
AlcedoHub::AlcedoHub() 
    : _ir(PIN_IR_SEND, PIN_IR_RECV) 
{
}

void AlcedoHub::setup() {
    Serial.println("[AlcedoHub] Init...");

    // 1. 赤外線の初期化
    _ir.setup();

    // 2. ネットワークの初期化 (コールバック登録)
    _network.setCallback([this](String t, String p){ 
        this->_onMqttMessage(t, p); 
    });
    _network.setup(WIFI_SSID, WIFI_PASS, MQTT_SERVER);

    // 3. GUIの初期化 (コールバック登録)
    // _gui.setCallback([this](int id, int val){
    //     this->_onGuiEvent(id, val);
    // });
    // _gui.setup();

    Serial.println("[AlcedoHub] Setup done.");
}

void AlcedoHub::loop() {
    _network.loop();
    _ir.loop();
    // _gui.loop(); // LVGLのタイマー処理など

    // IR受信チェック (学習モード)
    if (_ir.hasReceivedData()) {
        _network.publish("home/living/ir/received", _ir.getLastReceivedHex().c_str());
    }
}

// MQTT受信時 -> IR送信 or 画面更新
void AlcedoHub::_onMqttMessage(String topic, String payload) {
    // 例: IR送信命令ならIrManagerへ
    // 例: 画面更新命令ならGuiManagerへ
}

// GUI操作時 -> MQTT送信
void AlcedoHub::_onGuiEvent(int eventId, int value) {
    // 例: エアコンボタンが押された
    _network.publish("home/living/ac/control", String(value).c_str());
}