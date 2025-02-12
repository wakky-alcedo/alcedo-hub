#include <Arduino.h>
#include <NimBLEDevice.h>
#include <NimBLECharacteristic.h>
#include <BLE2902.h>              // NimBLE2902 のヘッダファイル

#define SERVICE_UUID        "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID "87654321-4321-6789-4321-abcdef987650"

static NimBLEAdvertisedDevice* myDevice;
bool doConnect = false;
NimBLEClient* pClient = nullptr;
BLERemoteCharacteristic* pRemoteCharacteristic = nullptr;

// アドバタイズされたデバイスが見つかった時のコールバック
class MyAdvertisedDeviceCallbacks: public NimBLEAdvertisedDeviceCallbacks {
    void onResult(NimBLEAdvertisedDevice advertisedDevice) {
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(NimBLEUUID(SERVICE_UUID))) {
            myDevice = new NimBLEAdvertisedDevice(advertisedDevice);
            advertisedDevice.getScan()->stop();
            doConnect = true;
        }
    }
};

// 接続後のコールバック
class MyClientCallback: public NimBLEClientCallbacks {
    void onConnect(NimBLEClient* pClient) {
        Serial.println("Connected to Peripheral");
    }
    void onDisconnect(NimBLEClient* pClient) {
        Serial.println("Disconnected from Peripheral");
    }
};

// Notify 受信時のコールバック
void onNotify(BLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    Serial.print("Received: ");
    Serial.println((char*)pData);
}

void setup() {
    Serial.begin(115200);
    NimBLEDevice::init("");

    // スキャン開始
    NimBLEScan* pBLEScan = NimBLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);
}

void loop() {
    if (doConnect) {
        // Peripheral に接続
        pClient = NimBLEDevice::createClient();
        pClient->setClientCallbacks(new MyClientCallback());

        if (pClient->connect(myDevice)) {
            Serial.println("Connected to Peripheral");

            // サービス取得
            BLERemoteService* pRemoteService = pClient->getService(NimBLEUUID(SERVICE_UUID));
            if (pRemoteService) {
                // キャラクタリスティック取得
                pRemoteCharacteristic = pRemoteService->getCharacteristic(NimBLEUUID(CHARACTERISTIC_UUID));
                if (pRemoteCharacteristic) {
                    if (pRemoteCharacteristic->canNotify()) {
                        pRemoteCharacteristic->registerForNotify(onNotify);  // Notifyを受け取る
                    }
                }
            }
        }
        doConnect = false;
    }
    delay(2000);
}
