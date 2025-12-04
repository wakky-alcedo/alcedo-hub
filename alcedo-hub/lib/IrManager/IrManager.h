#pragma once
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>

class IrManager {
public:
    IrManager(uint16_t sendPin, uint16_t recvPin);
    void setup();
    void loop();
    
    // 送信系
    void sendNEC(uint64_t data, uint16_t bits);
    void sendRaw(const uint16_t* buf, uint16_t len, uint16_t freq);
    
    // 受信系
    bool hasReceivedData();
    String getLastReceivedHex();
    String getLastProtocolName();

private:
    uint16_t _sendPin;
    uint16_t _recvPin;
    IRsend _irsend;
    IRrecv _irrecv;
    decode_results _results;
    bool _dataReady;
};