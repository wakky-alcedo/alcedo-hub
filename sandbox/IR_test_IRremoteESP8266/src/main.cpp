#include <Arduino.h>

#include <IRremoteESP8266.h>
#include <ir_Panasonic.h>
#include <IRsend.h>

#define ENABLE_LIGHT 1
#define ENABLE_AC 0
#define ENABLE_FAN 0

constexpr uint8_t IR_SEND_PIN = 32;
constexpr uint8_t IR_RECV_PIN = 33;
constexpr uint8_t USER_LERN_PIN = 2;

IRPanasonicAc pana(IR_SEND_PIN); //make an instance.

IRsend irsend=IRsend(IR_SEND_PIN);  // Set the GPIO to be used.
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
}

void loop() {
#if ENABLE_LIGHT
    // Light ON
    // IrSender.sendNEC(0x20DF10EF, 32);
    // irsend.sendNEC( )
    #define KASEIKYO_UNIT               432 // 16 pulses of 37 kHz (432,432432)  - Pronto 0x70 | 0x10
    #define KASEIKYO_HEADER_MARK        (8 * KASEIKYO_UNIT) // 3456
    #define KASEIKYO_HEADER_SPACE       (4 * KASEIKYO_UNIT) // 1728
    #define KASEIKYO_BIT_MARK           KASEIKYO_UNIT
    #define KASEIKYO_ONE_SPACE          (3 * KASEIKYO_UNIT) // 1296
    #define KASEIKYO_ZERO_SPACE         KASEIKYO_UNIT
    // 全灯
    // uint64_t tRawData[]={0x2FF00AF50301001, 0x3DC2FD}; // 0b0000001011111111000000001010111101010000001100000001000000000001  0b001111011100001011111101
    // お好み
    uint64_t tRawData[]={0x4FF00AF50301001, 0x3DC2FB};
    // 消灯
    // uint64_t tRawData[]={0x3FF00AF50301001, 0x3DC2FC}; // 0b0000001111111111000000001010111101010000001100000001000000000001  0b001111011100001011111100
    for (uint8_t i = 0; i < 3; i++) {
        irsend.sendGeneric(KASEIKYO_HEADER_MARK, KASEIKYO_HEADER_SPACE,
                            KASEIKYO_BIT_MARK, KASEIKYO_ONE_SPACE,
                            KASEIKYO_BIT_MARK, KASEIKYO_ZERO_SPACE,
                            KASEIKYO_BIT_MARK, 0, (uint8_t*)tRawData, 88/8, 38, false, 0, 50); // dataの順番が違う可能性？
        delay(30);
    }

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

    delay(2000); // 5秒ごとに送信
}
