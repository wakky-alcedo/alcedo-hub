#include <lvgl.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <display.hpp>
#include <ui/ui.h>
#include "CST820.h"


/*Don't forget to set Sketchbook location in File/Preferences to the path of your UI project (the parent foder of this INO file)*/

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;
constexpr uint16_t I2C_SDA = 21;
constexpr uint16_t I2C_SCL = 22;

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */

    touch.begin(); /*初始化触摸板*/

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.println( LVGL_Arduino );
    Serial.println( "I am LVGL_Arduino" );

    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print ); /* register print function for debugging */
#endif

    tft.begin();          /* TFT init */
    tft.setRotation( 1 ); /* Landscape orientation, flipped */

    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 10 );

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );


    ui_init();
    // Light
    lv_obj_set_style_text_font(ui_LabelHome1, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelHome1, LV_SYMBOL_NEW_LINE);
    // A/C
    lv_obj_set_style_text_font(ui_LabelHome2, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelHome2, LV_SYMBOL_NEW_LINE);
    lv_label_set_text( ui_LabelTempUp, LV_SYMBOL_UP);
    lv_label_set_text( ui_LabelTempDown, LV_SYMBOL_DOWN);
    lv_obj_set_style_text_font(ui_LabelACOn, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelACOn, LV_SYMBOL_POWER);
    lv_obj_set_style_text_font(ui_LabelACOff, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_label_set_text( ui_LabelACOff, LV_SYMBOL_CLOSE);
    lv_label_set_text( ui_LabelACOff, "OFF");
    // Curtain
    lv_obj_set_style_text_font(ui_LabelHome3, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelHome3, LV_SYMBOL_NEW_LINE);
    lv_obj_set_style_text_font(ui_LabelOpen, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelOpen, LV_SYMBOL_LEFT LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_font(ui_LabelStop, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelStop, LV_SYMBOL_PAUSE);
    lv_obj_set_style_text_font(ui_LabelClose, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelClose, LV_SYMBOL_RIGHT LV_SYMBOL_LEFT);

    Serial.println( "Setup done" );
}

void loop()
{
    lv_timer_handler(); /* let the GUI do its work */

    // sprintf(chbuff, "%02d:%02d", tm->tm_hour, tm->tm_min);
    // lv_label_set_text(uic_sample, chbuff);
    // lv_label_set_text(uic_aa, chbuff);
    // lv_label_get

    delay(5);
}

