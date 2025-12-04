/**
 * @file Config.h
 * @brief ハードウェアのピン配置や定数を定義
 */
#pragma once

#include <cstdint>

// 赤外線
constexpr uint8_t PIN_IR_SEND = 18;
constexpr uint8_t PIN_IR_RECV = 19;

// ディスプレイ (例: ILI9341など)
// constexpr uint8_t PIN_LCD_MISO = 19;
// constexpr uint8_t PIN_LCD_MOSI = 23;
// constexpr uint8_t PIN_LCD_CLK  = 18;
// constexpr uint8_t PIN_LCD_CS   = 5;
// constexpr uint8_t PIN_LCD_DC   = 2;
// constexpr uint8_t PIN_LCD_RST  = 4; // Touchと共有等の場合あり

// Wi-Fi / MQTT
constexpr const char* WIFI_SSID   = "Kawasemi-G";
constexpr const char* WIFI_PASS   = "most9849";
constexpr const char* MQTT_SERVER = "192.168.10.25";