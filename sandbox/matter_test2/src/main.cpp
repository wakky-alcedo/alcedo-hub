#include <Arduino.h>
#include <esp_matter.h>
#include <esp_matter_controller.h> // Matterクライアント機能

using namespace esp_matter;
using namespace esp_matter::cluster;
using namespace esp_matter::endpoint;

node::config_t node_config;
node_t *node;

// スイッチのエンドポイント
endpoint_t *switch_endpoint;

// スイッチのクラスタ
on_off::client_t *switch_cluster;

// スイッチが押されたときの処理
void on_switch_press() {
    ESP_LOGI("Switch", "Switch pressed, sending On command");
    on_off::command::send_on(switch_cluster); // スイッチをON
}

// Matterのイベントハンドラ
void on_device_event(const chip::DeviceLayer::ChipDeviceEvent *event, intptr_t arg) {
    if (event->Type == chip::DeviceLayer::DeviceEventType::kInternetConnectivityChange) {
        ESP_LOGI("Matter", "Internet connectivity changed");
    }
}

void setup() {
    Serial.begin(115200);

    // Matterノードを作成
    node = node::create(&node_config, nullptr, nullptr);
    
    // スイッチのエンドポイントを作成
    switch_endpoint = endpoint::create(node, ENDPOINT_FLAG_NONE);
    
    // OnOff クライアントを作成
    switch_cluster = on_off::client::create(switch_endpoint);

    // Matterデバイスの起動
    esp_matter::start(on_device_event);
}

void loop() {
    // スイッチが押されたらONコマンドを送信
    if (digitalRead(0) == LOW) { // GPIO0が押されたら
        on_switch_press();
        delay(500); // デバウンス
    }
}
