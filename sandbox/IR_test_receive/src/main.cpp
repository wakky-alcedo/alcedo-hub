#include <Arduino.h>
#include "IRremote.h"

#define ENABLE_LIGHT 0
#define ENABLE_AC 0
#define ENABLE_FAN 0
 
int IR_RECEIVE_PIN = 33;
int FEEDBACK_LED_PIN = 32;
 
void setup() {
    Serial.begin(115200);
    pinMode(FEEDBACK_LED_PIN, OUTPUT);
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK, FEEDBACK_LED_PIN);
    IrSender.begin(FEEDBACK_LED_PIN);
}


constexpr uint8_t repeatPeriodMillis = 140; //30; // 頭から頭までの時間
constexpr uint8_t numberOfRepeats = 0;
void loop() {
    if (IrReceiver.decode()) {

        // エアコン消す
        // {
        // // uint64_t tRawData[]={0x0220E00400000006,0x0220E00400005880,0x0000000EE0000081,0x00004D};
        // // IrSender.sendPulseDistanceWidthFromArray(38, 3600, 1600, 500, 1250, 500, 400, &tRawData[0], 27*8, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, numberOfRepeats);
        // // IrSender.sendPulseDistanceWidth(38, 3600, 1600, 500, 1250, 500, 400, 0x600000004E02002, 48, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, numberOfRepeats);
        // IrSender.sendPulseDistanceWidth(38, 3500, 1750, 450, 1300, 450, 400, 0x600000004E02002, 64, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, numberOfRepeats);
        // }

        // シーリングライト消灯
        {
        // uint64_t tRawData[]={0x3FF00AF50301001, 0x3DC2FC}; // 0b0000001111111111000000001010111101010000001100000001000000000001  0b001111011100001011111100
        // for (uint8_t i = 0; i < 3; i++) {
        //     IrSender.sendPulseDistanceWidthFromArray(38, 3550, 1550, 450, 1200, 450, 400, &tRawData[0], 88, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, numberOfRepeats);
        //     delay(30);
        // }
        }

        Serial.println("Short: ");
        IrReceiver.printIRResultShort(&Serial);
        Serial.println("Raw: ");
        IrReceiver.printIRResultRawFormatted(&Serial, true);
        // IrReceiver.printIRResultRawFormatted(&Serial, false);
        Serial.println("Minimal: ");
        IrReceiver.printIRResultMinimal(&Serial);
        Serial.println("\nAs C Variables: ");
        IrReceiver.printIRResultAsCVariables(&Serial);
        Serial.println("SendUsage: ");
        IrReceiver.printIRSendUsage(&Serial);
        Serial.println("--------------------");
        IrReceiver.resume();
    }

    uint16_t address = 0x0;
    uint16_t command = 0x0;
    uint64_t rawData = 0x3DC2FC;

    // delay(1);


    // uint64_t tRawData[]={0xFFF00AF50301001, 0x3DC2F0};
    // IrSender.sendPulseDistanceWidthFromArray(38, 3550, 1550, 500, 1150, 500, 350, &tRawData[0], 88, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, numberOfRepeats);
    // IrSender.sendKaseikyo(0x1, 0x1, 1, 0x1);

#if ENABLE_LIGHT
    // シーリングライト
    // 消灯
    {
    // uint16_t address = 0x0;
    // uint16_t command = 0x0;
    // uint64_t rawData = 0x3DC2FC; // 0b1111011100001011111100
    uint64_t tRawData[]={0x3FF00AF50301001, 0x3DC2FC}; // 0b0000001111111111000000001010111101010000001100000001000000000001  0b001111011100001011111100
    for (uint8_t i = 0; i < 3; i++) {
        IrSender.sendPulseDistanceWidthFromArray(38, 3550, 1550, 450, 1200, 450, 400, &tRawData[0], 88, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, 0);
        // IrSender.sendKaseikyo
        delay(30);
    }
    delay(2000);
    }


    // // お好み
    {
    uint64_t tRawData[]={0x4FF00AF50301001, 0x3DC2FB};
    IrSender.sendPulseDistanceWidthFromArray(38, 3550, 1550, 450, 1200, 450, 400, &tRawData[0], 88, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, numberOfRepeats);   
    delay(2000);
    }

    // // 全灯
    {
    uint16_t address = 0x0;
    uint16_t command = 0x0;
    uint64_t rawData = 0x3DC2FD; // 0b1111011100001011111101
    uint64_t tRawData[]={0x2FF00AF50301001, 0x3DC2FD}; // 0b0000001011111111000000001010111101010000001100000001000000000001  0b001111011100001011111101
    for (uint8_t i = 0; i < 3; i++) {
        IrSender.sendPulseDistanceWidthFromArray(38, 3550, 1550, 450, 1200, 450, 400, &tRawData[0], 88, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, 0);
        delay(30);
    }
    delay(2000);
    }

    // 保安灯
    // {
    // uint64_t tRawData[]={0x9FF00AF50301001, 0x3DC2F6};
    // IrSender.sendPulseDistanceWidthFromArray(38, 3500, 1550, 450, 1200, 450, 350, &tRawData[0], 88, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, numberOfRepeats);
    // delay(2000);
    // }

    // おやすみ30分
    // {
    // uint64_t tRawData[]={0x5FF00AF50301001, 0xDF2FA};
    // IrSender.sendPulseDistanceWidthFromArray(38, 3550, 1550, 450, 1200, 450, 350, &tRawData[0], 88, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, numberOfRepeats);
    // delay(1000);
    // }

    // 白色
    {
    uint64_t tRawData[]={0xEFF00AF50301001, 0x3DC2F1};
    IrSender.sendPulseDistanceWidthFromArray(38, 3500, 1600, 500, 1200, 500, 350, &tRawData[0], 88, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, numberOfRepeats);
    delay(1000);
    }

    // 暖色
    {
    uint64_t tRawData[]={0xFFF00AF50301001, 0x3DC2F0};
    IrSender.sendPulseDistanceWidthFromArray(38, 3500, 1600, 450, 1200, 450, 350, &tRawData[0], 88, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, numberOfRepeats);
    delay(1000);
    }

    // 明
    {
    uint64_t tRawData[]={0x6FF00AF50301001, 0x3DC2F9};
    IrSender.sendPulseDistanceWidthFromArray(38, 3500, 1550, 450, 1200, 450, 400, &tRawData[0], 88, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, numberOfRepeats);
    delay(1000);
    }

    // 暗
    {
    uint64_t tRawData[]={0x7FF00AF50301001, 0x3DC2F8};
    IrSender.sendPulseDistanceWidthFromArray(38, 3500, 1550, 500, 1200, 500, 350, &tRawData[0], 88, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, numberOfRepeats);
    delay(1000);
    }
#endif

#if ENABLE_AC
    // エアコン

    // 停止
    {
    // uint64_t tRawData[]={0x0220E00400000006,0x0220E00400005880,0x0000000EE0000081,0x00004D};
    // IrSender.sendPulseDistanceWidthFromArray(38, 3600, 1600, 500, 1250, 500, 400, &tRawData[0], 27*8, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, numberOfRepeats);
    // // IrSender.sendPulseDistanceWidth(38, 3600, 1600, 500, 1250, 500, 400, 0x600000004E02002, 64, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, numberOfRepeats);
    // // IrSender.sendPulseDistanceWidth(38, 3500, 1750, 450, 1300, 450, 400, 0x600000004E02002, 64, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, numberOfRepeats);
    // delay(3000);
    }
    // IrSender.sendPanasonic()


#endif

#if ENABLE_FAN
    // 扇風機
    {
    constexpr uint8_t numberOfRepeats = 0;

    // // 電源
    // IrSender.sendNEC(0x0, 0x45, 0);
    // delay(2000);

    // // 風量+
    // IrSender.sendNEC(0x0, 0x07, 0);
    // delay(2000);
    // // 風量-
    // IrSender.sendNEC(0x0, 0x09, 0);
    // delay(2000);

    // // 左右首振り
    // IrSender.sendNEC(0x0, 0x0C, 0);
    // delay(2000);
    // // 上下首振り
    // IrSender.sendNEC(0x0, 0x5E, 0);
    // delay(2000);

    // // タイマー
    // IrSender.sendNEC(0x0, 0x47, 0);


    }
#endif

    // 暖房
    // IrSender.sendPulseDistanceWidth(38, 3600, 1650, 450, 1250, 450, 400, 0x600000004E02002, 64, PROTOCOL_IS_LSB_FIRST, repeatPeriodMillis, numberOfRepeats);
}