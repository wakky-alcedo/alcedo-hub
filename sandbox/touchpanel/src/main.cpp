#include <Arduino.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <display.hpp>

#define PIN_LED_RED 4
#define PIN_LED_GREEN 16
#define PIN_LED_BLUE 17
#define PIN_BRIGTHNESS 4
#define PIN_SPEAKER 26


LGFX display;
LGFX_Sprite sprite(&display);      // lcdに描画するスプライト作成

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

    sprite.createSprite(100, 20);
}

uint32_t count = ~0;
void loop(void) {
    // 明るさを取得
    uint16_t brightness = analogRead(34);

    sprite.fillScreen(TFT_BLACK);
    sprite.setTextColor(TFT_WHITE); // 文字色を設定
    sprite.drawString("brightness", 0, 0);
    sprite.drawNumber(brightness, 70, 0); // 画面の向きを表示

    display.startWrite(); // 描画を開始
    sprite.pushSprite(30, 50);
    display.endWrite(); // 描画を終了
}