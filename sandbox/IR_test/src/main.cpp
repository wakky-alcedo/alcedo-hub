#include <Arduino.h>
#include <IRremote.hpp>

#define ENABLE_LIGHT 0
#define ENABLE_AC 1
#define ENABLE_FAN 0

constexpr uint8_t IR_SEND_PIN = 32;
constexpr uint8_t IR_RECV_PIN = 33;
constexpr uint8_t USER_LERN_PIN = 2;

// IRPanasonicAc pana(IR_SEND_PIN); //make an instance.

IRsend irsend=IRsend(IR_SEND_PIN);  // Set the GPIO to be used.
uint16_t rawData[35] = {  7000,4760,280,560,280,1400,280,560,280,1400,280,560,280,1400,280,560,280,1400,280,560,280,1400,280,560,280,560,280,1400,280,1400,280,1400,280,1400,280
};


void setup() {
    Serial.begin(115200);  // シリアル通信の初期化

    // ピン設定
    pinMode(USER_LERN_PIN, OUTPUT);
    pinMode(IR_SEND_PIN, OUTPUT);
    pinMode(IR_RECV_PIN, INPUT);


    IrSender.begin(IR_SEND_PIN);
    IrReceiver.begin(IR_RECV_PIN);
}

void loop() {

#if ENABLE_LIGHT
    // Light ON
    IrSender.sendNEC(0x20DF10EF, 32);
#endif

#if ENABLE_AC
    // エアコン Panasonic
    // ON
    
    // static IRRawDataType tSendValue = 0x0220E00400000006;
    // Serial.println(tSendValue);
    // IrSender.sendPulseDistanceWidth(&KaseikyoProtocolConstants, tSendValue, KASEIKYO_BITS, 1);
    // tSendValue++;

    static uint16_t tAddress = 0x0220; //0220
    static uint8_t tCommand = 0x00;
    Serial.print("Address: ");
    char charAdress[4] = "";
    sprintf(charAdress, "%04X", tAddress);
    Serial.print(charAdress);
    // Serial.print(tAddress);
    Serial.print(", Command: ");
    char charCommand[2] = "";
    sprintf(charCommand, "%02X", tCommand);
    Serial.println(charCommand);
    // Serial.println(tCommand);
    IrSender.getPulseCorrectionNanos();
    IrSender.sendPanasonic(tAddress, tCommand, 1);
    tCommand++;
    if (tCommand == 0x00) {
        tAddress++;
    }
#endif


    // シーリングライト？
    // irsend.sendRaw(rawData, 35, 38);  //Send a raw data at 38kHz.
    // delay(2000); //wait for a second.


    // ChatGPT
      // 例: NECプロトコルで送信（キャプチャした信号に基づいて変更）
    // uint64_t data = 0x20DF10EF; // キャプチャした信号のデータに置き換え
    // uint16_t bits = 32;         // ビット数（プロトコルにより異なる）
    
    // Serial.println("Sending IR signal...");
    // irsend.sendNEC(data, bits);

#if ENABLE_FAN



#endif

    delay(10); // 5秒ごとに送信
}
