// /**
//  * @file main.cpp
//  * @brief MQTT汎用学習リモコン (Alcedo IoT)
//  * * 役割:
//  * 1. MQTTで受信したJSONデータに基づいて赤外線を送信する (Executor)
//  * 2. 赤外線を受信したら生データをMQTTへ送信する (Learner)
//  */

// #include <Arduino.h>
// #include <WiFi.h>
// #include <PubSubClient.h>
// #include <ArduinoJson.h>
// #include <IRremoteESP8266.h>
// #include <IRsend.h>
// #include <IRrecv.h>
// #include <IRutils.h>

// // ==========================================
// // ★設定エリア (todo: 環境に合わせて変更してください)
// // ==========================================
// const char* WIFI_SSID = "YOUR_WIFI_SSID";         // Wi-FiのSSID
// const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";     // Wi-Fiのパスワード
// const char* MQTT_SERVER = "192.168.X.X";          // ラズパイのIPアドレス
// const int MQTT_PORT = 1883;

// // GPIO設定
// const uint16_t PIN_IR_SEND = 4;  // 赤外線送信LED (要トランジスタ回路)
// const uint16_t PIN_IR_RECV = 14; // 赤外線受信モジュール

// // MQTTトピック設定
// const char* TOPIC_IR_SEND_CMD = "home/living/ir/emit";   // 受信: IR発射命令
// const char* TOPIC_IR_RECV_DAT = "home/living/ir/received"; // 送信: IR受信データ
// const char* TOPIC_STATUS = "home/living/device/status";

// // ==========================================
// // グローバルオブジェクト
// // ==========================================
// WiFiClient espClient;
// PubSubClient client(espClient);
// IRsend irsend(PIN_IR_SEND);
// IRrecv irrecv(PIN_IR_RECV);
// decode_results results;
// const uint16_t kCaptureBufferSize = 1024; // 受信バッファサイズ

// // ==========================================
// // 関数宣言
// // ==========================================
// void setup_wifi();
// void reconnect();
// void mqtt_callback(char* topic, byte* payload, unsigned int length);
// void dump_ir_result(decode_results *results);

// // ==========================================
// // Main Setup
// // ==========================================
// void setup() {
//     Serial.begin(115200);
    
//     // IR初期化
//     irsend.begin();
//     irrecv.enableIRIn(); // 受信開始
//     Serial.println("Hardware initialized.");

//     // Wi-Fi & MQTT接続
//     setup_wifi();
//     client.setServer(MQTT_SERVER, MQTT_PORT);
//     client.setCallback(mqtt_callback);
// }

// // ==========================================
// // Main Loop
// // ==========================================
// void loop() {
//     // 1. MQTT接続維持
//     if (!client.connected()) {
//         reconnect();
//     }
//     client.loop();

//     // 2. 赤外線受信処理 (学習モード用)
//     if (irrecv.decode(&results)) {
//         // ノイズ対策: 短すぎる信号は無視
//         if (results.bits > 10) {
//             dump_ir_result(&results);
//         }
//         irrecv.resume(); // 次の受信へ
//     }
// }

// // ==========================================
// // 赤外線発射ロジック (ここがキモです)
// // ==========================================
// void mqtt_callback(char* topic, byte* payload, unsigned int length) {
//     Serial.print("Message arrived [");
//     Serial.print(topic);
//     Serial.print("] ");

//     // JSONパース
//     StaticJsonDocument<1024> doc;
//     DeserializationError error = deserializeJson(doc, payload, length);

//     if (error) {
//         Serial.print("deserializeJson() failed: ");
//         Serial.println(error.c_str());
//         return;
//     }

//     // JSONからデータを取得
//     const char* protocol = doc["protocol"]; // "NEC", "Sony", "Panasonic"...
    
//     // 送信処理分岐
//     // パターンA: 一般的なプロトコル (NECなど)
//     if (doc.containsKey("data") && doc.containsKey("bits")) {
//         uint64_t data = strtoull(doc["data"], NULL, 16); // 16進数文字列を数値化
//         int bits = doc["bits"];
//         int repeat = doc["repeat"] | 0;

//         Serial.printf("Sending %s: data=0x%llX, bits=%d\n", protocol, data, bits);

//         if (String(protocol) == "NEC") irsend.sendNEC(data, bits, repeat);
//         else if (String(protocol) == "Sony") irsend.sendSony(data, bits, repeat);
//         else if (String(protocol) == "Panasonic") irsend.sendPanasonic(data, bits, repeat);
//         // else if (String(protocol) == "AEHA") irsend.sendAEHA(data, bits, repeat);
//         // 必要に応じて他メーカーを追加
//     }
//     // パターンB: RAWデータ (特殊なエアコンなど)
//     else if (String(protocol) == "RAW" && doc.containsKey("raw_data")) {
//         JsonArray raw = doc["raw_data"];
//         uint16_t rawData[raw.size()];
//         for(int i=0; i<raw.size(); i++) {
//             rawData[i] = raw[i];
//         }
//         uint16_t freq = doc["freq"] | 38; // 周波数 (デフォルト38kHz)
        
//         Serial.printf("Sending RAW: len=%d, freq=%dkHz\n", raw.size(), freq);
//         irsend.sendRaw(rawData, raw.size(), freq);
//     }
// }

// // ==========================================
// // 赤外線受信データをMQTTへ送信
// // ==========================================
// void dump_ir_result(decode_results *results) {
//     StaticJsonDocument<512> doc;
    
//     doc["protocol"] = typeToString(results->decode_type);
//     doc["bits"] = results->bits;
//     doc["value"] = results->value;
    
//     // 16進数文字列に変換して格納
//     char hexBuf[20];
//     sprintf(hexBuf, "0x%llX", results->value);
//     doc["data_hex"] = hexBuf;

//     // MQTT送信
//     char buffer[512];
//     serializeJson(doc, buffer);
//     client.publish(TOPIC_IR_RECV_DAT, buffer);
    
//     Serial.println("IR Received & Published to MQTT");
// }

// // ==========================================
// // Wi-Fi / MQTT 接続ヘルパー
// // ==========================================
// void setup_wifi() {
//     delay(10);
//     Serial.println();
//     Serial.print("Connecting to ");
//     Serial.println(WIFI_SSID);

//     WiFi.mode(WIFI_STA);
//     WiFi.begin(WIFI_SSID, WIFI_PASS);

//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500);
//         Serial.print(".");
//     }
//     Serial.println("\nWiFi connected");
//     Serial.println("IP address: ");
//     Serial.println(WiFi.localIP());
// }

// void reconnect() {
//     while (!client.connected()) {
//         Serial.print("Attempting MQTT connection...");
//         String clientId = "ESP32Client-";
//         clientId += String(random(0xffff), HEX);
        
//         if (client.connect(clientId.c_str())) {
//             Serial.println("connected");
//             // 起動時に購読を開始する
//             client.subscribe(TOPIC_IR_SEND_CMD);
//         } else {
//             Serial.print("failed, rc=");
//             Serial.print(client.state());
//             Serial.println(" try again in 5 seconds");
//             delay(5000);
//         }
//     }
// }