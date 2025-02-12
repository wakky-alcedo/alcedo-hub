#include <Arduino.h>
#include <NimBLEDevice.h>        // NimBLE 関連の基本設定
// #include <NimBLECharacteristic.h> // NimBLE 関連のキャラクタリスティック
// #include <NimBLEDescriptor.h>     // NimBLE2902 などのデスクリプタ
#include <BLE2902.h>              // NimBLE2902 のヘッダファイル

#define SERVICE_UUID        "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID "87654321-4321-6789-4321-abcdef987650"

NimBLECharacteristic* pCharacteristic;
bool deviceConnected = false;
int lastSensorValue = -1;  // 前回のセンサーデータ

// コールバック関数
class MyServerCallbacks: public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer) {
        deviceConnected = true;
    }

    void onDisconnect(NimBLEServer* pServer) {
        deviceConnected = false;
    }
};

void setup() {
    Serial.begin(115200);
    NimBLEDevice::init("ESP32_Peripheral");

    // サーバー作成
    NimBLEServer* pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // サービス作成
    NimBLEService* pService = pServer->createService(SERVICE_UUID);

    // キャラクタリスティック作成 (Notifyプロパティ)
    pCharacteristic = pService->createCharacteristic(
                            CHARACTERISTIC_UUID,
                            NIMBLE_PROPERTY::NOTIFY
                        );

    // NimBLE2902 Descriptor を追加
    pCharacteristic->addDescriptor(new NimBLE2902());  // ここで問題の部分を修正

    pService->start();

    // アドバタイズ開始
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();
    Serial.println("Peripheral started");
}

void loop() {
    int sensorValue = analogRead(34);  // 例: GPIO34 のアナログ値

    if (deviceConnected && sensorValue != lastSensorValue) {
        String data = String(sensorValue);
        pCharacteristic->setValue(data.c_str());
        pCharacteristic->notify();  // 変更時のみ送信
        Serial.println("Sent: " + data);
        lastSensorValue = sensorValue;
    }
    delay(500);  // 短い間隔でチェック
}
