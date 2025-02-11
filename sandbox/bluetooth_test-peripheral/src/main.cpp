#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID "87654321-4321-6789-4321-abcdef987650"

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
int lastSensorValue = -1;  // 前回のセンサーデータ

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
    }
};

void setup() {
    Serial.begin(115200);
    BLEDevice::init("ESP32_Peripheral");

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_NOTIFY
                      );

    pCharacteristic->addDescriptor(new BLE2902());
    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pServer->getAdvertising()->start();
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
