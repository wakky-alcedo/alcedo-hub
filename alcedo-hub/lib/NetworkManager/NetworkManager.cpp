/**
 * @file NetworkManager.cpp
 * @brief Wi-FiおよびMQTT通信管理の実装
 * @author Alcedo Pebble Project
 */

#include "NetworkManager.h"

// PubSubClientのコールバックは静的関数である必要があるため、
// インスタンスへのポインタを保持してリレーする
static NetworkManager* _instance = nullptr;

/**
 * @brief MQTT受信時の静的トランポリン関数
 * @param topic 受信トピック
 * @param payload 受信ペイロード
 * @param length ペイロード長
 */
static void _globalMqttCallback(char* topic, byte* payload, unsigned int length) {
    if (_instance) {
        _instance->_handleInternalMqttMessage(topic, payload, length);
    }
}

/**
 * @brief コンストラクタ
 * Wi-FiクライアントをMQTTクライアントに紐付ける
 */
NetworkManager::NetworkManager() : _mqttClient(_espClient) {
    _instance = this; // 静的ポインタに自身を登録
}

/**
 * @brief セットアップ
 * @param ssid Wi-Fi SSID
 * @param pass Wi-Fi Password
 * @param mqttServer MQTTブローカーのIPアドレス
 */
void NetworkManager::setup(const char* ssid, const char* pass, const char* mqttServer) {
    _ssid = ssid;
    _pass = pass;
    _mqttServer = mqttServer;

    // MQTT設定
    _mqttClient.setServer(_mqttServer, 1883);
    _mqttClient.setCallback(_globalMqttCallback);
    
    // バッファサイズ拡張 (長いJSON対策)
    _mqttClient.setBufferSize(1024);

    // 初回接続
    _connectWifi();
}

/**
 * @brief ループ処理 (main loopから呼ばれる)
 * 接続維持とポーリングを行う
 */
void NetworkManager::loop() {
    // Wi-Fi接続確認
    if (WiFi.status() != WL_CONNECTED) {
        _connectWifi();
    }

    // MQTT接続確認
    if (!_mqttClient.connected()) {
        _reconnectMqtt();
    }

    // MQTTポーリング
    _mqttClient.loop();
}

/**
 * @brief MQTTメッセージ送信
 */
void NetworkManager::publish(const char* topic, const char* payload) {
    if (_mqttClient.connected()) {
        _mqttClient.publish(topic, payload);
    }
}

/**
 * @brief 指定トピックを購読する
 * (AlcedoHub::setup内で呼び出す想定)
 */
void NetworkManager::subscribe(const char* topic) {
    // 再接続時に再購読できるよう、リスト等で管理するのが理想だが
    // 今回は簡易的に直近のものを保持する設計、または都度呼ぶ運用とする
    if (_mqttClient.connected()) {
        _mqttClient.subscribe(topic);
    }
}

/**
 * @brief 受信コールバック関数の登録
 */
void NetworkManager::setCallback(MqttCallback cb) {
    _onMessage = cb;
}

/**
 * @brief 内部的なWi-Fi接続処理
 */
void NetworkManager::_connectWifi() {
    delay(10);
    Serial.printf("[Network] Connecting to %s ", _ssid);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _pass);

    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 20) {
        delay(500);
        Serial.print(".");
        retry++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println(" Connected.");
        Serial.print("[Network] IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println(" Failed. Retry in loop.");
    }
}

/**
 * @brief 内部的なMQTT再接続処理
 */
void NetworkManager::_reconnectMqtt() {
    // ノンブロッキングにするため、前回試行から時間が経っていなければスキップ
    static unsigned long lastAttempt = 0;
    unsigned long now = millis();
    if (now - lastAttempt < 5000) return;
    lastAttempt = now;

    Serial.print("[Network] Attempting MQTT connection... ");
    
    // ランダムなクライアントIDを生成 (重複排除)
    String clientId = "ESP32-Alcedo-" + String(random(0xffff), HEX);
    
    if (_mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
        // note: AlcedoHub側で管理しているトピックをここで再購読する仕組みが必要
        // 簡易実装として、hub側で定期的にsubscribeを呼ぶか、固定トピックならここでsubscribeする
        _mqttClient.subscribe("home/living/ir/emit"); // 暫定: 必須トピックはここで復帰
    } else {
        Serial.print("failed, rc=");
        Serial.print(_mqttClient.state());
        Serial.println(" try again in 5s");
    }
}

/**
 * @brief 静的関数から呼び出される内部ハンドラ
 * byte配列をStringに変換し、登録されたコールバックを呼ぶ
 * @param topic 受信トピック
 * @param payload 受信ペイロード
 * @param length ペイロード長
 */
void NetworkManager::_handleInternalMqttMessage(char* topic, byte* payload, unsigned int length) {
    // ペイロードをStringに変換
    String payloadStr;
    for (unsigned int i = 0; i < length; i++) {
        payloadStr += (char)payload[i];
    }
    
    // 登録されたコールバックがあれば実行
    if (_onMessage) {
        _onMessage(String(topic), payloadStr);
    }
}