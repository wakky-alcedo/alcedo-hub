#ifndef IRSENDLIGHT_HPP_
#define IRSENDLIGHT_HPP_

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

enum class LightCommand : uint8_t {
    On = 0x02,
    Off = 0x03,
    Favarite = 0x04,
    BrightnessUp = 0x06,
    BrightnessDown = 0x07,
    SafetyLamp = 0x09,
    ColorTempUp = 0x0F,
    ColorTempDown = 0x0E,
};

class IrSendLight {
private:
    IRsend irsend = IRsend(0);
    uint16_t IRsendPin;
public:
    IrSendLight(uint16_t IRsendPin);
    ~IrSendLight();
    void begin();
    void send_raw(uint8_t* raw_data);
    void send(uint8_t data, uint8_t number_of_times = 3);
    void send(LightCommand command, uint8_t number_of_times = 3);
};

IrSendLight::IrSendLight(uint16_t IRsendPin)
        : IRsendPin(IRsendPin) {
    irsend = IRsend(IRsendPin);  // Set the GPIO to be used.
}

IrSendLight::~IrSendLight() {
}

void IrSendLight::begin() {
    pinMode(IRsendPin, OUTPUT);
    irsend.begin();
}


// シーリングライトのリモコン信号を送信する
void IrSendLight::send_raw(uint8_t* raw_data) {
    constexpr uint16_t KASEIKYO_UNIT               = 432; // 16 pulses of 37 kHz (432,432432)  - Pronto 0x70 | 0x10
    constexpr uint16_t KASEIKYO_HEADER_MARK        = (8 * KASEIKYO_UNIT); // 3456
    constexpr uint16_t KASEIKYO_HEADER_SPACE       = (4 * KASEIKYO_UNIT); // 1728
    constexpr uint16_t KASEIKYO_BIT_MARK           = KASEIKYO_UNIT;
    constexpr uint16_t KASEIKYO_ONE_SPACE          = (3 * KASEIKYO_UNIT); // 1296
    constexpr uint16_t KASEIKYO_ZERO_SPACE         = KASEIKYO_UNIT; // 432

    // 送信する信号のデータ
    irsend.sendGeneric(KASEIKYO_HEADER_MARK, KASEIKYO_HEADER_SPACE,
                        KASEIKYO_BIT_MARK, KASEIKYO_ONE_SPACE,
                        KASEIKYO_BIT_MARK, KASEIKYO_ZERO_SPACE,
                        KASEIKYO_BIT_MARK, 0, (uint8_t*)raw_data, 88/8, 38, false, 0, 50); // dataの順番が違う可能性？
}

void IrSendLight::send(uint8_t data, uint8_t number_of_times) {
    uint8_t tRawData[11] = {0x01, 0x10, 0x30, 0x50, 0xAF, 0x00, 0xFF, data, ~data, 0xC2, 0x3D};
    for (uint8_t i = 0; i < number_of_times; i++) {
        send_raw((uint8_t*)tRawData);
        delay(30);
    }
}

void IrSendLight::send(LightCommand command, uint8_t number_of_times) {
    send((uint8_t)command, number_of_times);
}

#endif // IRSENDLIGHT_HPP_