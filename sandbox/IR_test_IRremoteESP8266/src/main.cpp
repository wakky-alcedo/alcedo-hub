#include <Arduino.h>

#include <IRremoteESP8266.h>
#include <ir_Panasonic.h>
#include <IRsend.h>
#include "IrSendLight.hpp"

#define ENABLE_LIGHT 1
#define ENABLE_AC 0
#define ENABLE_FAN 0

constexpr uint8_t IR_SEND_PIN = 32;
constexpr uint8_t IR_RECV_PIN = 33;
constexpr uint8_t USER_LERN_PIN = 2;

IRPanasonicAc pana(IR_SEND_PIN); //make an instance.

IRsend irsend=IRsend(IR_SEND_PIN);  // Set the GPIO to be used.

IrSendLight irsendLight(IR_SEND_PIN);

uint16_t rawData[35] = {  7000,4760,280,560,280,1400,280,560,280,1400,280,560,280,1400,280,560,280,1400,280,560,280,1400,280,560,280,560,280,1400,280,1400,280,1400,280,1400,280
};


void setup() {
    Serial.begin(115200);  // シリアル通信の初期化

    // ピン設定
    pinMode(USER_LERN_PIN, OUTPUT);
    pinMode(IR_SEND_PIN, OUTPUT);
    pinMode(IR_RECV_PIN, INPUT);

    pana.begin();
    irsend.begin();
    irsendLight.begin();

}

void loop() {
#if ENABLE_LIGHT
    // irsendLight.send(LightCmmand::On);
    // delay(2000);
    // irsendLight.send(LightCmmand::Favarite);
    // delay(2000);
    // irsendLight.send(LightCmmand::Off);
    // delay(2000);

    static uint8_t data = 0x20;
    Serial.println(data, HEX);
    irsendLight.send(data);
    data++;
    while(data == 0x02 || data == 0x03 || data == 0x04 || data == 0x06 || data == 0x07 || data == 0x09 || data == 0x0F || data == 0x0E) {
        data++;
    };
    if (data > 0x30) {
        data = 0x20;
    }
    delay(2000);

#endif

#if ENABLE_AC
    // エアコン Panasonic
    // ON
    // IrSender.sendPanasonic(0x4004,

    // if (IrReceiver.decode()) {  // 信号を受信したら
    //     Serial.print("Received Raw Data: ");
    //     Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);  // 生のデータを16進数で表示

    //     IrReceiver.printIRResultShort(&Serial);  // 受信したデータの簡潔な概要を表示
    //     IrReceiver.printIRSendUsage(&Serial);    // 受信した信号を送信するためのコードを表示

    //     IrReceiver.resume();  // 次の信号を受信できるようにする
    // }


    // エアコン Panasonic うまくいった！
    // pana.on(); //power on the aircon.
    // pana.setTemp(16);
    // pana.setMode(kPanasonicAcHeat);
    // pana.setFan(kPanasonicAcFanAuto);
    // pana.setSwingVertical(kPanasonicAcSwingVAuto);
    // pana.send(); //send IR signal.
    // digitalWrite(USER_LERN_PIN, HIGH);
    // delay(2000); //wait for a second.

    pana.off(); //power off the aircon.
    for (uint8_t i = 0; i < kPanasonicAcStateLength; i++) {
      if (i%8 == 0) {Serial.print("0x"); };
      char sendPacket[2] = "";
      sprintf(sendPacket, "%02X", *(pana.getRaw()+i));
      Serial.print(sendPacket);
      if (i%8 == 7) {Serial.print(","); };
    }
    Serial.println("");
    
    // Serial.println(*pana.getRaw());
    pana.send(); //send IR signal.
    // digitalWrite(USER_LERN_PIN, LOW);
    // delay(2000); //wait for a second.

#endif

#if ENABLE_FAN
    // シーリングライト？
    // irsend.sendRaw(rawData, 35, 38);  //Send a raw data at 38kHz.
    // delay(2000); //wait for a second.


    // ChatGPT
      // 例: NECプロトコルで送信（キャプチャした信号に基づいて変更）
    // uint64_t data = 0x20DF10EF; // キャプチャした信号のデータに置き換え
    // uint16_t bits = 32;         // ビット数（プロトコルにより異なる）
    
    // Serial.println("Sending IR signal...");
    // irsend.sendNEC(data, bits);

#endif

    // delay(2000); // 5秒ごとに送信
}
