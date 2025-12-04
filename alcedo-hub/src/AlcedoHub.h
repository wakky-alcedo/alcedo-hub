#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <NetworkManager.h> // lib/AlcedoNetwork からインクルード
#include <IrManager.h>      // lib/AlcedoIR からインクルード

/**
 * @brief Alcedoシステムの中心ハブ
 * 通信(Network)、ハードウェア(IR)、UI(GUI)を調停する
 */
class AlcedoHub {
public:
    AlcedoHub();
    void setup();
    void loop();

private:
    // --- 内部モジュール ---
    NetworkManager _network;
    IrManager _ir;

    // --- コールバックハンドラ ---
    // MQTTメッセージを受信したときに呼ばれる関数
    void _onMqttMessage(String topic, String payload);
    void _onGuiEvent(int eventId, int value);
};