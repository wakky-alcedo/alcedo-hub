#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEClient.h>

#define SERVICE_UUID        "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID "87654321-4321-6789-4321-abcdef987650"

static BLEAdvertisedDevice* myDevice;
bool doConnect = false;
BLEClient* pClient = NULL;
BLERemoteCharacteristic* pRemoteCharacteristic = NULL;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(BLEUUID(SERVICE_UUID))) {
            myDevice = new BLEAdvertisedDevice(advertisedDevice);
            advertisedDevice.getScan()->stop();
            doConnect = true;
        }
    }
};

class MyClientCallback: public BLEClientCallbacks {
    void onConnect(BLEClient* pClient) {
        Serial.println("Connected to Peripheral");
    }
    void onDisconnect(BLEClient* pClient) {
        Serial.println("Disconnected from Peripheral");
    }
};

void onNotify(BLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    Serial.print("Received: ");
    Serial.println((char*)pData);
}

void setup() {
    Serial.begin(115200);
    BLEDevice::init("");

    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);
}

void loop() {
    if (doConnect) {
        pClient = BLEDevice::createClient();
        pClient->setClientCallbacks(new MyClientCallback());

        if (pClient->connect(myDevice)) {
            BLERemoteService* pRemoteService = pClient->getService(BLEUUID(SERVICE_UUID));
            if (pRemoteService) {
                pRemoteCharacteristic = pRemoteService->getCharacteristic(BLEUUID(CHARACTERISTIC_UUID));
                if (pRemoteCharacteristic) {
                    if (pRemoteCharacteristic->canNotify()) {
                        pRemoteCharacteristic->registerForNotify(onNotify);
                    }
                }
            }
        }
        doConnect = false;
    }
    delay(2000);
}
