#include <Arduino.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <display.hpp>

#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>

#define PIN_LED_RED 4
#define PIN_LED_GREEN 16
#define PIN_LED_BLUE 17
#define PIN_BRIGTHNESS 4
#define PIN_SPEAKER 26

LGFX display;
LGFX_Sprite sprite(&display);      // lcdに描画するスプライト作成

Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp;

void setup(void) {
    display.init();

    display.setTextSize((std::max(display.width(), display.height()) + 255) >> 8);

    if (display.touch()) {
        if (display.width() < display.height()) {
            display.setRotation(display.getRotation() ^ 1); // 縦向きの場合は横向きにする
        }

        display.setTextDatum(textdatum_t::middle_center); // テキストの描画位置を設定
        display.drawString("touch the arrow marker.", display.width() >> 1, display.height() >> 1); // メッセージを表示
        display.setTextDatum(textdatum_t::top_left); // テキストの描画位置を設定

        // std::uint16_t fg = TFT_WHITE;
        // std::uint16_t bg = TFT_BLACK;
        // if (display.isEPD()) std::swap(fg, bg);
        // display.calibrateTouch(nullptr, fg, bg, std::max(display.width(), display.height()) >> 3);
    }

    display.fillScreen(TFT_BLACK);

    display.setBrightness(255); // 明るさの設定 なんかうまく反映されていなさそう？

    // ピンの設定
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_BLUE, OUTPUT);
    pinMode(PIN_BRIGTHNESS, INPUT);

    analogWrite(PIN_LED_RED, 0); // 明るさが逆
    analogWrite(PIN_LED_GREEN, 0);
    analogWrite(PIN_LED_BLUE, 0);

    sprite.createSprite(100, 100);

    aht.begin(&Wire, 0, AHTX0_I2CADDR_ALTERNATE);
    bmp.begin();
}

uint32_t count = ~0;
void loop(void) {
    // 明るさを取得
    uint16_t brightness = analogRead(34);

    // 温度と湿度を取得
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp); // AHT21から湿度と温度の取得
    float temperature1 = temp.temperature;
    uint8_t relative_humidity = humidity.relative_humidity;

    // 気圧を取得
    bmp.takeForcedMeasurement();
    float temperature2 = bmp.readTemperature();
    float pressure = bmp.readPressure() / 100.0F;

    float temperature = (temperature1 + temperature2) / 2.0F;

    sprite.fillScreen(TFT_BLACK);
    sprite.setTextColor(TFT_WHITE); // 文字色を設定
    sprite.drawString("brightness", 0, 0);
    sprite.drawNumber(brightness, 70, 0); // 明るさを表示
    sprite.drawNumber((int)temperature1, 0, 8); // 温度を表示
    sprite.drawNumber((int)temperature2, 20, 8); // 温度を表示
    sprite.drawNumber((int)relative_humidity, 40, 8); // 湿度を表示
    sprite.drawNumber(pressure, 60, 8); // 気圧を表示


    display.startWrite(); // 描画を開始
    sprite.pushSprite(30, 50);
    display.endWrite(); // 描画を終了
}