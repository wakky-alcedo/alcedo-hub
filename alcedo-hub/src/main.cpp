#include <lvgl.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <display/display.hpp>
#include <ui/ui.h>
#include "display/CST820.h"

#include "IrSendLight.hpp"
#include "IrSendAC.hpp"

#include <SPI.h>
#include "SpiDataSync.hpp"

/*Don't forget to set Sketchbook location in File/Preferences to the path of your UI project (the parent foder of this INO file)*/

/*Change to your screen resolution*/

/* IR */
constexpr uint8_t IR_SEND_PIN = 1;
// constexpr uint8_t IR_SEND_PIN = 3;
IrSendLight irsendLight(IR_SEND_PIN);
IrSendAc irsendAc(IR_SEND_PIN);
AC_Data ac_data = {false, AC_Mode::Auto, 25};

/* spi通信 */
#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define SPI_CS 5
SpiDataSync spi(HSPI);

ClimateData climate_data;
LightData light_data;
CurtainData curtain_data;
FanData fan_data;

uint32_t last_touched_time = 0;

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */

    display_setup();

    irsendLight.begin();
    irsendAc.begin();

    Serial.println( "Setup done" );

    // SPI
    spi.master_begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_CS);
}

void loop() {
    display_loop();
    irsendAc.getData(ac_data);
    spi.master_sync(climate_data, light_data, ac_data, curtain_data, fan_data);
    char buf[8];
    snprintf(buf, sizeof(buf), "%4.1f", climate_data.temperature/10.0f); // スライダーの値を文字列に変換
    lv_label_set_text(ui_Label3, buf); // ラベルのテキストを更新
    snprintf(buf, sizeof(buf), "%2d", climate_data.humidity);
    lv_label_set_text(ui_Label1, buf); // ラベルのテキストを更新
    snprintf(buf, sizeof(buf), "%4d", climate_data.pressure + 900);
    lv_label_set_text(ui_Label8, buf); // ラベルのテキストを更新
    // あとで，他の値もここに入れる
}

/* Light */
void SwitchLightClicked(lv_event_t * e) {
    // // スイッチの状態を取得し，ラベルを変更する
    lv_obj_t * sw = lv_event_get_target(e);
    bool state = lv_obj_has_state(sw, LV_STATE_CHECKED);

    if (state) {
        irsendLight.send(LightCommand::On);
    } else {
        irsendLight.send(LightCommand::Off);
    }
}


/* Aircon */
void ButtonACOnClickerd(lv_event_t * e)
{
    last_touched_time = millis();
    // Serial.println("ButtonACOnClickerd");
	irsendAc.setPower(true);
}

void ButtonACOffClicked(lv_event_t * e)
{
    last_touched_time = millis();
    irsendAc.setPower(false);
}

void DropdownACModeChanged(lv_event_t * e)
{
    last_touched_time = millis();

    // Serial.println("DropdownACModeChanged");
    lv_obj_t * dropdown = lv_event_get_target(e);
    uint16_t value = lv_dropdown_get_selected(dropdown);
    switch (value) {
    case 0:
        irsendAc.setMode(AC_Mode::Auto);
        // Serial.println("Auto");
        break;
    case 1:
        irsendAc.setMode(AC_Mode::Cool);
        // Serial.println("Cool");
        break;
    case 2:
        irsendAc.setMode(AC_Mode::Heat);
        // Serial.println("Heat");
        break;
    case 3:
        irsendAc.setMode(AC_Mode::Dry);
        // Serial.println("Dry");
        break;
    default:
        break;
    }

    // 温度ラベルの更新
    lv_obj_t * lavel = ui_LabelTemp;
    value = irsendAc.getTemp();
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", value);
    lv_label_set_text(lavel, buf);
}

void ButtonTempUpClicked(lv_event_t * e)
{
	// ui_LabelTemp の値を取得し，1増やして設定する
    lv_obj_t * lavel = ui_LabelTemp;
    int value = atoi(lv_label_get_text(lavel));
    value++;

    // 赤外線
    irsendAc.setTemp(value);
    value = irsendAc.getTemp();

    // 温度ラベルの更新
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", value);
    lv_label_set_text(lavel, buf);
}

void ButtonTempDownClicked(lv_event_t * e)
{
	// ui_LabelTemp の値を取得し，1減らして設定する
    lv_obj_t * lavel = ui_LabelTemp;
    int value = atoi(lv_label_get_text(lavel));
    value--;

    // 赤外線
    irsendAc.setTemp(value);
    value = irsendAc.getTemp();

    // 温度ラベルの更新
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", value);
    lv_label_set_text(lavel, buf);
}
