#ifndef DRAW_SCREAN_HPP_
#define DRAW_SCREAN_HPP_

#include <LovyanGFX.hpp>
#include <icon.hpp>
#include <display.hpp>

uint16_t icon_y = 130;
uint16_t width = 320;
uint16_t height = 240;

// 画面遷移のenum class
enum class Screen : uint8_t {
    HOME = 0,
    LIGHT = 1,
    AC = 2,
    CURTAIN = 3,
};

void draw_home_screen(LGFX& display) {
    display.fillScreen(TFT_BLACK);
    display.setTextColor(TFT_WHITE);
    // display.drawString("HOME", 0, 0);
    // display.drawString("LIGHT", 0, 20);
    // display.drawString("AC", 0, 40);
    // display.drawString("CURTAIN", 0, 60);


    // 温度描画
    // display.setTextSize(3);
    // display.setFont(&fonts::lgfxJapanGothicP_24);
    display.setFont(&fonts::Font4);
    display.setTextColor(TFT_WHITE);
    display.setCursor(10, 10);
    display.print("25");
    display.drawCircle(60, 15, 5, TFT_WHITE); // ℃マーク
    display.setCursor(70, 10);
    display.print("C");

    // 湿度描画
    display.setCursor(180, 10);
    display.print("50%");
    
    // 気圧描画
    display.setTextSize(2);
    display.setCursor(10, 80);
    display.print("1024 hPa");


    LGFX_Sprite light_icon(&display);     // displayに描画するスプライト作成
    LGFX_Sprite ac_icon(&display);     // displayに描画するスプライト作成

    // 電球アイコン
    // display_light_icon(light_icon);
    draw_light_bulb_icon(light_icon, 50, 80);

    // エアコンアイコン
    display_ac_icon(ac_icon, 70, 30);

    // display.startWrite(); // 描画を開始
    light_icon.pushSprite(30, icon_y);
    ac_icon.pushSprite(width/2, icon_y);
    // display.endWrite(); // 描画を終了
}

void draw_light_control(LGFX& display) {
    uint32_t text_y = 50;
    uint32_t text_width = 80;
    uint32_t text_height = 40;

    display.fillScreen(TFT_BLACK);
    
    // Draw ON button
    display.draw_textbox(display.width()/2 - text_width - 30, text_y, text_width, text_height, "ON");

    // Draw OFF button
    display.draw_textbox(display.width()/2 + 30, text_y, text_width, text_height, "OFF");

    // Draw first slider
    display.draw_slider(30, 130, 200, 20, 100);

    // Draw second slider
    display.draw_slider(30, 180, 200, 20, 0);
}

void draw_airconditioner_control(LGFX& display) {
    display.fillScreen(TFT_BLACK);
    
    // Draw ON button
    display.draw_textbox(20, 20, 80, 40, "ON");

    // Draw OFF button
    display.draw_textbox(120, 20, 80, 40, "OFF");
}

void draw_curtain_control(LGFX& display) {
    uint32_t text_y = 50;
    uint32_t text_width = 80;
    uint32_t text_height = 40;
    
    display.fillScreen(TFT_BLACK);
    
    // Draw OPEN button
    display.draw_textbox(display.width()/2 - text_width - 30, text_y, text_width, text_height, "OPEN");

    // Draw CLOSE button
    display.draw_textbox(display.width()/2 + 30, text_y, text_width, text_height, "CLOSE");

    // Draw slider
    display.draw_slider(30, 130, 200, 20, 50);
}

#endif // DRAW_SCREAN_HPP_
