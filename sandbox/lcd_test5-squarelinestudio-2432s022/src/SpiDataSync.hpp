#ifndef SPI_DATA_SYNC_HPP_
#define SPI_DATA_SYNC_HPP_

#include "IrSendAC.hpp"
#include <SPI.h>
#include <stdio.h>
#include <string.h>

struct ClimateData {
    uint16_t temperature = 250;
    uint8_t humidity = 50;
    uint8_t pressure = 1013 - 900;
};

struct LightData {
    bool power = false;
    uint8_t brightness = 0;
    uint8_t color_temp = 126;
};

enum class CurtainMode : uint8_t {
    Stop = 0x00,
    Opening = 0x01,
    Closing = 0x02
};

struct CurtainData {
    CurtainMode mode = CurtainMode::Stop;
    uint8_t position = 0; // 0~63, 0:閉, 63:開
};

struct FanData {
    bool power = false;
    uint8_t speed = 0; // 0~32
    bool is_horizontal_swing = false;
    bool is_vertical_swing = false;
};

constexpr uint8_t data_size = 9;

class SpiDataSync
{
private:
    SPIClass spi;
    uint8_t cs_pin;
    uint8_t last_send_data[data_size];
    uint8_t last_receive_data[data_size];
public:
    SpiDataSync(uint8_t spi_bus=HSPI);
    ~SpiDataSync();
    void master_begin(uint8_t sck_pin, uint8_t miso_pin, uint8_t mosi_pin, uint8_t cs_pin);
    void receive_begin(uint8_t sck_pin, uint8_t miso_pin, uint8_t mosi_pin, uint8_t cs_pin, void(*onDataReceived)());
    void master_sync(ClimateData& climate, LightData& light, AC_Data& ac, CurtainData& curtain, FanData& fan);
    void receive_sync(ClimateData& climate, LightData& light, AC_Data& ac, CurtainData& curtain, FanData& fan);
};

SpiDataSync::SpiDataSync(uint8_t spi_bus)
    : spi(spi_bus) {
}

SpiDataSync::~SpiDataSync()
{
}

void SpiDataSync::master_begin(uint8_t sck_pin, uint8_t miso_pin, uint8_t mosi_pin, uint8_t cs_pin) {
    spi.begin(sck_pin, miso_pin, mosi_pin, cs_pin);
    this->cs_pin = cs_pin;
    pinMode(cs_pin, OUTPUT);
    digitalWrite(cs_pin, HIGH);
}

void SpiDataSync::receive_begin(uint8_t sck_pin, uint8_t miso_pin, uint8_t mosi_pin, uint8_t cs_pin, void(*onDataReceived)()) {
    spi.begin(sck_pin, miso_pin, mosi_pin, cs_pin);
    this->cs_pin = cs_pin;
    pinMode(cs_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(cs_pin), onDataReceived, FALLING);
}


void SpiDataSync::master_sync (ClimateData& climate, LightData& light, AC_Data& ac, CurtainData& curtain, FanData& fan) {
    uint8_t send_data[data_size];
    uint8_t raw_receive_data[data_size];
    strlcpy((char*)send_data    , (char*)&climate, 4);
    strlcpy((char*)&send_data[4], (char*)&light.brightness, 1);
    strlcpy((char*)&send_data[5], (char*)&light.color_temp, 1);
    send_data[6] = 0; // light, ac
    send_data[6] |= (uint8_t)light.power;
    send_data[6] |= (uint8_t)ac.power     << 1;
    send_data[6] |= (uint8_t)ac.mode      << 2;
    send_data[6] |= (uint8_t)(ac.temp-16) << 4;
    send_data[7] = 0; // curtain
    send_data[7] |= (uint8_t)curtain.mode;
    send_data[7] |= (uint8_t)curtain.position << 2;
    send_data[8] = 0; // fan
    send_data[8] |= (uint8_t)fan.power;
    send_data[8] |= (uint8_t)fan.speed << 1;
    send_data[8] |= (uint8_t)fan.is_horizontal_swing << 6;
    send_data[8] |= (uint8_t)fan.is_vertical_swing << 7;

    digitalWrite(cs_pin, LOW); // スレーブを選択
    spi.transferBytes(send_data, raw_receive_data, data_size);
    digitalWrite(cs_pin, HIGH); // スレーブを解除

    uint8_t receive_data[data_size];
    for (int i = 0; i < data_size; i++) {
        uint8_t send_diff = last_send_data[i] ^ send_data[i];
        uint8_t receive_diff = last_receive_data[i] ^ raw_receive_data[i];

        receive_data[i]  = raw_receive_data[i] & ~(send_diff | receive_diff);   // まず同じところを代入
        // receive_data[i] |= send_data[i]        & send_diff & ~receive_diff;     // sendだけで変化した値を代入
        receive_data[i] |= raw_receive_data[i] & ~send_diff & receive_diff;     // receiveだけで変化した値を代入
        receive_data[i] |= send_data[i]        & send_diff;                     // sendで変化した値receiveに問答無用で代入
    
        last_send_data[i] = send_data[i];
        last_receive_data[i] = receive_data[i];
    }

    strlcpy((char*)&climate, (char*)receive_data    , 4);
    strlcpy((char*)&light.brightness, (char*)&receive_data[4], 1);
    strlcpy((char*)&light.color_temp, (char*)&receive_data[5], 1);
    light.power = (bool)(receive_data[6] & 0b00000001);
    ac.power = (bool)(receive_data[6] & 0b00000010);
    ac.mode = (AC_Mode)((receive_data[6] & 0b00001100) >> 2);
    ac.temp = (receive_data[6] & 0b11110000) >> 4;
    curtain.mode = (CurtainMode)(receive_data[7] & 0b00000011);
    curtain.position = (receive_data[7] & 0b11111100) >> 2;
    fan.power = (bool)(receive_data[8] & 0b00000001);
    fan.speed = (receive_data[8] & 0b00111110) >> 1;
    fan.is_horizontal_swing = (bool)(receive_data[8] & 0b01000000);
    fan.is_vertical_swing = (bool)(receive_data[8] & 0b10000000);
}

#endif // SPI_DATA_SYNC_HPP_