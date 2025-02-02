#include <lvgl.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <display/display.hpp>
#include <ui/ui.h>
#include "display/CST820.h"

// #include <IRremoteESP8266.h>
// #include <ir_Panasonic.h>
// #include <IRsend.h>
#include "IrSendLight.hpp"

/*Don't forget to set Sketchbook location in File/Preferences to the path of your UI project (the parent foder of this INO file)*/

/*Change to your screen resolution*/

constexpr uint8_t IR_SEND_PIN = 1;
IrSendLight irsendLight(IR_SEND_PIN);

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */

    display_setup();

    irsendLight.begin();

    Serial.println( "Setup done" );
}

void loop()
{
    display_loop();
    delay(5);
}

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