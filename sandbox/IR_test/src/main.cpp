#include <Arduino.h>
#include <IRremote.hpp>

#define IR_SEND_PIN 9

void setup() {
    // 赤外線リモコンの送信ピンを設定
    IrSender.begin(IR_SEND_PIN);
}

void loop() {
    // Light ON
    IrSender.sendNEC(0x20DF10EF, 32);
    delay(1000);
}
