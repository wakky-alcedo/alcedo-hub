#include <lvgl.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <display/display.hpp>
#include <ui/ui.h>
#include "display/CST820.h"
#include "AlcedoHub.h"

/*Don't forget to set Sketchbook location in File/Preferences to the path of your UI project (the parent foder of this INO file)*/

/*Change to your screen resolution*/

/* spi通信 */
#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define SPI_CS 5

AlcedoHub hub;

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */

    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // （Brownout検出器を無効化）

    hub.setup();

    display_setup();

    Serial.println( "Setup done" );
}

void loop() {
    hub.loop();
    display_loop();
    // char buf[8];
    // snprintf(buf, sizeof(buf), "%4.1f", climate_data.temperature/10.0f); // スライダーの値を文字列に変換
    // lv_label_set_text(ui_Label3, buf); // ラベルのテキストを更新
    // snprintf(buf, sizeof(buf), "%2d", climate_data.humidity);
    // lv_label_set_text(ui_Label1, buf); // ラベルのテキストを更新
    // snprintf(buf, sizeof(buf), "%4d", climate_data.pressure + 900);
    // lv_label_set_text(ui_Label8, buf); // ラベルのテキストを更新
    // あとで，他の値もここに入れる
}
