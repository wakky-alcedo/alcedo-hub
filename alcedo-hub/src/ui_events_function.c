#include <lvgl.h>
#include <ui/ui.h>
#include <stdio.h>
#include "arduino.h"
// #include "ir_dif.cpp"
// #include "IrSendLight.hpp"
uint32_t last_touched_time = 0;

/* Light */
void SwitchLightClicked(lv_event_t * e) {
    // // スイッチの状態を取得し，ラベルを変更する
    lv_obj_t * sw = lv_event_get_target(e);
    bool state = lv_obj_has_state(sw, LV_STATE_CHECKED);
}

void SliderBrightnessChanged(lv_event_t * e) {
    // スライダーの値を取得し，明るさラベルを変更する
    lv_obj_t * slider = lv_event_get_target(e);
    int value = lv_slider_get_value(slider);

    // ラベルオブジェクトを取得
    lv_obj_t * label = ui_LabelBrightness; // Label1 を SquareLine Studio で設定した名前に置き換え
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", value); // スライダーの値を文字列に変換

    lv_label_set_text(label, buf); // ラベルのテキストを更新
}

void SliderColorChanged(lv_event_t * e)
{
    // スライダーの値を取得し，明るさラベルを変更する
    lv_obj_t * slider = lv_event_get_target(e);
    int value = lv_slider_get_value(slider);

    // ラベルオブジェクトを取得
    lv_obj_t * label = ui_LabelColor; // Label1 を SquareLine Studio で設定した名前に置き換え
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", value); // スライダーの値を文字列に変換

    lv_label_set_text(label, buf); // ラベルのテキストを更新
}

/* Aircon */
void ButtonACOnClickerd(lv_event_t * e)
{
    last_touched_time = millis();
    // Serial.println("ButtonACOnClickerd");
}

void ButtonACOffClicked(lv_event_t * e)
{
    last_touched_time = millis();
}

void DropdownACModeChanged(lv_event_t * e)
{
    last_touched_time = millis();

    // Serial.println("DropdownACModeChanged");
    lv_obj_t * dropdown = lv_event_get_target(e);
    uint16_t value = lv_dropdown_get_selected(dropdown);
    switch (value) {
    case 0:
        // Serial.println("Auto");
        break;
    case 1:
        // Serial.println("Cool");
        break;
    case 2:
        // Serial.println("Heat");
        break;
    case 3:
        // Serial.println("Dry");
        break;
    default:
        break;
    }

    // 温度ラベルの更新
    lv_obj_t * lavel = ui_LabelTemp;
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
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", value);
    lv_label_set_text(lavel, buf);
}

/* Curtain */

void SliderOpenChanged(lv_event_t * e)
{
    // スライダーの値を取得し，明るさラベルを変更する
    lv_obj_t * slider = lv_event_get_target(e);
    int value = lv_slider_get_value(slider);

    // ラベルオブジェクトを取得
    lv_obj_t * label = ui_LabelOpenSlider; // Label1 を SquareLine Studio で設定した名前に置き換え
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", value); // スライダーの値を文字列に変換

    lv_label_set_text(label, buf); // ラベルのテキストを更新
}

void ButtonOpenClicked(lv_event_t * e)
{
	// Your code here
}

void ButtonStopClicked(lv_event_t * e)
{
	// Your code here
}

void ButtonCloseClicked(lv_event_t * e)
{
	// Your code here
}
